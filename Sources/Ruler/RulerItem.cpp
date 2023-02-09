//
// Created by 58226 on 2023/2/7.
//

#include "Ruler/RulerItem.h"
#include "timelinewidget.h"
#include "TimelineDefination.h"
#include "IntervalWatcher.h"
#include <QStyleOptionGraphicsItem>
#define TimelineInstance() (GET_POINTER<TimelineWidget>())
//IntervalWatcher iw ;
void RulerItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //iw.start();
    if (!CHECK_POINTER<TimelineWidget>()) {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        Q_UNUSED(painter);
        return;
    }

    painter->setPen(QPen(DRAW_COLOR,1));

    painter->drawLines(m_lines);
    int drawCount = m_DrawEnd - m_DrawStart;
    for(int i =0;i<m_pointText.count();i++)
    {
           painter->drawText(QPoint(i*10 * MIN_TICK_WIDTH, TOP_MARGIN),m_pointText[i]);
    }
    //iw.stop();
    //qDebug()<<iw.milliSecond()<<" ms after ruler repaint";
}
QRectF RulerItem::boundingRect() const
{
    if (!CHECK_POINTER<TimelineWidget>()) {
        return {0, 0, 0, 0};
    }
    auto maxDuration = TimelineInstance()->maxDuration();
    auto frameTick = TimelineInstance()->frameTick();
    auto regionRect = TimelineInstance()->getArea(TimelineWidget::Area::RightTop);
    auto drawingWidth = (int)(maxDuration / frameTick) * MIN_TICK_WIDTH;
    return {0, 0, (double)drawingWidth, regionRect.height()};
}
void RulerItem::OnLenthChange()
{
    m_lines.clear();
    m_pointText.clear();
    auto maxDuration = TimelineInstance()->maxDuration();
    auto frameTick = TimelineInstance()->frameTick();
    auto regionRect = TimelineInstance()->getArea(TimelineWidget::Area::RightTop);
    auto maxDrawingHeight = (int)(regionRect.height() * (PERCENT_OF_TICK / 100.0));
    auto drawingCount = (int)(maxDuration / frameTick);
    for (int i = 0; i < drawingCount; i++) {
        int iHeight = i % 10 ? (maxDrawingHeight / 2) : (maxDrawingHeight);//half of max
        m_lines.push_back(QLine(i * MIN_TICK_WIDTH,
                                (int)(regionRect.height()+WIDGET_MARGIN-3) - iHeight,
                                i * MIN_TICK_WIDTH,
                                (int)(regionRect.height()+WIDGET_MARGIN-3)));
        if(iHeight==maxDrawingHeight)
        {
            m_pointText.push_back(FrameToTimeCode((int)(i * frameTick), FRAME_PER_SECOND));
        }
    }
}
