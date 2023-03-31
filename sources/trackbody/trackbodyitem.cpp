//
// Created by 58226 on 2023/2/7.
//

#include "trackBody/trackbodyitem.h"
#include "timelinedefination.h"
#include <QStyleOptionGraphicsItem>
#include "timelinewidget.h"
#define  TimelineInstance() (GET_POINTER<TimelineWidget>())
TrackBodyItem::TrackBodyItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{

}
TrackBodyItem::TrackBodyItem(const TrackMime &curData, QGraphicsItem *parent)
    :
    QGraphicsItem(parent)
{
    m_mimeKey = curData.id;
}

TrackBodyItem::~TrackBodyItem()
{
    qDebug()<<"delete trackBody";
}

TrackMime TrackBodyItem::getMimeData() const
{
    if(!TimelineInstance())
        return {};
    TrackMime data;
    TimelineInstance()->getTrackData(data, m_mimeKey);
    return data;
}
void TrackBodyItem::forceUpdate()
{
    prepareGeometryChange();
    update();
}
QRectF TrackBodyItem::boundingRect() const
{
    if(!TimelineInstance())
        return {};
    auto trackBodyHeight = TimelineInstance()->getTrackCount() * TRACK_HEIGHT;
    auto mime = getMimeData();
    auto curArea = TimelineInstance()->getArea(TimelineWidget::RightBottom);
    auto yIndex = (double)mime.index * TRACK_HEIGHT;
    if (trackBodyHeight < curArea.height())//y center alignment
    {
        yIndex += (curArea.height() - trackBodyHeight) / 2.0;
    }
    return {0, static_cast<qreal>(yIndex),
            static_cast<qreal>((TimelineInstance()->maxDuration() / (qreal)TimelineInstance()->frameTick()) * MIN_TICK_WIDTH),
            TRACK_HEIGHT};
}
void TrackBodyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(!TimelineInstance())
        return;
    auto countOfTrack = TimelineInstance()->getTrackCount();
    auto trackBodyHeight = countOfTrack * TRACK_HEIGHT;
    auto mime = getMimeData();
    auto curArea = TimelineInstance()->getArea(TimelineWidget::RightBottom);
    double yIndex = mime.index * TRACK_HEIGHT;
    auto maxWidth = (TimelineInstance()->maxDuration() / TimelineInstance()->frameTick()) * MIN_TICK_WIDTH;
    if (trackBodyHeight < curArea.height())//y center alignment
    {
        yIndex += (curArea.height() - trackBodyHeight) / 2.0;
    }
    painter->setPen(QPen(BLACK_COLOR));
    painter->setBrush(QBrush((mime.index % 2 == 1) ? BACK_LIGHT_COLOR : BACK_DEEP_COLOR));
    painter->drawRect(curArea.left(), yIndex, maxWidth, TRACK_HEIGHT);
}