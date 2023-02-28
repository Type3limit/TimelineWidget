//
// Created by 58226 on 2023/2/9.
//

#include "Clip/ClipItem.h"
#include "TimelineDefination.h"
#include "timelinewidget.h"
#include "IntervalWatcher.h"
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#define timeline() (GET_POINTER<TimelineWidget>())
ClipItem::ClipItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{

    Init();

}

ClipItem::ClipItem(const QString &key, QGraphicsItem *parent)
{
    m_mimeKey = key;
    Init();
}
ClipItem::~ClipItem()
{
    SAFE_DELETE(m_shadow);
}
void ClipItem::Init()
{
    setFlags(ItemIsSelectable | ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    //setCacheMode(DeviceCoordinateCache)
    m_shadow = new ShadowClipItem(m_mimeKey);
    m_image.load("./test.png");
}
void ClipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //IntervalWatcher iw;
    //iw.start();
    auto countOfTrack = timeline()->getTrackCount();
    auto trackBodyHeight = countOfTrack * TRACK_HEIGHT;
    auto trackData = getTrackData(m_trackMimeKey);
    if (trackData.isDefaultData())
        return;
    auto curData = trackData.getClip(m_mimeKey);
    auto xIndex = ceil(((double)(curData.startPos) / timeline()->frameTick() * MIN_TICK_WIDTH));
    auto width = ceil((double)(curData.duration) / timeline()->frameTick() * MIN_TICK_WIDTH);
    auto curArea = timeline()->getArea(TimelineWidget::RightBottom);
    double yIndex = trackData.index * TRACK_HEIGHT;
    bool shouldDrawText = false;
    bool shouldDrawPic = false;
    if (trackBodyHeight < curArea.height())//y center alignment
    {
        yIndex += (curArea.height() - trackBodyHeight) / 2.0;
    }
    QColor paintColor = Qt::red;
    switch (curData.type) {
    case SpecificType::Subtitle:
        shouldDrawText = true;
        paintColor = SUBTITLE_CLIP_COLOR;
        break;
    case SpecificType::Effect:
        paintColor = EFFECT_CLIP_COLOR;
        shouldDrawPic = true;
        shouldDrawText = true;
        break;
    case SpecificType::Sticker:
        shouldDrawPic = true;
        paintColor = STICKER_CLIP_COLOR;
        break;
    default:
        break;
    }
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(Qt::white));
    painter->setBrush(QBrush(paintColor));
    QPainterPath path;
    path.addRoundedRect(QRectF(xIndex, yIndex + 0.5, width, TRACK_HEIGHT - 1), 5, 5);
    painter->fillPath(path, QBrush(paintColor));
    auto limitedArea = timeline()->getViewPort(TimelineWidget::RightBottom);
    if (m_isOnHover || timeline()->isSelected(m_mimeKey)) {
        painter->drawRoundedRect(QRectF(xIndex, yIndex, width, TRACK_HEIGHT), 5, 5);
    }
