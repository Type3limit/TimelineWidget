//
// Created by 58226 on 2023/2/6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_TimelineWidget.h" resolved

#include "timelinewidget.h"
#include "../forms/ui_timelinewidget.h"
#include "extensionmethods.h"
#include "timelinedefination.h"
#include "intervalwatcher.h"

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
    auto actualHeight = m_timelineData.tracks.size() * TRACK_HEIGHT;
    actualHeight = actualHeight < ui->RightBottomRegion->height() ? ui->RightBottomRegion->height() - WIDGET_MARGIN * 2
                                                                  : actualHeight;
    m_trackHeadView->setDrawingAreaSize(ui->LeftBottomRegion->width() - WIDGET_MARGIN,
                                        (int)actualHeight);
    //轨道体切片区
    m_trackBodyView = new TrackBodyDrawingView(this);
    ui->RightBottomRegion->layout()->addWidget(m_trackBodyView);
    m_trackBodyView->setDrawingAreaSize(ui->RightBottomRegion->width() - WIDGET_MARGIN,
                                        (int)actualHeight);
    //标尺和轨道体的滚动信号链接
    connect(m_trackBodyView, &TrackBodyDrawingView::scrollChanged,
            m_rulerView, &RulerDrawingView::onTrackBodyScroll);

    connect(m_trackBodyView, &TrackBodyDrawingView::vScollChanged,
            m_trackHeadView, &TrackHeadDrawingView::onTrackBodyScroll);

    //标尺移动到边缘位置自动滚动
    connect(m_rulerView, &RulerDrawingView::onAnchorHeadReachEdge, m_trackBodyView,
            &TrackBodyDrawingView::scrollToPos);
}

void TimelineWidget::wheelEvent(QWheelEvent *event)
{
    auto curTick = event->angleDelta().y() < 0 ? (m_timelineData.m_ulFrameTick <= WHEEL_SCALE_DELTA ? 1 :
                                                  m_timelineData.m_ulFrameTick - WHEEL_SCALE_DELTA) : (
                       m_timelineData.m_ulFrameTick + WHEEL_SCALE_DELTA);
    setFrameTick(curTick > maxFrameTick() ? maxFrameTick() : curTick);

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
        auto actualHeight = m_timelineData.tracks.size() * TRACK_HEIGHT;
        actualHeight =
            actualHeight < ui->RightBottomRegion->height() ? ui->RightBottomRegion->height() - WIDGET_MARGIN * 2
                                                           : actualHeight;
        m_trackHeadView->setDrawingAreaSize(ui->LeftBottomRegion->width() - WIDGET_MARGIN,
                                            (int)actualHeight);
    }
    if (m_trackBodyView) {
        auto actualHeight = m_timelineData.tracks.size() * TRACK_HEIGHT;
        actualHeight =
            actualHeight < ui->RightBottomRegion->height() ? ui->RightBottomRegion->height() - WIDGET_MARGIN * 2
                                                           : actualHeight;
        m_trackBodyView->setDrawingAreaSize((int)widthInUse, (int)actualHeight);
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
    }
    case RightTop: {
        return m_rulerView->getViewPortRect();
    }
    case RightBottom: {
        return m_trackBodyView->getViewPortRect();
    }
    default:
        return {};
    }
}

void TimelineWidget::updateMaxTick()
{
    auto curMax =
        qRound((double)(maxDuration() * MIN_TICK_WIDTH) / getArea(Area::RightTop).width());
    curMax = curMax <= WHEEL_SCALE_DELTA ? 1 : curMax - WHEEL_SCALE_DELTA;

    if (curMax >= 3) {
        curMax = curMax % 2 ? curMax - 1 : curMax - 2;
    }
    //qDebug()<<"with max frame:"<<curMax;
    m_timelineData.m_maxFrameTick = curMax;
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
    //if (m_timelineData.m_ulFrameTick != curData) {
    m_timelineData.m_ulFrameTick = curData;
    auto frameWidth = (qreal)(((qreal)maxDuration()) / frameTick()) * MIN_TICK_WIDTH;
    auto widthInUse = frameWidth;

    if (frameWidth < getArea(Area::RightTop).width()) {
        widthInUse = (ulong)(getArea(Area::RightTop).width());
    }
    if (m_rulerView) {
        m_rulerView->setDrawingAreaSize((int)widthInUse, ui->RightTopRegion->height() - WIDGET_MARGIN * 2);
    }
    if (m_trackBodyView) {
        auto actualHeight = m_timelineData.tracks.size() * TRACK_HEIGHT;
        actualHeight =
            actualHeight < ui->RightBottomRegion->height() ? ui->RightBottomRegion->height() - WIDGET_MARGIN * 2
                                                           : actualHeight;
        m_trackBodyView->setDrawingAreaSize((int)widthInUse, (int)actualHeight);
    }
    if (shouldEmitSignal) {
        emit FrameTickChanged(m_timelineData.m_ulFrameTick);
    }

    //}

}

