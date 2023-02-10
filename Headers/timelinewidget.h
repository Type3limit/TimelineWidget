//
// Created by 58226 on 2023/2/6.
//

#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>
#include "Tick/TickDrawingView.h"
#include "Ruler/RulerDrawingView.h"
#include "TrackHead/TrackHeadDrawingView.h"
#include "TrackBody/TrackBodyDrawingView.h"
#include "MimeData/TimelineMime.h"


QT_BEGIN_NAMESPACE
namespace Ui
{
class TimelineWidget;
}
QT_END_NAMESPACE

class TimelineWidget: public QWidget
{
Q_OBJECT
    Q_PROPERTY(ulong FrameTick READ frameTick WRITE setFrameTick)
    Q_PROPERTY(ulong Duration READ maxDuration WRITE setMaxDuration)
    Q_PROPERTY(ulong CurPos READ curPos WRITE setCurPos NOTIFY PositionChanged)
public:
    enum Area
    {
        LeftTop,
        LeftBottom,
        RightTop,
        RightBottom
    };

public:
    explicit TimelineWidget(QWidget *parent = nullptr);
    ~TimelineWidget() override;

private:
    Ui::TimelineWidget *ui;

public:
    ///初始化控件
    void initWidgets();
    ///获取控件具体位置宽高
    QRectF getArea(Area pos) const;


public:
#pragma region DataAccess
    ///获取时间单位尺
    ulong frameTick() const;
    ///设置时间单位尺
    void setFrameTick(ulong curData, bool shouldEmitSignal = true);
    ///获取时间线最大长度（绘制区终点）
    ulong maxDuration() const;
    ///设置时间线最大长度（绘制区终点）
    void setMaxDuration(ulong curData);
    ///获取时间线位置
    ulong curPos() const;
    ///设置时间线当前位置
    void setCurPos(ulong pos, bool shouldEmitSignal = true);
#pragma region Track option
    ///修改指定轨道数据
    void alterTrackData(const QString &key, const TrackMime &curData);
    ///获取轨道数量
    int getTrackCount() const;
    ///获取指定Track
    bool getTrackData(TrackMime &data, const QString &key);
    ///获取指定位置的Track
    TrackMime getTrackData(int index);
    ///添加轨道(-1 to tail)
    bool addTrack(QUuid key, SpecificType trackType, int index);
    ///删除轨道
    void removeTrack(QString key);
#pragma endregion
#pragma region Clip option
    void addClip(const QString& trackKey,const ClipMime& mime);
    void removeClip(const ClipMime& clipData);
#pragma  endregion
#pragma endregion

protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    void updateMaxTick();
private:
    TickDrawingView *m_tickView = nullptr;
    RulerDrawingView *m_rulerView = nullptr;
    TrackHeadDrawingView *m_trackHeadView = nullptr;
    TrackBodyDrawingView *m_trackBodyView = nullptr;
private:
    ///单位尺度的帧数
    volatile ulong m_ulFrameTick = 1;
    ///时间线总长度
    volatile ulong m_ulMaxDuration = 0;
    ///当前位置
    volatile ulong m_ulCurPos = 0;
    ///最大的尺度帧数
    volatile ulong m_maxFrameTick = 9;
private:
    TimelineMime m_timelineData;
    QMutex m_sync;
    QWaitCondition m_cond;
signals:
    ///当时间线当前位置发生改变时发出
    void PositionChanged(ulong pos);
    ///当时间线尺度发生改变时发出
    void FrameTickChanged(ulong change);
    ///当轨道数据发生改变时发出
    void TrackUpdated(const QString &key);
    ///对应轨道的切片数量变更事件,-1删除 1增加
    void TrackClipChanged(const QString & trackKey,const QString& clipKey,int mode);
};


#endif //TIMELINEWIDGET_H