//    if (m_isDragMoved) {
//        QPainterPath curPath;
//        curPath.addRoundedRect(m_shadowRect,5,5);
//        painter->fillPath(curPath, QBrush(paintColor));
//    }
    //TODO:draw extra info of clip,maybe we should change the mime data struct
    int startX = xIndex < limitedArea.x() ? limitedArea.x() : xIndex;
    int endX = (xIndex + width) > limitedArea.right() ? limitedArea.right() : (xIndex + width);
    //for test
    painter->setPen(QPen(Qt::white));
    for (int curX = startX; curX <= xIndex + width; curX += 20) {
        if ((xIndex + width) - curX < 20)
            return;
        //if (shouldDrawText) {
        painter->drawText(curX, (int)yIndex, 20, TRACK_HEIGHT, Qt::AlignHCenter | Qt::AlignVCenter, "测试");
        painter->drawText(curX, (int)yIndex + 10, 20, TRACK_HEIGHT, Qt::AlignHCenter | Qt::AlignVCenter, "测试");
        painter->drawText(curX, (int)yIndex + 20, 20, TRACK_HEIGHT, Qt::AlignHCenter | Qt::AlignVCenter, "测试");
        //}
        if (shouldDrawPic) {
            painter->drawPixmap(QRect(curX, yIndex, 20, 20), m_image);
        }
    }
    //iw.stop();
    //qDebug()<<iw.milliSecond()<<" ms to finish clip draw";
}
QRectF ClipItem::boundingRect() const
{
    auto trackBodyHeight = timeline()->getTrackCount() * TRACK_HEIGHT;
    auto trackData = getTrackData(m_trackMimeKey);
    auto curData = trackData.getClip(m_mimeKey);
    auto xIndex = ((double)(curData.startPos) / timeline()->frameTick()) * MIN_TICK_WIDTH;
    auto width = ((double)(curData.duration) / timeline()->frameTick()) * MIN_TICK_WIDTH;
    auto curArea = timeline()->getArea(TimelineWidget::RightBottom);
    double yIndex = trackData.index * TRACK_HEIGHT;
    if (trackBodyHeight < curArea.height())//y center alignment
    {
        yIndex += (curArea.height() - trackBodyHeight) / 2.0;
    }
    return QRectF(xIndex, yIndex, width, TRACK_HEIGHT);
}
bool ClipItem::insertToTrack(const QString &trackKey)
{
    if (!m_trackMimeKey.isEmpty())
        return false;
    m_trackMimeKey = trackKey;
    //qDebug()<<"insert clip :"<<m_mimeKey<<"to track:"<<m_trackMimeKey;
    return true;
}
bool ClipItem::removeFromTrack()
{
    if (m_trackMimeKey.isEmpty()) {
        return false;
    }
    m_trackMimeKey = "";
    return true;
}
ClipMime ClipItem::getMimeData(const QString &clipKey, bool searchWhenTrackKeyEmpty) const
{
    return getTrackData(m_trackMimeKey, searchWhenTrackKeyEmpty).getClip(clipKey);
}
TrackMime ClipItem::getTrackData(const QString &trackKey, bool searchWhenTrackKeyEmpty) const
{
    if (trackKey.isEmpty()) {
        if (!searchWhenTrackKeyEmpty)
            return {};
        auto TrackCount = timeline()->getTrackCount();
        TrackMime curData;
        for (int i = 0; i < TrackCount; i++) {
            curData = timeline()->getTrackData(i);
            if (!curData.getClip(m_mimeKey).isDefaultData()) {
                break;
            }
        }
        return curData;
    }
    else {
        TrackMime curTrackData;
        if (!timeline()->getTrackData(curTrackData, trackKey))
            return {};
        return curTrackData;
    }

}
QVariant ClipItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    return QGraphicsItem::itemChange(change, value);
}
void ClipItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::LeftButton) {
        if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier)) {
            timeline()->setSelectedClip(QList<QString>{m_mimeKey}, timeline()->isSelected(m_mimeKey));
        }
            //TODO:add width expand process
        else {

            if (!timeline()->isSelected(m_mimeKey)) {
                timeline()->setSelectedClip(m_mimeKey, false);
            }
            m_prePoint = event->scenePos();
            timeline()->clipMoved(0, 0, false);
            //TODO:add clip left and right region to expanded its length
            m_isMouseDrag = true;
        }
        event->accept();
    }
    else {
        QGraphicsItem::mousePressEvent(event);
    }

}
void ClipItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_isMouseDrag) {
        event->accept();
        timeline()->clipMoved(0, 0, true);

    }
    else {
        QGraphicsItem::mouseReleaseEvent(event);
    }

}
void ClipItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_isMouseDrag) {
        auto xDelta = event->scenePos().x() - m_prePoint.x();
        auto yDelta = event->scenePos().y() - m_prePoint.y();
        m_prePoint = event->scenePos();
        //TODO: add left and right drag area to expand clip length;
        //TODO: Collision detection of clips unselected
        timeline()->clipMoved((int)xDelta, (int)yDelta, false);

        event->accept();
    }
    else {
        QGraphicsItem::mouseMoveEvent(event);
    }

}
void ClipItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_isOnHover = true;
    event->ignore();
    QGraphicsItem::hoverEnterEvent(event);
}
void ClipItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_isOnHover = false;
    QGraphicsItem::hoverLeaveEvent(event);
}
void ClipItem::clipDrag(int x, int y)
{
    if (this->scene() == nullptr)
        return;
    if (!m_isDragMoved) {
        m_isDragMoved = true;
        m_originRect = m_shadowRect = boundingRect();

        this->scene()->addItem(m_shadow);
    }
    else {
        m_shadowRect = QRectF(m_shadowRect.x() + x, m_shadowRect.y() + y,
                              m_shadowRect.width(), m_shadowRect.height());
        if (m_shadow) {
            m_shadow->setDrawRect(m_shadowRect);
            m_shadow->forceUpdate();
        }
    }
}
void ClipItem::stopClipDrag()
{
    if (this->scene() == nullptr)
        return;
    if (m_isDragMoved) {
        if (m_shadow) {
            m_shadow->setDrawRect({0, 0, 0, 0});
            m_shadow->forceUpdate();
        }
        this->scene()->removeItem(m_shadow);
        auto xDelta = m_shadowRect.x() - m_originRect.x();
        auto curMime = getMimeData(m_mimeKey);
        if (curMime.isDefaultData())
            return;
        auto curFrameMoved = (long)ceil(xDelta * timeline()->percentPerUnit());
        long curX = (long)curMime.startPos;
        curMime.startPos = curX + curFrameMoved < 0 ? 0 :
                           (curX + curFrameMoved > timeline()->maxDuration() ?
                            timeline()->maxDuration() : curX + curFrameMoved);
        TrackMime readyTrackData;
        // using center position to determine which track to insert
        timeline()->getTrackByVerticalPos((m_shadowRect.y() + TRACK_HEIGHT / 2), readyTrackData);
        bool isChangeTrack = false;
        auto originTrackKey = curMime.trackId;
        if (!readyTrackData.isDefaultData()) {
            isChangeTrack = true;
            curMime.trackId = readyTrackData.id;
        }
        checkForCollision(curMime,isChangeTrack?originTrackKey:"");
        timeline()->alterClipData(curMime.id, m_trackMimeKey, curMime);
        prepareGeometryChange();
    }
    m_isDragMoved = false;
}
void ClipItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    timeline()->setSelectedClip(m_mimeKey, timeline()->isSelected(m_mimeKey));
    QGraphicsItem::mouseDoubleClickEvent(event);
}
void ClipItem::forceUpdate()
{
    prepareGeometryChange();
    update();
}
void ClipItem::checkForCollision(ClipMime curMime,const QString& originTrackKey)
{
    auto curRange = ClipRange(timeline()->m_clipRange);
    curRange.oneClipChanged(curMime,originTrackKey);
    QList<QString> collisionItems;
    if (!curRange.hasCollision(curMime.trackId, curMime.id, collisionItems)) {
        return;
    }
    TrackMime trackData;
    if (!timeline()->getTrackData(trackData, originTrackKey.isEmpty()?m_trackMimeKey:curMime.trackId)) {
        return;
    }
    auto collisionClips = trackData.getClips([](const ClipMime &curItem) -> bool
                                             { return !(timeline()->isSelected(curItem.id)); });
    QList<ClipMime> headClips, tailClips;
    ExtensionMethods::SourcesExtension<ClipMime>::classification(collisionClips,
                                                                 [&curMime](const ClipMime &item) -> bool
                                                                 {
                                                                     return item.startPos <= curMime.startPos;
                                                                 },
                                                                 headClips,
                                                                 tailClips);
    //let these clips in order
    std::sort(headClips.begin(), headClips.end(), [](const ClipMime &l, const ClipMime &r) -> bool
    {
        return l.startPos < r.startPos;
    });
    std::sort(tailClips.begin(), tailClips.end(), [](const ClipMime &l, const ClipMime &r) -> bool
    {
        return l.startPos < r.startPos;
    });
    //TODO:for head ,depart it.

    //for tail ,if front space is not enough,move tail clips.
    if(tailClips.count()>0&&tailClips[0].startPos<curMime.startPos+curMime.duration)
    {
        auto tailDiff = (curMime.startPos+curMime.duration)-tailClips[0].startPos;
        for (int i = 0; i < tailClips.count(); i++) {

            tailClips[i].startPos+=tailDiff;
            timeline()->alterClipData(tailClips[i].id,tailClips[i].trackId,tailClips[i]);
        }
    }
}

