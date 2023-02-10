//
// Created by 58226 on 2023/2/9.
//

#include "Clip/ClipItem.h"
#include "TimelineDefination.h"
#include "timelinewidget.h"

#define timeline() (GET_POINTER<TimelineWidget>())
ClipItem::ClipItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{


}
ClipItem::ClipItem(const QString& key, QGraphicsItem *parent)
{
    m_mimeKey = key;
    setFlags(
        QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setCacheMode(QGraphicsItem::CacheMode::DeviceCoordinateCache);
}


void ClipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    auto countOfTrack = timeline()->getTrackCount();
    auto trackBodyHeight = countOfTrack * TRACK_HEIGHT;
    auto trackData = getTrackData();
    auto curData =trackData.getClip(m_mimeKey);
    auto xIndex = ((double)(curData.startPos)/timeline()->frameTick())*MIN_TICK_WIDTH;
    auto width = ((double)(curData.startPos)/timeline()->frameTick())*MIN_TICK_WIDTH;
    auto curArea = timeline()->getArea(TimelineWidget::RightBottom);
    double yIndex = trackData.index * TRACK_HEIGHT;
    if (trackBodyHeight < curArea.height())//y center alignment
    {
        yIndex += (curArea.height() - trackBodyHeight) / 2.0;
    }
    QColor paintColor = Qt::red;
    switch (curData.type) {
    case SpecificType::Subtitle:
        paintColor = SUBTITLE_CLIP_COLOR;
        break;
    case SpecificType::Effect:
        paintColor = EFFECT_CLIP_COLOR;
        break;
    case SpecificType::Sticker:
        paintColor=STICKER_CLIP_COLOR;
        break;
    default:break;
    }
    painter->setPen(QPen(paintColor));
    painter->setBrush(QBrush(paintColor));
    painter->drawRect(xIndex,yIndex,width,TRACK_HEIGHT);
    //TODO:draw extra info of clip,maybe we should change the mime data struct
}
QRectF ClipItem::boundingRect() const
{
    auto countOfTrack = timeline()->getTrackCount();
    auto trackBodyHeight = countOfTrack * TRACK_HEIGHT;
    auto trackData = getTrackData();
    auto curData =trackData.getClip(m_mimeKey);
    auto xIndex = ((double)(curData.startPos)/timeline()->frameTick())*MIN_TICK_WIDTH;
    auto width = ((double)(curData.startPos)/timeline()->frameTick())*MIN_TICK_WIDTH;
    auto curArea = timeline()->getArea(TimelineWidget::RightBottom);
    double yIndex = trackData.index * TRACK_HEIGHT;
    if (trackBodyHeight < curArea.height())//y center alignment
    {
        yIndex += (curArea.height() - trackBodyHeight) / 2.0;
    }
    return QRectF(xIndex,yIndex,width,TRACK_HEIGHT);
}
bool ClipItem::insertToTrack(const QString &trackKey)
{
    if(!m_trackMimeKey.isEmpty())
        return false;
    m_trackMimeKey = trackKey;
    return true;
}
bool ClipItem::removeFromTrack()
{
    if(m_trackMimeKey.isEmpty())
    {
        return false;
    }
    m_trackMimeKey="";
    return true;
}
ClipMime ClipItem::getMimeData(bool searchWhenTrackKeyEmpty ) const
{
     return getTrackData(searchWhenTrackKeyEmpty).getClip(m_mimeKey);
}
TrackMime ClipItem::getTrackData(bool searchWhenTrackKeyEmpty) const
{
    if(m_trackMimeKey.isEmpty())
    {
        if(!searchWhenTrackKeyEmpty)
            return {};
        auto TrackCount = timeline()->getTrackCount();
        bool isFind = false;
        TrackMime curData;
        for(int i = 0;i<TrackCount;i++)
        {
            curData = timeline()->getTrackData(i);
            if(!curData.getClip(m_mimeKey).isDefaultData())
            {
                isFind = true;
                break;
            }
        }
        return curData;
    }
    else{
        TrackMime curTrackData;
        if(!timeline()->getTrackData(curTrackData,m_trackMimeKey))
            return {};
        return curTrackData;
    }

}
