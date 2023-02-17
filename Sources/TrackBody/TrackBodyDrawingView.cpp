//
// Created by 58226 on 2023/2/6.
//

#include "TrackBody/TrackBodyDrawingView.h"
#include <QEvent>
#include <QKeyEvent>
#include <QApplication>
#include "timelinewidget.h"
#include <QMap>
#include <QtMath>
#define TimelineInstance() (GET_POINTER<TimelineWidget>())
typedef ExtensionMethods::SourcesExtension<QGraphicsItem *> itemsEx;
TrackBodyDrawingView::TrackBodyDrawingView(QWidget *parent)
    : SelfContainedSceneView(parent)
{
    m_anchorBody = new AnchorBodyItem();
    m_selectObj = new TrackSelectionItem();
    scene()->addItem(m_anchorBody);
    m_anchorBody->setZValue(999);//Anchor TopMost
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    connect(TimelineInstance(), &TimelineWidget::TrackUpdated, this, &TrackBodyDrawingView::OnTrackHeadUpdate);
    connect(horizontalScrollBar(), &QScrollBar::sliderMoved, this, &TrackBodyDrawingView::ScrolledTo);
    connect(TimelineInstance(), &TimelineWidget::PositionChanged,
            m_anchorBody, &AnchorBodyItem::OnTimelinePosChanged, Qt::ConnectionType::QueuedConnection);
    connect(TimelineInstance(), &TimelineWidget::TrackClipChanged, this, &TrackBodyDrawingView::ClipChanged);
    connect(TimelineInstance(), &TimelineWidget::ClipUpdated, this,
            &TrackBodyDrawingView::singleClipChanged, Qt::ConnectionType::QueuedConnection);
}
TrackBodyDrawingView::~TrackBodyDrawingView()
{
    SAFE_DELETE(m_anchorBody)
    ExtensionMethods::SourcesExtension<QString>::eachBy(m_bodyItems.keys(), [&](const QString &key) -> void
    {
        auto curItem = m_bodyItems[key];
        SAFE_DELETE(curItem)
    });
    m_bodyItems.clear();
}
void TrackBodyDrawingView::ScrolledTo(int pos)
{
    auto curRect = getViewPortRect();
    update(QRect((int)curRect.x(), (int)curRect.y(), (int)curRect.width(), (int)curRect.height()));
    emit ScrollChanged(pos);
}

