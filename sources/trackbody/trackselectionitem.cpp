//
// Created by 58226 on 2023/2/16.
//

#include "TrackBody/trackselectionitem.h"
#include "timelinedefination.h"
#include <QPainter>
void TrackSelectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(Qt::lightGray,1,Qt::DashLine));
    painter->drawRoundedRect(boundingRect(),15,15);
}
QRectF TrackSelectionItem::boundingRect() const
{
    if(m_startPos.x()>m_endPos.x()&&m_startPos.y()>m_endPos.y())
    {
        return {m_endPos,m_startPos};
    }
    else
    {
        return {m_startPos,m_endPos};
    }

}
void TrackSelectionItem::setPos(QPointF start, QPointF end)
{
    m_startPos = start;
    m_endPos = end;
}
