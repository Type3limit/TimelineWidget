//
// Created by 58226 on 2023/2/7.
//

#include "TrackBody/trackbodyitem.h"
#include "timelinedefination.h"
#include <QStyleOptionGraphicsItem>
#include "timelinewidget.h"
#define  TimelineInstance() (GET_POINTER<timelinewidget>())
trackbodyitem::trackbodyitem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{

}
trackbodyitem::trackbodyitem(const trackmime &curData, QGraphicsItem *parent)
    :
    QGraphicsItem(parent)
{
    m_mimeKey = curData.id;
}

trackbodyitem::~trackbodyitem()
{
    extensionMethods::sourcesExtension<QString>::eachBy(m_clips.keys(), [&](const QString &key) -> void
    {
        auto curItem = m_clips[key];
        SAFE_DELETE(curItem);
    });
    std::for_each(m_removeLists.begin(), m_removeLists.end(),[&](clipitem* curItem)->void
    {
        SAFE_DELETE(curItem);
    });

    m_clips.clear();
}

trackmime trackbodyitem::getMimeData() const
{
    trackmime data;
    TimelineInstance()->getTrackData(data, m_mimeKey);
    return data;
}
void trackbodyitem::forceUpdate()
{
    prepareGeometryChange();
}
QRectF trackbodyitem::boundingRect() const
{
    auto trackBodyHeight = TimelineInstance()->getTrackCount() * TRACK_HEIGHT;
    auto mime = getMimeData();
    auto curArea = TimelineInstance()->getArea(timelinewidget::RightBottom);
    auto yIndex = (double)mime.index * TRACK_HEIGHT;
    if (trackBodyHeight < curArea.height())//y center alignment
    {
        yIndex += (curArea.height() - trackBodyHeight) / 2.0;
    }
    return {0, static_cast<qreal>(yIndex),
            static_cast<qreal>((TimelineInstance()->maxDuration() / TimelineInstance()->frameTick()) * MIN_TICK_WIDTH),
            TRACK_HEIGHT};
}
void trackbodyitem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    auto countOfTrack = TimelineInstance()->getTrackCount();
    auto trackBodyHeight = countOfTrack * TRACK_HEIGHT;
    auto mime = getMimeData();
    auto curArea = TimelineInstance()->getArea(timelinewidget::RightBottom);
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
void trackbodyitem::addClipItem(const QString &itemKey)
{
    if (itemKey.isEmpty())
        return;
    auto curItem = new clipitem(itemKey);
    curItem->insertToTrack(m_mimeKey);
    m_clips.insert(itemKey, curItem);
    curItem->m_isRemoved = false;
    this->scene()->addItem(curItem);
    TimelineInstance()->updateSelectedSourceCache(itemKey,curItem);
}
void trackbodyitem::removeClipItem(const QString &itemKey)
{
    if (!m_clips.contains(itemKey))
        return;
    auto curItem = m_clips[itemKey];
    curItem->removeFromTrack();
    curItem->m_isRemoved = true;
    m_clips.remove(itemKey);
    this->scene()->removeItem(curItem);
    if(m_removeLists.count()>=2)
        //不立即删除，信号发送与接受执行不是同步完成的
        // 后续部分可能有用到被删除的ClipItem指针
        // 这里最多保留一个ClipItem指针。
    {
        while(m_removeLists.count()<2)
        {
            SAFE_DELETE(m_removeLists.front());
            m_removeLists.pop_front();
        }
    }
    m_removeLists.push_back(curItem);
}
void trackbodyitem::updateClipItem(const QString &itemKey)
{
    if (!m_clips.contains(itemKey))
        return;
    m_clips[itemKey]->forceUpdate();
}
clipitem* trackbodyitem::getClipItem(const QString &itemKey)
{
    if (!m_clips.contains(itemKey))
        return nullptr;
    return  m_clips[itemKey];
}