int TimelineWidget::getTrackCount() const
{
    return (int)m_timelineData.tracks.size();
}

double TimelineWidget::percentPerUnit()
{
    return (maxDuration()) / m_trackBodyView->scene()->width();
}

bool TimelineWidget::getTrackByVerticalPos(double yPos, TrackMime &data)
{
    //yPos += m_trackBodyView->getVScroll();
    auto trackBodyHeight = getTrackCount() * TRACK_HEIGHT;
    auto curArea = getArea(RightBottom);
    int index = -1;
    if (curArea.height() >= trackBodyHeight) {
        index = (int)((yPos - (double)((curArea.height() - trackBodyHeight) / 2.0)) / TRACK_HEIGHT);
        if (index < 0 || index > m_timelineData.tracks.size()) {
            return false;
        }

    }
    else {
        index = (int)((yPos) / (double)TRACK_HEIGHT);
        //qDebug() << "index in" << index;
        if (index < 0 || index > m_timelineData.tracks.size()) {
            return false;
        }
    }
    data = getTrackData(index);
    return !data.isDefaultData();
}

#pragma endregion

#pragma region Track option
bool TimelineWidget::addTrack(const QString &curKey, int trackType, int index = -1)
{

    auto actualIndex = (index < 0 ? getTrackCount() : (index >= getTrackCount() ? getTrackCount() : index));
    TrackMime curData{curKey, actualIndex, static_cast<SpecificType>(trackType)};
    //修改源数据
    auto curTracks = m_timelineData.getTracks();
    for (auto itr: curTracks) {
        if (itr.index >= actualIndex) {
            itr.index++;
            alterTrackData(itr.id, itr);
        }
    }
    bool result;
    auto frameWidth = (m_timelineData.m_ulMaxDuration / m_timelineData.m_ulFrameTick) * MIN_TICK_WIDTH;
    auto widthInUse = frameWidth;
    m_timelineData.addTrack(TrackMime(curKey, actualIndex, static_cast<SpecificType>(trackType)));
    result = (m_trackHeadView->addTrackHead(curData) && m_trackBodyView->addTrackBody(curData));
    if (m_trackHeadView) {
        auto actualHeight = m_timelineData.tracks.size() * TRACK_HEIGHT;
        actualHeight =
            actualHeight < ui->RightBottomRegion->height() ? ui->RightBottomRegion->height() - WIDGET_MARGIN * 2
                                                           : actualHeight;
        m_trackHeadView->setDrawingAreaSize(ui->LeftBottomRegion->width() - WIDGET_MARGIN,
                                            (int)actualHeight);
    }
    if (m_trackBodyView) {
        auto actualHeight = m_timelineData.tracks.size() * TRACK_HEIGHT;
        actualHeight =
            actualHeight < ui->RightBottomRegion->height() ? ui->RightBottomRegion->height() - WIDGET_MARGIN * 2
                                                           : actualHeight;
        m_trackBodyView->setDrawingAreaSize((int)widthInUse, (int)actualHeight);
    }
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
    m_trackHeadView->deleteTrackHead(key);
    m_trackBodyView->deleteTrackBody(key);
}

#pragma  endregion

