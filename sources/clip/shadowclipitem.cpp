//
// Created by 58226 on 2023/2/14.
//

#include "Clip/shadowclipitem.h"
#include <QPainter>
#include <qdebug.h>
ShadowClipItem::~ShadowClipItem()
{
    qDebug()<<"delete shadow";
}
void ShadowClipItem::setDrawRect(const QRectF &rec)
{
    m_drawRect = rec;
}
void ShadowClipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(Qt::black);
    painter->drawRoundedRect(m_drawRect,5,5);
    QPainterPath path;
    path.addRoundedRect(m_drawRect,5,5);
    painter->fillPath(path,QBrush(QColor(157,171,194,128)));
}
void ShadowClipItem::forceUpdate()
{
    prepareGeometryChange();
}
QRectF ShadowClipItem::boundingRect() const
{
    return m_drawRect;
}
