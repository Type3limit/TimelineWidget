//
// Created by 58226 on 2023/2/7.
//

#include "Ruler/ruleritem.h"
#include "timelinewidget.h"
#include "timelinedefination.h"
#include "intervalwatcher.h"
#include <QStyleOptionGraphicsItem>
#define TimelineInstance() (GET_POINTER<TimelineWidget>())
//intervalwatcher iw ;
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
    int startPos = (int)(m_updateRect.x()/(10 * MIN_TICK_WIDTH))-1;
    startPos = startPos<0?0:startPos;
    int endPos = (int)((m_updateRect.x()+m_updateRect.width())/(10 * MIN_TICK_WIDTH))+1;
    endPos = endPos>=m_pointText.length()?m_pointText.length()-1:endPos;

    for(int i=startPos;i<=endPos;i++)
    {
           painter->drawText(QPoint(i*10 * MIN_TICK_WIDTH, TOP_MARGIN),m_pointText[i]);
    }
    //iw.stop();
    //qDebug()<<iw.milliSecond()<<" ms after ruler repaint";
}
QRectF RulerItem::boundingRect() const
{
    return m_updateRect;
}
void RulerItem::OnLengthChange()
{
    m_lines.clear();
    m_pointText.clear();
    auto maxDuration = TimelineInstance()->maxDuration();
    auto frameTick = TimelineInstance()->frameTick();
    auto regionRect = TimelineInstance()->getArea(TimelineWidget::Area::RightTop);
    auto maxDrawingHeight = (int)(regionRect.height() * (PERCENT_OF_TICK / 100.0));
    auto drawingCount = (int)(maxDuration / frameTick);
    auto drawingWidth = (int)(maxDuration / frameTick) * MIN_TICK_WIDTH;
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
    prepareGeometryChange();
}
RulerItem::RulerItem(QGraphicsObject *parent): QGraphicsItem(parent)
{
    //setCacheMode(CacheMode::DeviceCoordinateCache);
}
void RulerItem::setUpdateRect(const QRectF &dest)
{
    m_updateRect = dest;
    prepareGeometryChange();
    //qDebug()<<m_updateRect;
}