#pragma region clip option
void TimelineWidget::addClip(const QString &trackKey,
                             const ClipMime &mime,
                             bool shouldEmitSignal,
                             bool shouldUpdateMaxDuration)
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
        //emit TrackClipChanged(trackKey, mime.id, 1);
        m_trackBodyView->clipChanged(trackKey,mime.id,1);
    }
    if (shouldUpdateMaxDuration && mime.endPosition() > (maxDuration() / 1.5)) {
        updateMaxDuration();
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
    else {
        m_timelineData.getTrack(searchTrack, clipKey.trackId);
    }
    if (searchTrack.isDefaultData()) {
        if (!m_timelineData.getTrack(searchTrack, [&](TrackMime curTrack) -> bool
        {
            return curTrack.getClip(clipKey.id).isDefaultData();
        })) {
            qDebug() << "remove clip failed,after search ,can not find clip[" << clipKey.id
                     << "],which with an empty track id";
            return;
        }
    }
    m_timelineData.removeClip(searchTrack.id, clipKey.id);
    if (shouldEmitSignal) {
        //emit TrackClipChanged(searchTrack.id, clipKey.id, -1);
        m_trackBodyView->clipChanged(searchTrack.id,clipKey.id,-1);
    }
}
void TimelineWidget::addClip(int index, ClipMime &mime, bool shouldEmitSignal, bool shouldUpdateMaxDuration)
{
    TrackMime cur;
    auto actualTrackSize = (int)m_timelineData.tracks.size();
    if (index == -1 || index > actualTrackSize) {
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
    if (shouldUpdateMaxDuration && mime.endPosition() > (maxDuration() / 1.5)) {
        updateMaxDuration();
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
        ClipMime origin = mime;
        origin.trackId = curTrack.id;
        removeClip(origin);
        addClip(newTrack.id, mime);
    }
    else {
        m_timelineData.setClip(mime.trackId, mime.id, mime);
        if (shouldEmitSignal) {
            emit ClipUpdated(mime.trackId, mime.id);
        }
    }

}

void TimelineWidget::alterClipData(const QString &key,
                                   const QString &trackKey,
                                   const QList<ClipMime> &mimes,
                                   bool searchWhenTrackKeyEmpty)
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

    auto originClip = curTrack.getClip(key);
    if (originClip.isDefaultData())
        return;
    removeClip(originClip);
    std::for_each(mimes.begin(), mimes.end(), [&](const ClipMime &curClip) -> void
    {
        addClip(curClip.trackId, curClip);
    });

}

void TimelineWidget::setSelectedClip(const QString &clip, bool isCancel)
{
    if (isCancel) {
        m_selectedClips.removeAll(clip);
    }
    else {
        m_selectedClips.clear();
        m_selectedClips.push_back(clip);
        auto curClipMime = m_timelineData.getClip(clip, "");
        auto curClipItem = m_trackBodyView->getClipItem(clip);
        if (curClipItem != nullptr && !m_selectedClipsCache.contains(clip)) {
            m_selectedClipsCache.insert(clip, curClipItem);
        }
    }

}
void TimelineWidget::setSelectedClip(const QList<QString> &clips, bool isCancel)
{
    if (isCancel) {
        std::for_each(clips.begin(), clips.end(), [&](const QString &curClip) -> void
        {
            m_selectedClips.removeAll(curClip);
        });
    }
    else {
        m_selectedClips.clear();
        std::for_each(clips.begin(), clips.end(), [&](const QString &curClip) -> void
        {
            if (!m_selectedClips.contains(curClip)) {
                m_selectedClips.push_back(curClip);
            }
            auto curClipMime = m_timelineData.getClip(curClip, "");
            auto curClipItem = m_trackBodyView->getClipItem(curClip);
            if (curClipItem != nullptr && !m_selectedClipsCache.contains(curClip)) {
                m_selectedClipsCache.insert(curClip, curClipItem);
            }
        });
    }

}
void TimelineWidget::setSelectedClip(const QList<ClipItem*> &clips, bool isCancel)
{
    if (isCancel) {

        m_selectedClips.clear();
        m_trackBodyView->update();
    }
    else {
        m_selectedClips.clear();
        std::for_each(clips.begin(), clips.end(), [&](ClipItem* curClip) -> void
        {
            if (curClip == nullptr)
                return;

            if (!m_selectedClips.contains(curClip->m_mimeKey)) {
                m_selectedClips.push_back(curClip->m_mimeKey);
            }
            if (!m_selectedClipsCache.contains(curClip->m_mimeKey)) {
                m_selectedClipsCache.insert(curClip->m_mimeKey, curClip);
            }
            curClip->prepareGeometryChange();
        });
    }

}

