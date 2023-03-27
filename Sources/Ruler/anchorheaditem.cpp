//
// Created by 58226 on 2023/2/7.
//

#include "Ruler/anchorheaditem.h"
#include "timelinedefination.h"
#include "timelinewidget.h"
#include "intervalwatcher.h"
#include <QApplication>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#define  TimelineInstance() (GET_POINTER<timelinewidget>())

anchorheaditem::anchorheaditem(QGraphicsItem *object)
    : QGraphicsObject(object)
{

    setFlags(
        QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    //setCacheMode(QGraphicsItem::CacheMode::DeviceCoordinateCache);
    connect(TimelineInstance(), &timelinewidget::FrameTickChanged, this, [&](auto data) -> void
    { this->prepareGeometryChange(); });

}

QRectF anchorheaditem::boundingRect() const
{
    auto curXPos = ((double)(TimelineInstance()->curPos())
        / (double)(TimelineInstance()->frameTick())) * MIN_TICK_WIDTH;
    auto curYPos = TimelineInstance()->getArea(timelinewidget::RightTop).height();
    auto bw = TIMELINE_ANCHOR_WIDTH / 2;//bisection of width

    return {curXPos - bw, curYPos - bw, TIMELINE_ANCHOR_WIDTH, TIMELINE_ANCHOR_WIDTH};
}
void anchorheaditem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //intervalwatcher iw;
    //iw.start();
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPainterPath path;
    auto curXPos = ceil(((double)(TimelineInstance()->curPos())
        / (double)(TimelineInstance()->frameTick())) * MIN_TICK_WIDTH);
    auto curYPos = TimelineInstance()->getArea(timelinewidget::RightTop).height() + WIDGET_MARGIN;
    auto bw = TIMELINE_ANCHOR_WIDTH / 2;//bisection of width
    painter->setPen(QPen(ANCHOR_COLOR));
    painter->setBrush(QBrush(ANCHOR_COLOR));
    //Top triangle
    path.moveTo(curXPos - bw, curYPos - TIMELINE_ANCHOR_WIDTH);
    path.lineTo(curXPos + bw, curYPos - TIMELINE_ANCHOR_WIDTH);
    path.lineTo(curXPos + bw, curYPos - bw);
    path.lineTo(curXPos, curYPos);
    path.lineTo(curXPos - bw, curYPos - bw);
    path.lineTo(curXPos - bw, curYPos - TIMELINE_ANCHOR_WIDTH);

    painter->fillPath(path, QBrush(ANCHOR_COLOR));
    //iw.stop();
    //qDebug()<<iw.milliSecond()<<"ms after anchor head repaint";
}
void anchorheaditem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor(Qt::CursorShape::SizeHorCursor));
    QGraphicsItem::hoverEnterEvent(event);
}
void anchorheaditem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor());
    QGraphicsItem::hoverLeaveEvent(event);
}
void anchorheaditem::OnTimelinePosChanged(ulong pos)
{
    prepareGeometryChange();
}
void anchorheaditem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_bIsStartDrag) {
        event->accept();
        m_bIsStartDrag = true;
        //m_curPos=event->scenePos();
    }
    else {
        QGraphicsItem::mousePressEvent(event);
    }

}
void anchorheaditem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_bIsStartDrag) {
        event->accept();
        auto deltaOfX = event->scenePos().x() / scene()->width();
        auto actualFrame = (double)TimelineInstance()->maxDuration() * deltaOfX;
        actualFrame =
            actualFrame < 0 ? 0 : (actualFrame > TimelineInstance()->maxDuration() ? TimelineInstance()->maxDuration()
                                                                                   : actualFrame);
        TimelineInstance()->setCurPos((ulong)actualFrame);
    }
    else {
        QGraphicsItem::mouseMoveEvent(event);
    }
}
void anchorheaditem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_bIsStartDrag) {
        m_bIsStartDrag = false;
        event->accept();
    }
    QGraphicsItem::mouseReleaseEvent(event);
}
void anchorheaditem::forceUpdate()
{
    prepareGeometryChange();
    update();
}

