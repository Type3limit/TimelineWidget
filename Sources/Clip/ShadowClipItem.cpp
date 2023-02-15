//
// Created by 58226 on 2023/2/14.
//

#include "Clip/ShadowClipItem.h"
#include <QPainter>
void ShadowClipItem::setDrawRect(const QRectF &rec)
{
    m_drawRect = rec;
}
void ShadowClipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setPen(Qt::black);
    painter->drawRect(m_drawRect);
    painter->fillRect(m_drawRect,QBrush(QColor(157,171,194,128)));
}
void ShadowClipItem::forceUpdate()
{
    prepareGeometryChange();
}
QRectF ShadowClipItem::boundingRect() const
{
    return m_drawRect;
}