void TrackBodyDrawingView::wheelEvent(QWheelEvent *event)
{
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
        QGraphicsView::wheelEvent(event);
    }
    event->ignore();
    auto curRect = getViewPortRect();
    update(QRect((int)curRect.x(), (int)curRect.y(), (int)curRect.width(), (int)curRect.height()));
}
void TrackBodyDrawingView::mouseMoveEvent(QMouseEvent *evt)
{
    if (m_bIsRightButtonPressed) {
        auto deltaX = evt->pos().x() - m_prePos.x();
        auto deltaY = evt->pos().y() - m_prePos.y();
        auto maxX = horizontalScrollBar()->maximum();
        auto maxY = verticalScrollBar()->maximum();
        auto x = (int)(horizontalScrollBar()->value() - deltaX);
        auto y = (int)(verticalScrollBar()->value() - deltaY);
        x = x < 0 ? 0 : (x > maxX ? maxX : x);
        y = y < 0 ? 0 : (y > maxY ? maxY : y);
        horizontalScrollBar()->setValue(x);
        verticalScrollBar()->setValue(y);
        emit ScrollChanged(x);
        m_prePos = evt->pos();
        evt->accept();
    }

    if (m_bIsMultiSelectionMode) {
        m_selectionEnd = mapToScene(evt->pos());
        m_selectObj->setPos(m_selectionStart, m_selectionEnd);
        m_selectObj->update();
        evt->accept();
    }
    else if (!evt->modifiers().testFlag(Qt::ShiftModifier) && evt->buttons().testFlag(Qt::LeftButton)) {
        QPoint pos = evt->pos();
        int XDirection = 0;
        int YDirection = 0;
        int distance = SCROLL_DISTANCE;
        if (pos.x() < SCROLL_DISTANCE) {
            XDirection = -1;
        }
        else if (width() - pos.x() < SCROLL_DISTANCE) {
            XDirection = 1;
        }
        if (pos.y() < SCROLL_DISTANCE) {
            YDirection = -1;
        }
        else if (height() - pos.y() < SCROLL_DISTANCE) {
            YDirection = 1;
        }
        if (XDirection != 0) {
            auto cur = horizontalScrollBar()->value();
            horizontalScrollBar()->setValue(cur - (distance * XDirection));
            emit ScrolledTo(cur - (distance * XDirection));
        }
        if (YDirection != 0) {
            auto cur = verticalScrollBar()->value();
            verticalScrollBar()->setValue(cur - (distance * XDirection));
        }

    }


    QGraphicsView::mouseMoveEvent(evt);
}
void TrackBodyDrawingView::mousePressEvent(QMouseEvent *event)
{
    if (!m_bIsRightButtonPressed && event->button() == Qt::RightButton) {
        m_bIsRightButtonPressed = true;
        event->accept();
        m_prePos = event->pos();
    }
    else if (event->button() == Qt::LeftButton)//may on tracks or anchor body
    {
        auto curItems = items(event->pos());
        auto Clips = itemsEx::where(curItems, [&](QGraphicsItem *itr) -> bool
        {
            return dynamic_cast<ClipItem *>(itr) != nullptr;
        });
        bool isAnchor = curItems.size() == 1 && dynamic_cast<AnchorBodyItem *>(curItems.first()) != nullptr;

        if (!isAnchor && Clips.count() <= 0) {
            m_bIsMultiSelectionMode = true;
            m_selectionStart = mapToScene(event->pos());
            scene()->addItem(m_selectObj);
            m_selectObj->setPos(m_selectionStart, m_selectionStart);
            event->accept();
        }
        else {
            QGraphicsView::mousePressEvent(event);
        }
    }
    else {
        QGraphicsView::mousePressEvent(event);
    }

}
void TrackBodyDrawingView::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_bIsRightButtonPressed && event->button() == Qt::RightButton) {
        m_bIsRightButtonPressed = false;
        event->accept();
    }
    else if (m_bIsMultiSelectionMode) {
        m_bIsMultiSelectionMode = false;
        m_selectionEnd = mapToScene(event->pos());
        scene()->removeItem(m_selectObj);
        //TODO: get items in selection area
        event->accept();
    }
    else {
        QGraphicsView::mouseReleaseEvent(event);
    }
}
void TrackBodyDrawingView::scrollToCursor()
{
    auto curArea = TimelineInstance()->getArea(TimelineWidget::RightBottom);
    auto curLeftPosOfCursor =
        ((double)TimelineInstance()->curPos() / (double)TimelineInstance()->frameTick()) * MIN_TICK_WIDTH;
    auto leftPos = (curLeftPosOfCursor - curArea.width() / 2);
    leftPos = leftPos < 0 ? 0 : leftPos;
    horizontalScrollBar()->setValue((int)leftPos);
    emit ScrollChanged((int)(leftPos));
    update();
    //auto updateRect = getViewPortRect();
    //update(updateRect.x(),updateRect.y(),updateRect.width(),updateRect.height());
}
bool TrackBodyDrawingView::addTrackBody(const TrackMime &originData)
{
    auto key = originData.id;
    if (m_bodyItems.contains(key)) {
        qDebug() << "add track body failed! Already has the same key:" << key;
        return false;
    }
    auto curData = new TrackBodyItem(originData);
    m_bodyItems.insert(key, curData);
    scene()->addItem(curData);
    return true;
}
bool TrackBodyDrawingView::deleteTrackBody(const QString &key)
{
    if (m_bodyItems.contains(key)) {
        auto org = m_bodyItems[key];
        scene()->removeItem(org);
        m_bodyItems.remove(key);
        SAFE_DELETE(org)
        return true;
    }
    qDebug() << "delete track head failed!,key:[" << key << "]is not exist";
    return false;
}
TrackBodyItem *TrackBodyDrawingView::getTrackBody(const QString &key)
{
    if (!m_bodyItems.contains(key))
        return nullptr;
    return m_bodyItems[key];
}
TrackBodyItem *TrackBodyDrawingView::updateTrackBody(const QString &key, TrackBodyItem *curData)
{
    if (!m_bodyItems.contains(key))
        return nullptr;
    auto org = m_bodyItems[key];
    if (org != curData) {
        m_bodyItems[key] = curData;
        SAFE_DELETE(org)
    }
    return m_bodyItems[key];
}
void TrackBodyDrawingView::OnTrackHeadUpdate(const QString &key)
{
    if (!m_bodyItems.contains(key))
        return;
    m_bodyItems[key]->forceUpdate();
}
void TrackBodyDrawingView::ScrollToPos(int pos)
{
    horizontalScrollBar()->setValue(pos);

}

void TrackBodyDrawingView::ClipChanged(const QString &trackKey, const QString &clipKey, int mode)
{
    auto curTrack = getTrackBody(trackKey);
    if (!curTrack)
        return;
    if (mode > 0) {
        curTrack->addClipItem(clipKey);
    }
    else {
        curTrack->removeClipItem(clipKey);
    }
    updateGeometry();
}
void TrackBodyDrawingView::singleClipChanged(const QString &trackKey, const QString &clipKey)
{
    auto curTrack = getTrackBody(trackKey);
    if (!curTrack)
        return;
    auto curRect = getViewPortRect();
    curTrack->updateClipItem(clipKey);
    update((int)curRect.x(), (int)curRect.y(), (int)curRect.width(), (int)curRect.height());
}
void TrackBodyDrawingView::setDrawingAreaSize(int width, int height)
{
    SelfContainedSceneView::setDrawingAreaSize(width, height);
}



