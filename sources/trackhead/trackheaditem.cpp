//
// Created by 58226 on 2023/2/7.
//

#include <utility>

#include "TrackHead/trackheaditem.h"
#include "timelinewidget.h"
#include <QStyleOptionGraphicsItem>
#define TimelineInstance() (GET_POINTER<TimelineWidget>())
TrackHeadItem::TrackHeadItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
}
TrackHeadItem::TrackHeadItem(const TrackMime &curData, QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    m_mimeKey = curData.id;
}

QRectF TrackHeadItem::boundingRect() const
{
    if(ExtensionMethods::QStringExtension::isNullOrEmpty(m_mimeKey))
        return {0,0,0,0};
    auto trackBodyHeight = TimelineInstance()->getTrackCount()* TRACK_HEIGHT;
    auto mime = getMimeData();
    auto curArea = TimelineInstance()->getArea(TimelineWidget::RightBottom);
    auto yIndex = (double)mime.index * TRACK_HEIGHT;
    if (trackBodyHeight < curArea.height())//y center alignment
    {
        yIndex += (curArea.height() - trackBodyHeight) / 2.0;
    }
    return {0, static_cast<qreal>(yIndex), curArea.width(), TRACK_HEIGHT};
}
void TrackHeadItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    auto trackBodyHeight = TimelineInstance()->getTrackCount()* TRACK_HEIGHT;
    auto mime = getMimeData();
    auto curArea = TimelineInstance()->getArea(TimelineWidget::RightBottom);
    double yIndex = mime.index * TRACK_HEIGHT;
    if (trackBodyHeight < curArea.height())//y center alignment
    {
        yIndex += (curArea.height() - trackBodyHeight) / 2.0;
    }
    painter->setPen(QPen(BLACK_COLOR));
    painter->setBrush(QBrush(BACK_DEEP_COLOR));
    painter->fillRect(QRectF(0, yIndex, curArea.width(), TRACK_HEIGHT),QBrush(BACK_DEEP_COLOR));
    painter->setPen(QPen(Qt::white));
    //painter->drawText(QRectF(0, yIndex, curArea.width(), TRACK_HEIGHT),"预留");
    //TODO:draw other button or what else on track head
}
TrackMime TrackHeadItem::getMimeData() const
{
    TrackMime data;
    TimelineInstance()->getTrackData(data,m_mimeKey);
    return data;
}
void TrackHeadItem::forceUpdate()
{
    prepareGeometryChange();
}


