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
#define  TimelineInstance() (GET_POINTER<TimelineWidget>())

AnchorHeadItem::AnchorHeadItem(QGraphicsItem *object)
    : QGraphicsObject(object)
{

    setFlags(
        QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    //setCacheMode(QGraphicsItem::CacheMode::DeviceCoordinateCache);
    connect(TimelineInstance(), &TimelineWidget::FrameTickChanged, this, [&](auto data) -> void
    { this->prepareGeometryChange(); });

}

QRectF AnchorHeadItem::boundingRect() const
{
    auto curXPos = ((double)(TimelineInstance()->curPos())
        / (double)(TimelineInstance()->frameTick())) * MIN_TICK_WIDTH;
    auto curYPos = TimelineInstance()->getArea(TimelineWidget::RightTop).height();
    auto bw = TIMELINE_ANCHOR_WIDTH / 2;//bisection of width

    return {curXPos - bw, curYPos - bw, TIMELINE_ANCHOR_WIDTH, TIMELINE_ANCHOR_WIDTH};
}
void AnchorHeadItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //intervalwatcher iw;
    //iw.start();
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPainterPath path;
    auto curXPos = ceil(((double)(TimelineInstance()->curPos())
        / (double)(TimelineInstance()->frameTick())) * MIN_TICK_WIDTH);
    auto curYPos = TimelineInstance()->getArea(TimelineWidget::RightTop).height() + WIDGET_MARGIN;
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
void AnchorHeadItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor(Qt::CursorShape::SizeHorCursor));
    QGraphicsItem::hoverEnterEvent(event);
}
void AnchorHeadItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor());
    QGraphicsItem::hoverLeaveEvent(event);
}
void AnchorHeadItem::onTimelinePosChanged(ulong pos)
{
    prepareGeometryChange();
}
void AnchorHeadItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
void AnchorHeadItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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
void AnchorHeadItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_bIsStartDrag) {
        m_bIsStartDrag = false;
        event->accept();
    }
    QGraphicsItem::mouseReleaseEvent(event);
}
void AnchorHeadItem::forceUpdate()
{
    prepareGeometryChange();
    update();
}

