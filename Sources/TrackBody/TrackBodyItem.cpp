//
// Created by 58226 on 2023/2/7.
//

#include "TrackBody/TrackBodyItem.h"
#include "TimelineDefination.h"
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
    ExtensionMethods::SourcesExtension<QString>::eachBy(m_clips.keys(), [&](const QString &key) -> void
    {
        auto curItem = m_clips[key];
        SAFE_DELETE(curItem);
    });
    m_clips.clear();
}

TrackMime TrackBodyItem::getMimeData() const
{
    TrackMime data;
    TimelineInstance()->getTrackData(data, m_mimeKey);
    return data;
}
void TrackBodyItem::forceUpdate()
{
    prepareGeometryChange();
}
QRectF TrackBodyItem::boundingRect() const
{
    auto trackBodyHeight = TimelineInstance()->getTrackCount() * TRACK_HEIGHT;
    auto mime = getMimeData();
    auto curArea = TimelineInstance()->getArea(TimelineWidget::RightBottom);
    auto yIndex = (double)mime.index * TRACK_HEIGHT;
    if (trackBodyHeight < curArea.height())//y center alignment
    {
        yIndex += (curArea.height() - trackBodyHeight) / 2.0;
    }
    return {0, static_cast<qreal>(yIndex),
            static_cast<qreal>((TimelineInstance()->maxDuration() / TimelineInstance()->frameTick()) * MIN_TICK_WIDTH),
            TRACK_HEIGHT};
}
void TrackBodyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

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
void TrackBodyItem::addClipItem(const QString &itemKey)
{
    if (itemKey.isEmpty())
        return;
    auto curItem = new ClipItem(itemKey);
    curItem->insertToTrack(m_mimeKey);
    m_clips.insert(itemKey, curItem);
    this->scene()->addItem(curItem);
}
void TrackBodyItem::removeClipItem(const QString &itemKey)
{
    if (!m_clips.contains(itemKey))
        return;
    auto curItem = m_clips[itemKey];
    curItem->removeFromTrack();
    m_clips.remove(itemKey);
    this->scene()->removeItem(curItem);
    delete curItem;
}
void TrackBodyItem::updateClipItem(const QString &itemKey)
{
    if (!m_clips.contains(itemKey))
        return;
    m_clips[itemKey]->update();
}