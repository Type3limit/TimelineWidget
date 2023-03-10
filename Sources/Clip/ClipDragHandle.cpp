//
// Created by 58226 on 2023/3/1.
//

#include "Clip/ClipDragHandle.h"
#include <QCursor>
#include <QPainter>
void ClipDragHandle::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::CursorShape::SizeHorCursor);
    QGraphicsItem::hoverEnterEvent(event);
}
void ClipDragHandle::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::CursorShape::ArrowCursor);
    QGraphicsItem::hoverLeaveEvent(event);
}
QRectF ClipDragHandle::boundingRect() const
{
    return m_drawingRect;
}

void ClipDragHandle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPainterPath path;
    painter->setRenderHint(QPainter::RenderHint::Antialiasing);
    path.addRoundedRect(m_drawingRect,2,2);
    painter->fillPath(path,QBrush(QColor(255,0,0,0)));
}
