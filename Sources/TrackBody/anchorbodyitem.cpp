//
// Created by 58226 on 2023/2/7.
//

#include "TrackBody/anchorbodyitem.h"
#include "timelinedefination.h"
#include "timelinewidget.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#define TimelineInstance() (GET_POINTER<timelinewidget>())
anchorbodyitem::anchorbodyitem(QGraphicsItem *parent)
    : QGraphicsObject(parent)
{
    setFlags(
        QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setCacheMode(QGraphicsItem::CacheMode::DeviceCoordinateCache);
    connect(TimelineInstance(), &timelinewidget::FrameTickChanged, this, [&](auto data) -> void
    { this->prepareGeometryChange(); });
}
QRectF anchorbodyitem::boundingRect() const
{

    auto curXPos = ceil((double)(TimelineInstance()->curPos())
                            / (double)(TimelineInstance()->frameTick()) * MIN_TICK_WIDTH);
    double curHeight = TimelineInstance()->getArea(timelinewidget::Area::RightBottom).height();
    auto actualHeight = (double)TimelineInstance()->m_timelineData.tracks.size() * TRACK_HEIGHT;
    actualHeight =
        actualHeight < curHeight? curHeight - WIDGET_MARGIN * 2
                                : actualHeight;
    return {curXPos - 0.5, 0, TIMELINE_ANCHOR_BODY_WIDTH, actualHeight};
}
void anchorbodyitem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (!CHECK_POINTER<timelinewidget>())
        return;
    auto curXPos = ceil((double)(TimelineInstance()->curPos())
        / (double)(TimelineInstance()->frameTick()) * MIN_TICK_WIDTH);
    double curHeight = TimelineInstance()->getArea(timelinewidget::Area::RightBottom).height();
    auto actualHeight = (double)TimelineInstance()->m_timelineData.tracks.size() * TRACK_HEIGHT;
    actualHeight =
        actualHeight < curHeight? curHeight - WIDGET_MARGIN * 2
                                                       : actualHeight;
    painter->setPen(QPen(ANCHOR_COLOR));
    painter->setBrush(QBrush(ANCHOR_COLOR));
    painter->drawRect(QRectF(curXPos , 0, 1, (int)actualHeight));
}
void anchorbodyitem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor(Qt::CursorShape::SizeHorCursor));
    QGraphicsItem::hoverEnterEvent(event);
}
void anchorbodyitem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(QCursor());
    QGraphicsItem::hoverLeaveEvent(event);
}
void anchorbodyitem::OnTimelinePosChanged(ulong pos)
{
    prepareGeometryChange();
}
void anchorbodyitem::mousePressEvent(QGraphicsSceneMouseEvent *event)
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
void anchorbodyitem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_bIsStartDrag) {
        if (!CHECK_POINTER<timelinewidget>())
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
void anchorbodyitem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_bIsStartDrag) {
        m_bIsStartDrag = false;
        event->accept();
    }
    QGraphicsItem::mouseReleaseEvent(event);
}
void anchorbodyitem::forceUpdate()
{
    prepareGeometryChange();
    update();
}
