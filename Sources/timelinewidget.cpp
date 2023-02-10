//
// Created by 58226 on 2023/2/6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TimelineWidget.h" resolved

#include "timelinewidget.h"
#include "../Forms/ui_TimelineWidget.h"
#include "Tick/TickDrawingView.h"
#include "Ruler/RulerDrawingView.h"
#include "TrackHead/TrackHeadDrawingView.h"
#include "TrackBody/TrackBodyDrawingView.h"
#include "ExtensionMethods.h"
#include "TimelineDefination.h"

TimelineWidget::TimelineWidget(QWidget *parent)
    :
    QWidget(parent), ui(new Ui::TimelineWidget)
{
    ui->setupUi(this);
    SET_POINTER<TimelineWidget>(this);
    initWidgets();
}

TimelineWidget::~TimelineWidget()
{
    UNSET_POINTER<TimelineWidget>();
    delete ui;
    SAFE_DELETE(m_tickView)
    SAFE_DELETE(m_rulerView)
    SAFE_DELETE(m_trackBodyView)
    SAFE_DELETE(m_trackHeadView)
}

void TimelineWidget::initWidgets()
{
    //刻度单位标识区
    m_tickView = new TickDrawingView(this);
    ui->LeftTopRegion->layout()->addWidget(m_tickView);
    m_tickView
        ->setDrawingAreaSize(ui->LeftTopRegion->width() - WIDGET_MARGIN, ui->LeftTopRegion->height() - WIDGET_MARGIN);
    //时间线刻度区
    m_rulerView = new RulerDrawingView(this);
    ui->RightTopRegion->layout()->addWidget(m_rulerView);
    m_rulerView
        ->setDrawingAreaSize(ui->RightTopRegion->width() - WIDGET_MARGIN, ui->RightTopRegion->height() - WIDGET_MARGIN);
    //轨道头功能区
    m_trackHeadView = new TrackHeadDrawingView(this);
    ui->LeftBottomRegion->layout()->addWidget(m_trackHeadView);
    m_trackHeadView->setDrawingAreaSize(ui->LeftBottomRegion->width() - WIDGET_MARGIN,
                                        ui->LeftBottomRegion->height() - WIDGET_MARGIN);
    //轨道体切片区
    m_trackBodyView = new TrackBodyDrawingView(this);
    ui->RightBottomRegion->layout()->addWidget(m_trackBodyView);
    m_trackBodyView->setDrawingAreaSize(ui->RightBottomRegion->width() - WIDGET_MARGIN,
                                        ui->RightBottomRegion->height() - WIDGET_MARGIN);
    //标尺和轨道体的滚动信号链接
    connect(m_trackBodyView, &TrackBodyDrawingView::ScrollChanged,
            m_rulerView, &RulerDrawingView::OnTrackBodyScroll);

    //标尺移动到边缘位置自动滚动
    connect(m_rulerView,&RulerDrawingView::OnAnchorHeadReachEdge,m_trackBodyView,
            &TrackBodyDrawingView::ScrollToPos);
}

void TimelineWidget::wheelEvent(QWheelEvent *event)
{
    auto curTick = event->delta() < 0 ? (m_ulFrameTick <= WHEEL_SCALE_DELTA ? 1 : m_ulFrameTick - WHEEL_SCALE_DELTA) : (
        m_ulFrameTick + WHEEL_SCALE_DELTA);
    setFrameTick(curTick > m_maxFrameTick ? m_maxFrameTick : curTick);
    auto frameWidth = (m_ulMaxDuration / m_ulFrameTick) * MIN_TICK_WIDTH;
    auto widthInUse = frameWidth;

    if (frameWidth < getArea(Area::RightTop).width()) {
        widthInUse = (ulong)(getArea(Area::RightTop).width());
    }
    if (m_rulerView) {
        m_rulerView->setDrawingAreaSize((int)widthInUse, ui->RightTopRegion->height() - WIDGET_MARGIN * 2);
    }
    if (m_trackBodyView) {
        m_trackBodyView->setDrawingAreaSize((int)widthInUse, ui->RightBottomRegion->height() - WIDGET_MARGIN * 2);
    }

    m_trackBodyView->scrollToCursor();
    m_rulerView->rulerUpdate();
    m_tickView->update();

    QWidget::wheelEvent(event);
}

void TimelineWidget::resizeEvent(QResizeEvent *event)
{

    if (m_ulMaxDuration == 0 && m_ulFrameTick == 1)//默认状态时，根据控件宽度设置一个绘制区
    {
        //更新最大长度
        m_ulMaxDuration = event->size().width() / MIN_TICK_WIDTH + 1;
    }
    auto frameWidth = (m_ulMaxDuration / m_ulFrameTick) * MIN_TICK_WIDTH;
    auto widthInUse = frameWidth;
    if (frameWidth < ui->RightBottomRegion->width()) {
        widthInUse = ui->RightBottomRegion->width();
    }

    if (m_tickView) {
        m_tickView->setDrawingAreaSize(ui->LeftTopRegion->width() - WIDGET_MARGIN,
                                       ui->LeftTopRegion->height() - WIDGET_MARGIN);
    }
    if (m_rulerView) {
        m_rulerView->setDrawingAreaSize((int)widthInUse, ui->RightTopRegion->height() - WIDGET_MARGIN * 2);
    }
    if (m_trackHeadView) {
        m_trackHeadView->setDrawingAreaSize(ui->LeftBottomRegion->width() - WIDGET_MARGIN,
                                            ui->LeftBottomRegion->height() - WIDGET_MARGIN);
    }
    if (m_trackBodyView) {
        m_trackBodyView->setDrawingAreaSize((int)widthInUse, ui->RightBottomRegion->height() - WIDGET_MARGIN * 2);
    }
    updateMaxTick();
    m_trackBodyView->scrollToCursor();
    m_rulerView->rulerUpdate();
    QWidget::resizeEvent(event);
}

