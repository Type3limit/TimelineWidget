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
#include "IntervalWatcher.h"

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
    connect(m_rulerView, &RulerDrawingView::OnAnchorHeadReachEdge, m_trackBodyView,
            &TrackBodyDrawingView::ScrollToPos);
}

void TimelineWidget::wheelEvent(QWheelEvent *event)
{
    auto curTick = event->angleDelta().y() < 0 ? (m_timelineData.m_ulFrameTick <= WHEEL_SCALE_DELTA ? 1 :
                                                  m_timelineData.m_ulFrameTick - WHEEL_SCALE_DELTA) : (
                       m_timelineData.m_ulFrameTick + WHEEL_SCALE_DELTA);
    setFrameTick(curTick > m_timelineData.m_maxFrameTick ? m_timelineData.m_maxFrameTick : curTick);
    auto frameWidth = (m_timelineData.m_ulMaxDuration / m_timelineData.m_ulFrameTick) * MIN_TICK_WIDTH;
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

    if (m_timelineData.m_ulMaxDuration == 0 && m_timelineData.m_ulFrameTick == 1)//默认状态时，根据控件宽度设置一个绘制区
    {
        //更新最大长度
        m_timelineData.m_ulMaxDuration = event->size().width() / MIN_TICK_WIDTH + 1;
    }
    auto frameWidth = (m_timelineData.m_ulMaxDuration / m_timelineData.m_ulFrameTick) * MIN_TICK_WIDTH;
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

QRectF TimelineWidget::getViewPort(TimelineWidget::Area pos) const
{
    switch (pos) {
    case LeftTop: {
        return m_tickView->getViewPortRect();
    }
    case LeftBottom: {
        return m_trackHeadView->getViewPortRect();
        break;
    }
    case RightTop: {
        return m_rulerView->getViewPortRect();
        break;
    }
    case RightBottom: {
        return m_trackBodyView->getViewPortRect();
        break;
    }
    default:
        return {};
    }
}

void TimelineWidget::updateMaxTick()
{
    m_timelineData.m_maxFrameTick =
        qRound((double)(m_timelineData.m_ulMaxDuration * MIN_TICK_WIDTH) / getArea(Area::RightTop).width());
    m_timelineData.m_maxFrameTick =
        m_timelineData.m_maxFrameTick <= WHEEL_SCALE_DELTA ? 1 : m_timelineData.m_maxFrameTick - WHEEL_SCALE_DELTA;
}

#pragma region get property
ulong TimelineWidget::frameTick() const
{

    return m_timelineData.m_ulFrameTick < 1 ? 1 : m_timelineData.m_ulFrameTick;

}
ulong TimelineWidget::maxDuration() const
{
    return m_timelineData.m_ulMaxDuration;
}
void TimelineWidget::setMaxDuration(ulong curData)
{

    m_timelineData.m_ulMaxDuration = curData;
    updateMaxTick();

}
ulong TimelineWidget::curPos() const
{

    return m_timelineData.m_ulCurPos;

}

void TimelineWidget::setCurPos(ulong pos, bool shouldEmitSignal)
{
    if (m_timelineData.m_ulCurPos != pos) {

        m_timelineData.m_ulCurPos = pos;
        if (shouldEmitSignal) {
            emit PositionChanged(m_timelineData.m_ulCurPos);
        }
    }
}
void TimelineWidget::setFrameTick(ulong curData, bool shouldEmitSignal)
{
    if (m_timelineData.m_ulFrameTick != curData) {
        m_timelineData.m_ulFrameTick = curData;
        if (shouldEmitSignal) {
            emit FrameTickChanged(m_timelineData.m_ulFrameTick);
        }
    }
}

int TimelineWidget::getTrackCount() const
{
    return (int)m_timelineData.tracks.size();
}
#pragma endregion

#pragma region Track option
bool TimelineWidget::addTrack(const QString &curKey, int trackType, int index = -1)
{
    IntervalWatcher iw;
    iw.start();
    auto actualIndex = (index < 0 ? getTrackCount() : (index >= getTrackCount() ? getTrackCount() : index));
    TrackMime curData{curKey, actualIndex, static_cast<SpecificType>(trackType)};
    m_sync.lock();
    //修改源数据
    auto curTracks = m_timelineData.getTracks();
    for (auto itr: curTracks) {
        if (itr.index >= actualIndex) {
            itr.index++;
            alterTrackData(itr.id, itr);
        }
    }
    bool result = true;
    m_timelineData.addTrack(TrackMime(curKey, actualIndex, static_cast<SpecificType>(trackType)));
    result = (m_trackHeadView->addTrackHead(curData) && m_trackBodyView->addTrackBody(curData));
    m_sync.unlock();
    iw.stop();
    qDebug() << iw.milliSecond() << "ms after add track";
    return result;
}
void TimelineWidget::alterTrackData(const QString &key, const TrackMime &curData)
{
    m_timelineData.setTrack(key, curData);
    emit TrackUpdated(key);
}
bool TimelineWidget::getTrackData(TrackMime &data, const QString &key)
{
    return m_timelineData.getTrack(data, key);
}
//-1 get last index data
TrackMime TimelineWidget::getTrackData(int index = -1)
{
    TrackMime curData;
    if (index == -1)
        index = (int)(m_timelineData.tracks.size()) - 1;
    m_timelineData.getTrack(curData, [=](const TrackMime &data) -> bool
    {
        return data.index == index;
    });
    return curData;
}
void TimelineWidget::removeTrack(const QString &key)
{
    m_sync.lock();
    TrackMime curTrack;
    if (!m_timelineData.getTrack(curTrack, key)) {
        qDebug() << "can not find track with key :[" << key << "]";
        return;
    }
    m_timelineData.removeTrack(key);
    //修改源数据
    auto curTracks = m_timelineData.getTracks();
    for (auto itr: curTracks) {
        if (itr.index > curTrack.index) {
            itr.index--;
            alterTrackData(itr.id, itr);
        }
    }

    m_sync.unlock();
}

#pragma  endregion

#pragma  region Clip option
void TimelineWidget::addClip(const QString &trackKey, const ClipMime &mime, bool shouldEmitSignal)
{
    TrackMime cur;
    if (!m_timelineData.getTrack(cur, [&](const TrackMime &x) -> bool
    {
        return x.id == trackKey;
    })) {
        qDebug() << "add clip failed , track:[" << trackKey << "]is not exist";
        return;
    }
    m_timelineData.addClip(cur.id, mime);
    if (shouldEmitSignal) {
        emit TrackClipChanged(trackKey, mime.id, 1);
    }

}
void TimelineWidget::removeClip(const ClipMime &clipKey, bool searchWhenTrackKeyEmpty, bool shouldEmitSignal)
{
    auto curTrackKey = clipKey.trackId;
    TrackMime searchTrack;
    if (curTrackKey.isEmpty()) {
        if (!searchWhenTrackKeyEmpty) {
            qDebug() << "remove clip failed,can not find clip[" << clipKey.id
                     << "],which with an empty track id without search";
            return;
        }
    }
    if (!m_timelineData.getTrack(searchTrack, [&](TrackMime curTrack) -> bool
    {
        return curTrack.getClip(clipKey.id).isDefaultData();
    })) {
        qDebug() << "remove clip failed,after search ,can not find clip[" << clipKey.id
                 << "],which with an empty track id";
        return;
    }
    m_timelineData.removeClip(searchTrack.id, clipKey.id);
    if (shouldEmitSignal) {
        emit TrackClipChanged(searchTrack.id, clipKey.id, -1);
    }
}
void TimelineWidget::addClip(int index, ClipMime &mime, bool shouldEmitSignal)
{
    TrackMime cur;
    auto actualTrackSize = (int)m_timelineData.tracks.size();
    if (index == -1||index>actualTrackSize) {
        index = actualTrackSize;
    }//set to last;
    if (!m_timelineData.getTrack(cur, [&](const TrackMime &x) -> bool
    {
        return x.index == index;
    })) {
        qDebug() << "add clip failed , track at index:[" << index << "]is not exist";
        return;
    }
    mime.trackId = cur.id;//insert index may different with mime data.change with actual track;
    m_timelineData.addClip(cur.id, mime);
    if (shouldEmitSignal) {
        emit TrackClipChanged(cur.id, mime.id, 1);
    }
}
void TimelineWidget::alterClipData(const QString &key,
                                   const QString &trackKey,
                                   const ClipMime &mime,
                                   bool searchWhenTrackKeyEmpty,
                                   bool shouldEmitSignal)
{
    TrackMime curTrack;
    //无法根据Track key找到轨道的情况下，尝试搜索轨道
    if (!m_timelineData.getTrack(curTrack, trackKey)) {
        if (!searchWhenTrackKeyEmpty) {
            return;
        }
        if (!m_timelineData.getTrack(curTrack, [&](TrackMime track) -> bool
        { return !track.getClip(key).isDefaultData(); })) {
            qDebug() << "alter clip with key:[" << key << "]failed,can not find it in any track";
            return;
        }
    }
    bool isTrackChanged = curTrack.id != mime.trackId;//轨道id和mime数据轨道id不一致的情况下，视为clip的轨道发生变更
    if (isTrackChanged) {
        TrackMime newTrack;//找到需要插入的新轨道
        if (!m_timelineData.getTrack(newTrack, mime.trackId)) {
            qDebug() << "change track failed with track id:[" << mime.trackId << "],can not find it";
            return;
        }
        removeClip(curTrack.getClip(key));//原始轨道删除
        addClip(mime.trackId, mime);//新轨道添加
    }
    else {
       m_timelineData.setClip(mime.trackId,mime.id,mime);
    }
    if (shouldEmitSignal) {
        emit ClipUpdated(mime.trackId, mime.id);
    }
}

void TimelineWidget::setSelectedClip(const QString &clip)
{
    m_selectedClips.clear();
    m_selectedClips.push_back(clip);
}
void TimelineWidget::setSelectedClip(const QList<QString> &clips)
{
    m_selectedClips = QList<QString>(clips);
}
bool TimelineWidget::isSelected(const QString &clipKey)
{
    return m_selectedClips.contains(clipKey);
}
TimelineMime TimelineWidget::getTimeMime()
{
    return m_timelineData;
}
ulong TimelineWidget::maxFrameTick() const
{
    return m_timelineData.m_maxFrameTick;
}
void TimelineWidget::forceUpdate(TimelineWidget::Area pos)
{
    switch (pos) {
    case LeftTop: {
        ui->LeftTopRegion->update();
        break;
    }
    case LeftBottom: {
        ui->LeftBottomRegion->update();
        break;
    }
    case RightTop: {
        ui->RightTopRegion->update();
        break;
    }
    case RightBottom: {
        ui->RightBottomRegion->update();
        break;
    }
    }
}

#pragma  endregion
