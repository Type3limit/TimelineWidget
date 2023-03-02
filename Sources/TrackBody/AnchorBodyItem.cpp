//
// Created by 58226 on 2023/2/7.
//

#include "TrackBody/AnchorBodyItem.h"
#include "TimelineDefination.h"
#include "TimelineWidget.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#define TimelineInstance() (GET_POINTER<TimelineWidget>())
AnchorBodyItem::AnchorBodyItem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    setFlags(
        QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setCacheMode(QGraphicsItem::CacheMode::DeviceCoordinateCache);
    connect(TimelineInstance(), &TimelineWidget::FrameTickChanged, this, [&](auto data) -> void
    { this->prepareGeometryChange(); });
}
QRectF AnchorBodyItem::boundingRect() const
{
    auto
        curXPos = ((double)(TimelineInstance()->curPos()) / (double)(TimelineInstance()->frameTick())) * MIN_TICK_WIDTH;
    auto curYPos = TimelineInstance()->getArea(TimelineWidget::RightBottom).height();
    return {curXPos - 0.5, 0, TIMELINE_ANCHOR_BODY_WIDTH, curYPos};
}
void AnchorBodyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (!CHECK_POINTER<TimelineWidget>())
        return;
    auto curXPos = ceil((double)(TimelineInstance()->curPos())
        / (double)(TimelineInstance()->frameTick()) * MIN_TICK_WIDTH);
    auto curHeight = TimelineInstance()->getArea(TimelineWidget::Area::RightBottom).height();
    painter->setPen(QPen(ANCHOR_COLOR));
    painter->setBrush(QBrush(ANCHOR_COLOR));
    painter->drawRect(QRectF(curXPos , 0, 1, curHeight));
}
void AnchorBodyItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor(Qt::CursorShape::SizeHorCursor));
    QGraphicsItem::hoverEnterEvent(event);
}
void AnchorBodyItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor());
    QGraphicsItem::hoverLeaveEvent(event);
}
void AnchorBodyItem::OnTimelinePosChanged(ulong pos)
{
    prepareGeometryChange();
}
void AnchorBodyItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (!m_bIsStartDrag) {
        event->ignore();
        m_bIsStartDrag = true;
        //m_curPos=event->scenePos();
        event->accept();
    }
    else {
        QGraphicsItem::mousePressEvent(event);
    }
}
void AnchorBodyItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_bIsStartDrag) {
        if (!CHECK_POINTER<TimelineWidget>())
            return;
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
void AnchorBodyItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_bIsStartDrag) {
        m_bIsStartDrag = false;
        event->accept();
    }
    QGraphicsItem::mouseReleaseEvent(event);
}