QRectF TimelineWidget::getArea(TimelineWidget::Area pos) const
{
    QRect curRect;
    switch (pos) {
    case LeftTop: {
        curRect = ui->LeftTopRegion->rect();
        break;
    }
    case LeftBottom: {
        curRect = ui->LeftBottomRegion->rect();
        break;
    }
    case RightTop: {
        curRect = ui->RightTopRegion->rect();
        break;
    }
    case RightBottom: {
        curRect = ui->RightBottomRegion->rect();
        break;
    }
    }
    return {static_cast<qreal>(curRect.left()),
            static_cast<qreal>(curRect.top()),
            static_cast<qreal>(curRect.width() - WIDGET_MARGIN),
            static_cast<qreal>(curRect.height() - WIDGET_MARGIN)};
}

void TimelineWidget::updateMaxTick()
{
    m_maxFrameTick = qRound((double)(m_ulMaxDuration * MIN_TICK_WIDTH) / getArea(Area::RightTop).width());
    m_maxFrameTick = m_maxFrameTick <= WHEEL_SCALE_DELTA ? 1 : m_maxFrameTick - WHEEL_SCALE_DELTA;
}

#pragma region get property
ulong TimelineWidget::frameTick() const
{

    return m_ulFrameTick<1?1:m_ulFrameTick;

}
ulong TimelineWidget::maxDuration() const
{
    return m_ulMaxDuration;
}
void TimelineWidget::setMaxDuration(ulong curData)
{

    m_ulMaxDuration = curData;
    updateMaxTick();

}
ulong TimelineWidget::curPos() const
{

    return m_ulCurPos;

}

void TimelineWidget::setCurPos(ulong pos, bool shouldEmitSignal)
{
    if (m_ulCurPos != pos) {

        m_ulCurPos = pos;
        if (shouldEmitSignal) {
            emit PositionChanged(m_ulCurPos);
        }
    }
}
void TimelineWidget::setFrameTick(ulong curData, bool shouldEmitSignal)
{
    if (m_ulFrameTick != curData) {
        m_ulFrameTick = curData;
        if (shouldEmitSignal) {
            emit FrameTickChanged(m_ulFrameTick);
        }
    }
}

int TimelineWidget::getTrackCount() const
{
    return (int)m_timelineData.tracks.size();
}
#pragma endregion

#pragma region Track option
bool TimelineWidget::addTrack(QUuid key, SpecificType trackType, int index = -1)
{
    auto curKey = key.toString().remove("{").remove("}").remove("-");
    auto actualIndex= (index<0?getTrackCount():(index>=getTrackCount()?getTrackCount():index));
    TrackMime curData {curKey,actualIndex,trackType};
    m_sync.lock();
    //修改源数据
    auto curTracks =  m_timelineData.getTracks();
    for(auto itr : curTracks)
    {
        if(itr.index>=actualIndex)
        {
            itr.index++;
            alterTrackData(itr.id,itr);
        }
    }
    bool result = true;
    m_timelineData.addTrack(TrackMime(curKey,actualIndex,trackType));
    if(!m_trackHeadView->addTrackHead(curData)||!m_trackBodyView->addTrackBody(curData))
    {
        result = false;
    }
    m_sync.unlock();
    return result;
}
void TimelineWidget::alterTrackData(const QString &key, const TrackMime& curData)
{
    m_timelineData.setTrack(key,curData);
    emit TrackUpdated(key);
}
bool TimelineWidget::getTrackData(TrackMime& data,const QString& key)
{
    return m_timelineData.getTrack(data,key);
}
//-1 get last index data
TrackMime TimelineWidget::getTrackData(int index = -1)
{
    TrackMime curData;
    if(index ==-1)
        index = (int)(m_timelineData.tracks.size())-1;
    m_timelineData.getTrack(curData,[=](const TrackMime& data)->bool
    {
       return data.index==index;
    });
    return curData;
}
void TimelineWidget::removeTrack(QString key)
{
    m_timelineData.removeTrack(key);
}

#pragma  endregion

#pragma  region Clip option
void TimelineWidget::addClip(const QString &trackKey, const ClipMime &mime)
{
    TrackMime cur;
    if(!m_timelineData.getTrack(cur,[&](const TrackMime& x)->bool{
        return x.id== trackKey;
    }))
    {
        qDebug()<<"add clip failed , track:["<<trackKey<< "]is not exist";
        return;
    }
    cur.addClip(mime);
    emit TrackClipChanged(trackKey,mime.id,1);
}
void TimelineWidget::removeClip(const ClipMime &clipKey)
{

}
#pragma  endregion
