//
// Created by 58226 on 2023/2/6.
//

#include "Ruler/RulerDrawingView.h"
#include <QScrollBar>
#include "TimelineWidget.h"
#include "TimelineDefination.h"
#define TimelineInstance() (GET_POINTER<TimelineWidget>())

RulerDrawingView::RulerDrawingView(QWidget *parent)
    : SelfContainedSceneView(parent)
{

    setFocusPolicy(Qt::FocusPolicy::NoFocus);
    m_ruler = new RulerItem();
    m_anchorHead = new AnchorHeadItem(m_ruler);
    scene()->addItem(m_ruler);
    //scene()->addItem(m_anchorHead);
    auto timelineWidget = TimelineInstance();
    connect(timelineWidget, &TimelineWidget::PositionChanged, m_anchorHead,
            [&](ulong curpos)->void{
             m_ruler->setUpdateRect(getViewPortRect());
             m_anchorHead->OnTimelinePosChanged(curpos);
        }, Qt::ConnectionType::QueuedConnection);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
#pragma region setStyle
    horizontalScrollBar()->setStyleSheet(" QScrollBar:horizontal\n"
                                         "    {\n"
                                         "        height: 0px;\n"
                                         "        margin: 0px 0px 0px 0px;\n"
                                         "        border: 0px transparent #2A2929;\n"
                                         "        border-radius: 5px;\n"
                                         "        background-color:#000000;    \n"
                                         "    }"
                                         " QScrollBar::handle:horizontal\n"
                                         "    {\n"
                                         "        background-color: #515151;      \n"
                                         "        min-width: 0px;\n"
                                         "        border-radius: 5px;\n"
                                         "    }"
                                         " QScrollBar::add-line:horizontal\n"
                                         "    {\n"
                                         "        margin: 0px;\n"
                                         "        border-image: url(:/qss_icons/rc/right_arrow_disabled.png);\n"
                                         "        width: 0px;\n"
                                         "        height: 0px;\n"
                                         //            "        subcontrol-position: right;\n"
                                         //            "        subcontrol-origin: margin;\n"
                                         "    }"
                                         " QScrollBar::sub-line:horizontal\n"
                                         "    {\n"
                                         "        margin: 0px;\n"
                                         "        border-image: url(:/qss_icons/rc/left_arrow_disabled.png);\n"
                                         "        height: 0px;\n"
                                         "        width: 0px;\n"
                                         //            "        subcontrol-position: left;\n"
                                         //            "        subcontrol-origin: margin;\n"
                                         "    }"
                                         " QScrollBar::add-line:horizontal:hover,QScrollBar::add-line:horizontal:on\n"
                                         "    {\n"
                                         "        border-image: url(:/qss_icons/rc/right_arrow.png);\n"
                                         "        height: 0px;\n"
                                         "        width: 0px;\n"
                                         //            "        subcontrol-position: right;\n"
                                         //            "        subcontrol-origin: margin;\n"
                                         "    }"
                                         " QScrollBar::sub-line:horizontal:hover, QScrollBar::sub-line:horizontal:on\n"
                                         "    {\n"
                                         "        border-image: url(:/qss_icons/rc/left_arrow.png);\n"
                                         "        height: 0px;\n"
                                         "        width: 0px;\n"
                                         //            "        subcontrol-position: left;\n"
                                         //            "        subcontrol-origin: margin;\n"
                                         "    }"
                                         " QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:horizontal\n"
                                         "    {\n"
                                         "        background: none;\n"
                                         "    }"
                                         " QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal\n"
                                         "    {\n"
                                         "        background: none;\n"
                                         "    }\n");
#pragma endregion
}

void RulerDrawingView::OnTrackBodyScroll(int pos)
{
    horizontalScrollBar()->setValue(pos);
    updateGeometry();
    m_ruler->setUpdateRect( getViewPortRect());
    if(m_anchorHead)
    {
        m_anchorHead->forceUpdate();
    }
}

void RulerDrawingView::mousePressEvent(QMouseEvent *event)
{
    auto curItems = items(event->pos());
    //qDebug() << curItems;
    if (ExtensionMethods::SourcesExtension<QGraphicsItem *>::firstOf(curItems, [&](auto obj) -> bool{
        return dynamic_cast<AnchorHeadItem *>(obj) != nullptr;}, nullptr) == nullptr) {
        auto deltaOfX = (event->x()+horizontalScrollBar()->value()) / (double)m_nWidth;
//        qDebug()<<event->x()<<":"<<horizontalScrollBar()->value();
        auto actualFrame = (double)TimelineInstance()->maxDuration() * deltaOfX;
        actualFrame =
            actualFrame < 0 ? 0 : (actualFrame > TimelineInstance()->maxDuration() ? TimelineInstance()->maxDuration()
                                                                                   : actualFrame);
        TimelineInstance()->setCurPos((ulong)actualFrame);
        event->accept();
    }

    SelfContainedSceneView::mousePressEvent(event);
}
void RulerDrawingView::mouseMoveEvent(QMouseEvent *evt)
{
    if(!evt->modifiers().testFlag(Qt::ShiftModifier) && evt->buttons().testFlag(Qt::LeftButton))
    {
        QPoint pos = evt->pos();
        int XDirection = 0;
        int YDirection = 0;
        int distance = SCROLL_DISTANCE;
        if(pos.x() < SCROLL_DISTANCE) {
            XDirection = -1;
        }
        else if(width()-pos.x() < SCROLL_DISTANCE) {
            XDirection = 1;
        }
        if(XDirection!=0)
        {
            auto cur = horizontalScrollBar()->value();
            horizontalScrollBar()->setValue(cur-(distance*XDirection));
            emit OnAnchorHeadReachEdge(cur-(distance*XDirection));
        }

    }
    QGraphicsView::mouseMoveEvent(evt);
}
void RulerDrawingView::rulerUpdate()
{
    m_ruler->setUpdateRect( getViewPortRect());
    m_ruler->OnLenthChange();
}
void RulerDrawingView::setDrawingAreaSize(int width, int height)
{
    SelfContainedSceneView::setDrawingAreaSize(width, height);
}
