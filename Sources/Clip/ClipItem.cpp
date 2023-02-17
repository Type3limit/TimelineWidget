//
// Created by 58226 on 2023/2/9.
//

#include "Clip/ClipItem.h"
#include "TimelineDefination.h"
#include "timelinewidget.h"
#include "IntervalWatcher.h"
#include <QGraphicsSceneMouseEvent>
#include <QApplication>
#define timeline() (GET_POINTER<TimelineWidget>())
ClipItem::ClipItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{

    Init();
}

ClipItem::ClipItem(const QString& key, QGraphicsItem *parent)
{
    m_mimeKey = key;
    Init();
}

void ClipItem::Init()
{
    setFlags(ItemIsSelectable | ItemSendsGeometryChanges);
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::LeftButton);
    //setCacheMode(DeviceCoordinateCache)
    m_shadow = new ShadowClipItem(m_mimeKey);
    m_image.load("./test.png");
}
void ClipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //IntervalWatcher iw;
    //iw.start();
    auto countOfTrack = timeline()->getTrackCount();
    auto trackBodyHeight = countOfTrack * TRACK_HEIGHT;
    auto trackData = getTrackData();
    auto curData =trackData.getClip(m_mimeKey);
    auto xIndex = ((double)(curData.startPos)/timeline()->frameTick())*MIN_TICK_WIDTH;
    auto width = ((double)(curData.duration)/timeline()->frameTick())*MIN_TICK_WIDTH;
    auto curArea = timeline()->getArea(TimelineWidget::RightBottom);
    double yIndex = trackData.index * TRACK_HEIGHT;
    bool shouldDrawText = false;
    bool shouldDrawPic = false;
    if (trackBodyHeight < curArea.height())//y center alignment
    {
        yIndex += (curArea.height() - trackBodyHeight) / 2.0;
    }
    QColor paintColor = Qt::red;
    switch (curData.type) {
    case SpecificType::Subtitle:
        shouldDrawText = true;
        paintColor = SUBTITLE_CLIP_COLOR;
        break;
    case SpecificType::Effect:
        paintColor = EFFECT_CLIP_COLOR;
        shouldDrawPic = true;
        shouldDrawText = true;
        break;
    case SpecificType::Sticker:
        shouldDrawPic = true;
        paintColor=STICKER_CLIP_COLOR;
        break;
    default:break;
    }
    painter->setPen(QPen(Qt::white));
    painter->setBrush(QBrush(paintColor));
    painter->fillRect(xIndex,yIndex,width,TRACK_HEIGHT,QBrush(paintColor));
    auto limitedArea = timeline()->getViewPort(TimelineWidget::RightBottom);
    if(m_isOnHover||timeline()->isSelected(m_mimeKey))
    {
        painter->drawRect(xIndex,yIndex,width,TRACK_HEIGHT);
    }
    if(m_isDragMoved)
    {
        painter->fillRect(m_shadowRect,QBrush(paintColor));
    }
    //TODO:draw extra info of clip,maybe we should change the mime data struct
    int startX= xIndex<limitedArea.x()?limitedArea.x():xIndex;
    int endX = (xIndex+width)>limitedArea.right()?limitedArea.right():(xIndex+width);

        painter->setPen(QPen(Qt::white));
        for(int curX = startX;curX<=xIndex+width;curX+=20)
        {
            if((xIndex+width)-curX < 20)
                return;
            if(shouldDrawText)
            {
               painter->drawText(curX,(int)yIndex,20,TRACK_HEIGHT,Qt::AlignHCenter | Qt::AlignVCenter,"测试");
            }
            if(shouldDrawPic)
            {
                painter->drawPixmap(QRect(curX,yIndex,20,20),m_image);
            }
        }
        //iw.stop();
        //qDebug()<<iw.milliSecond()<<" ms to finish clip draw";
}
QRectF ClipItem::boundingRect() const
{
    auto trackBodyHeight = timeline()->getTrackCount() * TRACK_HEIGHT;
    auto trackData = getTrackData();
    auto curData =trackData.getClip(m_mimeKey);
    auto xIndex = ((double)(curData.startPos)/timeline()->frameTick())*MIN_TICK_WIDTH;
    auto width = ((double)(curData.duration)/timeline()->frameTick())*MIN_TICK_WIDTH;
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
    //qDebug()<<"insert clip :"<<m_mimeKey<<"to track:"<<m_trackMimeKey;
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
        TrackMime curData;
        for(int i = 0;i<TrackCount;i++)
        {
            curData = timeline()->getTrackData(i);
            if(!curData.getClip(m_mimeKey).isDefaultData())
            {
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
QVariant ClipItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    return QGraphicsItem::itemChange(change, value);
}
void ClipItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::MouseButton::LeftButton)
    {
        if(QApplication::keyboardModifiers().testFlag(Qt::ControlModifier))
        {
           timeline()->setSelectedClip(QList<QString>{m_mimeKey},timeline()->isSelected(m_mimeKey));
        }
        else
        {
            timeline()->setSelectedClip(m_mimeKey,timeline()->isSelected(m_mimeKey));
            if(!m_isDragMoved)
            {
                m_originRect = m_shadowRect = boundingRect();
                m_isDragMoved = true;
                event->accept();
                m_prePoint = event->scenePos();
                scene()->addItem(m_shadow);
            }
        }
    }
    else{
        QGraphicsItem::mousePressEvent(event);
    }

}
void ClipItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_isDragMoved)
    {
        m_isDragMoved = false;
        event->accept();


        scene()->removeItem(m_shadow);

        auto xDelta = m_shadowRect.x()-m_originRect.x();
        auto curMime = getMimeData();
        if(curMime.isDefaultData())
            return;
        auto framePerUnit = (timeline()->maxDuration()) /scene()->width();
        auto curFrameMoved =(long) ceil(xDelta * framePerUnit);
        long curX = (long)curMime.startPos;
        curMime.startPos = curX+curFrameMoved<0?0:(curX+curFrameMoved>timeline()->maxDuration()?timeline()->maxDuration():curX+curFrameMoved);
        timeline()->alterClipData(curMime.id,m_trackMimeKey,curMime);
        //TODO:deal with y to change track
        scene()->update(timeline()->getViewPort(TimelineWidget::RightBottom));
    }
    else{
        QGraphicsItem::mouseReleaseEvent(event);
    }

}
void ClipItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_isDragMoved)
    {
        auto xDelta =event->scenePos().x()-m_prePoint.x();
        auto yDelta = event->scenePos().y()-m_prePoint.y();
        m_prePoint = event->scenePos();
        //TODO: add left and right drag area to expand clip length;
        m_shadowRect = QRectF(m_shadowRect.x()+xDelta,m_shadowRect.y()+yDelta,m_shadowRect.width(),m_shadowRect.height());
        if(m_shadow)
        {
            m_shadow->setDrawRect(m_shadowRect);
            m_shadow->forceUpdate();
        }
    }
    else{
        QGraphicsItem::mouseMoveEvent(event);
    }

}
void ClipItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    m_isOnHover = true;
    event->ignore();
    QGraphicsItem::hoverEnterEvent(event);
}
void ClipItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    m_isOnHover = false;
    QGraphicsItem::hoverLeaveEvent(event);
}
ClipItem::~ClipItem()
{
    SAFE_DELETE(m_shadow);
}