void TimelineWidget::updateSelectedSourceCache(const QString &clipId,ClipItem*clip,bool isRemove)
{
    if (!m_selectedClipsCache.contains(clipId)) {
        if(isRemove)
            return;
        m_selectedClipsCache.insert(clipId, clip);
    }
    else {
        if(isRemove)
        {
            m_selectedClipsCache.remove(clipId);
        }
        else{
            m_selectedClipsCache[clipId] = clip;
        }

    }
}

void TimelineWidget::setClipMovement(int xDiff, int yDiff)
{
    if (m_selectedClips.count() <= 0)
        return;
    bool shouldUpdateMaxDuration = false;
    //pre check;

    //m_timelineData.getClips(m_selectedClips);

    std::for_each(m_selectedClips.begin(), m_selectedClips.end(), [&](const QString &curKey) -> void
    {
        auto curMime = m_timelineData.getClip(curKey, "");
        if (curMime.isDefaultData())
            return;
        long curPos = (long)curMime.startPos + xDiff;
        if (curPos > maxDuration())
            shouldUpdateMaxDuration = true;
        if (curPos < 0) {
            xDiff -= (int)curPos;//let others move with this
            curMime.startPos = 0;
        }
        alterClipData(curMime.id, curMime.trackId, curMime);
    });
    if (shouldUpdateMaxDuration)
        setMaxDuration(maxDuration() * 2);

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

void TimelineWidget::updateMaxDuration()
{
    auto curLastClip = m_timelineData.getLastClip();
    if (curLastClip.isDefaultData())
        return;
    if (curLastClip.endPosition() < (maxDuration() / 1.5))
        return;
    //qDebug()<<"current Last"<<curLastClip.startPos+curLastClip.duration;
    auto curMax = maxDuration();
    while ((curLastClip.startPos + curLastClip.duration) >= (curMax / 1.5)) {
        curMax *= 2;
    }
    //qDebug()<<"set max duration with:"<<curMax;

    setMaxDuration(curMax);
    setFrameTick(frameTick());
}

void TimelineWidget::adaptTimelineLength()
{
    auto curLastClip = m_timelineData.getLastClip();
    if (curLastClip.isDefaultData())
        return;
    auto desirePos = (ulong)((curLastClip.startPos + curLastClip.duration) * 1.5);//   1/3？
    desirePos = desirePos > maxDuration() ? maxDuration() : desirePos;
    auto areaWidth = getArea(RightBottom).width();
    auto curFrameTick = desirePos / (areaWidth / MIN_TICK_WIDTH);
    auto setFrame = (long)(curFrameTick < 1 ? 1 : (curFrameTick > maxFrameTick() ? maxFrameTick() : curFrameTick));
    if (setFrame >= 3) {
        setFrame = setFrame % 2 ? setFrame - 1 : setFrame - 2;//do not max
    }
    //qDebug()<<"with frame tick:"<<setFrame;
    setFrameTick(setFrame);
    update();
    forceUpdate(Area::RightBottom);
}
void TimelineWidget::clipMoved(double x, double y, bool isOver,bool posNotMoved)
{
    if (isOver && m_selectedClips.count() > 1)//multi clips process
    {

        bool hasCollision = false;
        bool atLeastOne = false;
        //bool outOfTrackCount = false;
        ClipMime colliedMime;
        QMap<TrackMime, QList<ClipMime>> movements;
        for (const auto &clipKey: m_selectedClips) {

            if (!m_selectedClipsCache.contains(clipKey)) {
                return;
            }
            TrackMime curTrackData;
            hasCollision = m_selectedClipsCache[clipKey]->preCheckForCollision(colliedMime, curTrackData);
            if (!hasCollision && curTrackData.isDefaultData()) {
                //outOfTrackCount = true;
                for (const auto &st: m_selectedClips) {
                    m_selectedClipsCache[st]->removeShadow();
                }
                return;
            }
            if (hasCollision) {
                atLeastOne = true;
            }
            if (!colliedMime.isDefaultData() && !curTrackData.isDefaultData()) {
                if (!movements.contains(curTrackData)) {
                    movements.insert(curTrackData, QList<ClipMime>());

                }
                movements[curTrackData].push_back(colliedMime);
            }
        }
        if (!atLeastOne) {//if no collision,just move them to the right track.
            for(int i =0;i<m_selectedClips.count();i++)
            {
                if (!m_selectedClipsCache.contains(m_selectedClips[i])) {
                    continue;
                }
                bool isSceneNull=false;
                auto currentMode = m_selectedClipsCache[m_selectedClips[i]]->stopClipDrag(isSceneNull, posNotMoved, false);
                if(!isSceneNull&&currentMode.isDefaultData())
                {
                    for(int j =0;j<m_selectedClips.count();j++)
                    {
                        if (!m_selectedClipsCache.contains(m_selectedClips[j])) {
                            continue;
                        }
                        m_selectedClipsCache[m_selectedClips[j]]->removeShadow();
                    }
                    i=0;
                    qDebug()<<"reset loop";
                    continue;
                }
            }
        }
        else {
            multiClipCollied(movements, false);  
        }
         //removeEmptyTrack();
    }
    else {
        for(int i=0;i<m_selectedClips.count();i++)
        {
            auto clipKey = m_selectedClips[i];
            if (!m_selectedClipsCache.contains(clipKey)) {
                return;
            }
            if (!isOver) {
                m_selectedClipsCache[clipKey]->clipDrag(x, y);
            }
            else {
                bool isSceneNull = false;
                auto curMode = m_selectedClipsCache[clipKey]->stopClipDrag(isSceneNull, posNotMoved, false);
                if(!isSceneNull&&curMode.isDefaultData())
                {
                    i=0;
                    qDebug()<<"restart clips loop";
                    continue;
                }
            }
        }
        updateMaxDuration();
    }
    m_trackBodyView->update(m_trackBodyView->getViewPortRect().toRect());
    m_trackBodyView->removeAllDeletedClip();
    removeEmptyTrack();
}

void TimelineWidget::alterClipMovement(double x, double y)
{
    qDebug()<<"adjust with:"<<x<<","<<y;
    for(const auto& clipKey:m_selectedClips )
    {
        if (!m_selectedClipsCache.contains(clipKey)) {
            continue;
        }
        m_selectedClipsCache[clipKey]->clipDrag(x, y);
    }
}


QList<ClipItem*> TimelineWidget::getAllSelectedClip()
{
    QList<ClipItem*> result;
    std::for_each(m_selectedClips.begin(), m_selectedClips.end(), [&](const QString &clipKey) -> void
    {
        if (m_selectedClipsCache.contains(clipKey)) {
            result.push_back(m_selectedClipsCache[clipKey]);
        }
    });
    return result;
}

void TimelineWidget::multiClipCollied(const QMap<TrackMime, QList<ClipMime>> &movements, bool posNotMoved)
{

    intervalwatcher iw;
    iw.start();
    //get all selected mime data
    auto selectedClips = ExtensionMethods::SourcesExtension<QString>::select<ClipMime>(m_selectedClips,
                                                                                       [&](const QString &clipKey) -> ClipMime
                                                                                       {
                                                                                           return m_timelineData
                                                                                               .getClip(clipKey, "");
                                                                                       });
    //group clips by track key
    auto trackClips = ExtensionMethods::SourcesExtension<ClipMime>::groupBy<QString>(selectedClips,
                                                                                     [&](const ClipMime &curClip) -> QString
                                                                                     {
                                                                                         return curClip.trackId;
                                                                                     });
    auto tracks = movements.keys();
    auto minTrack = std::min_element(tracks.begin(), tracks.end(), [](const TrackMime &l, const TrackMime &r) -> bool
    {
        return l.index < r.index;
    });
    auto maxTrack = std::max_element(tracks.begin(), tracks.end(), [](const TrackMime &l, const TrackMime &r) -> bool
    {
        return l.index < r.index;
    });

    if (minTrack == tracks.end() || maxTrack == tracks.end())
        return;

    QMap<QString, TrackMime> TrackData;
    auto trackClipsKeys = trackClips.keys();
    for (const auto &trackKey: trackClipsKeys) {
        TrackMime curTrackMime;
        if (getTrackData(curTrackMime, trackKey)) {
            TrackData.insert(trackKey, curTrackMime);
        }
    }
    QMap<TrackMime, QString> newTracksRef;
    //get tracks mime data,find maximal index and the minimum
    for (const auto &itr: tracks) {
        auto addIndex = (itr.index < 0) ? 0 : (itr.index);
        auto newTrackId = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
        if (!addTrack(newTrackId, itr.type,
                      addIndex)) {
            qDebug() << "try add new Track failed!";
            auto newTracks = newTracksRef.keys();
            for(int i =0;i<newTracks.count();i++)
            {
                removeTrack(newTracksRef[newTracks[i]]);
            }
            return;
        }
        else {
            newTracksRef.insert(itr, newTrackId);
        }
    }


    QList<QString> trackKeys = trackClips.keys();

    int addCount = TrackData.count();
    for (int i = 0; i < addCount; i++) {
        auto curKey = trackKeys[i];
        auto curMimes = trackClips[curKey];
        TrackMime clipOriginTrack = TrackData[curKey];
        for(int j=0;j<curMimes.count();j++)
        {
            auto curMime = curMimes[j];
            bool isSceneNull = false;
            auto movedMime = m_selectedClipsCache[curMime.id]->stopClipDrag(isSceneNull, posNotMoved, true);
            if(!isSceneNull&& movedMime.isDefaultData())
            {
                j=0;
                qDebug()<<"reset loop with multi clip drag";
                continue;
            }
            auto trackInfo = ExtensionMethods::SourcesExtension<ClipMime>
            ::whichHasValue<TrackMime>(movements, movedMime,
                                       [](const ClipMime &l, const ClipMime &r)
                                       { return l.id == r.id; });
            if (!trackInfo.isDefaultData()) {
                movedMime.trackId = newTracksRef[trackInfo];
                alterClipData(movedMime.id, clipOriginTrack.id, movedMime);
            }
        }
    }
    iw.stop();
    qDebug()<<""<<iw.milliSecond()<<"ms after multi clip drag";
    forceUpdate(RightBottom);

}

#pragma  endregion

#pragma region dataResume

bool TimelineWidget::buildFromJsonFile(const QString &filePath)
{
    return buildFromJson(ExtensionMethods::QStringExtension::readAllText(filePath));
}
bool TimelineWidget::buildFromJson(const QString &data)
{

    if (ExtensionMethods::QStringExtension::isNullOrEmpty(data))
        return false;
    intervalwatcher iw;
    iw.start();
    m_trackBodyView->emptyTracks();
    m_trackHeadView->emptyTracks();
    FROM_JSON(m_timelineData, data.toStdString().c_str());
    std::sort(m_timelineData.tracks.begin(),
              m_timelineData.tracks.end(),
              [&](const TrackMime &left, const TrackMime &right) -> bool
              {
                  return left.index < right.index;
              });
    for (const auto &track: m_timelineData.tracks) {
        if (!(m_trackHeadView->addTrackHead(track) && m_trackBodyView->addTrackBody(track))) {
            qDebug() << "read from json data failed! add track failed!";
            return false;
        }
        for (const auto &clip: track.clips) {
            emit TrackClipChanged(track.id, clip.id, 1);
            updateMaxDuration();
        }

    }

    setMaxDuration(m_timelineData.m_ulMaxDuration);
    setFrameTick(m_timelineData.m_ulFrameTick);
    setCurPos(m_timelineData.m_ulCurPos);
    m_trackBodyView->scrollToCursor();
    iw.stop();
    qDebug() << iw.milliSecond() << "ms after build json data";
    return true;
}
void TimelineWidget::removeEmptyTrack()
{
    auto emptyTracks = ExtensionMethods::SourcesExtension<TrackMime>::where({m_timelineData.tracks.begin(),
                                                                             m_timelineData.tracks.end()},
                                                                            [&](const TrackMime &curTrack) -> bool
                                                                            {
                                                                                return curTrack.clips.empty();
                                                                            });
    for (int i = 0; i < emptyTracks.size(); i++) {
        removeTrack(emptyTracks[i].id);
    }
}


#pragma endregion