//
// Created by 58226 on 2023/2/6.
//

#include "Tick/TickItem.h"
#include <QPainter>
#include "timelinewidget.h"
#include "TimelineDefination.h"
///
#define TimelineInstance()  (GET_POINTER<TimelineWidget>())


static QRectF GetCenteredValueOf(QRectF sourceRect, QRectF containedRect)
{
    if (!sourceRect.contains(containedRect))
        return {0, 0, 0, 0};
    auto Left = (sourceRect.width() - containedRect.width()) / 2 + sourceRect.left();
    auto Top = (sourceRect.height() - containedRect.height()) / 2 + sourceRect.top();
    return {Left, Top, static_cast<qreal>(containedRect.width()), static_cast<qreal>(containedRect.height())};
}

void TickItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QPen(DRAW_COLOR));
    auto curFont = painter->font();
    curFont.setPixelSize(SCALE_CODE_FONT_SIZE);
    painter->setFont(curFont);

    auto curFrameTick = TimelineInstance()->frameTick();
    m_str = FrameToScaleCode(curFrameTick, FRAME_PER_SECOND);
    auto StrLength = m_str.length() * SCALE_CODE_FONT_SIZE;
    auto resultRect = GetCenteredValueOf(
        QRectF(0, 0, scene()->width(), scene()->height()),
        QRectF(0, 0, StrLength, SCALE_CODE_FONT_SIZE));
    painter->drawText(resultRect, m_str);
//    auto regionRect = TimelineInstance()->getArea(TimelineWidget::Area::LeftTop);
//    painter->setPen(QPen(DRAW_COLOR));
//    painter->setBrush(QBrush(DRAW_COLOR));
    //painter->drawLine(QPoint(0, regionRect.height()), QPoint(regionRect.width(), regionRect.height()));
}
QRectF TickItem::boundingRect() const
{
    return GetCenteredValueOf(QRectF(0, 0, scene()->width(), scene()->height()),
                              QRectF(0, 0, m_str.length() * SCALE_CODE_FONT_SIZE, SCALE_CODE_FONT_SIZE));
}
