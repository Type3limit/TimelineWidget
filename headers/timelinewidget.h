//
// Created by 58226 on 2023/2/6.
//

#ifndef TIMELINEWIDGET_H
#define TIMELINEWIDGET_H

#include <QWidget>
#include "Tick/tickdrawingview.h"
#include "Ruler/rulerdrawingview.h"
#include "TrackHead/trackheaddrawingview.h"
#include "TrackBody/trackbodydrawingview.h"
#include "MimeData/timelinemime.h"
#include "MimeData/cliprange.h"

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
    ///获取控件具体位置宽高
    QRectF getArea(Area pos) const;
    ///获取可视区范围矩形
    QRectF getViewPort(Area pos) const;
    ///强制某一个区域更新
    void forceUpdate(Area pos);
    ///获取时间线源数据
    TimelineMime getTimeMime();
    ///根据当前切片调整最大长度（仅在不足时变长，不缩短）
    void updateMaxDuration();
    ///根据切片长度自适应调整
    void adaptTimelineLength();
public:
#pragma region DataAccess
    ///获取时间单位尺
    ulong frameTick() const;
    ///设置时间单位尺
    void setFrameTick(ulong curData, bool shouldEmitSignal = true);
    ///获取最大时间单位尺
    ulong maxFrameTick() const;
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
    ///获取指定位置的Track(逻辑index)
    TrackMime getTrackData(int index);
    ///添加轨道(-1 to tail)
    bool addTrack(const QString &key, int trackType, int index);
    ///删除轨道
    void removeTrack(const QString &key);
    ///删除所有空轨道
    void removeEmptyTrack();
#pragma endregion
#pragma region Clip option
    ///设置被选中的单个切片，如有多个，会替换为当前设置切片
    void setSelectedClip(const QString &clips, bool isCancel = false);
    ///设置多个选中切片，取已有选中项和传入选中项的并集
    void setSelectedClip(const QList<QString> &clips, bool isCancel = false);
    ///设置多个选中切片，直接使用传入项
    void setSelectedClip(const QList<ClipItem*> &clips, bool isCancel = false);
    ///获取所有被选中的切片实际对象
    QList<ClipItem*> getAllSelectedClip();

    ///判断某个切片是否被选中
    bool isSelected(const QString &clipKey);
    ///切片所有被选中的切片
    void setClipMovement(int xDiff, int yDiff);
    ///添加切片到指定轨道
    void addClip(const QString &trackKey, const ClipMime &mime, bool shouldEmitSignal = true,bool shouldUpdateMaxDuration = true);
    ///添加切片到指定轨道
    void addClip(int index, ClipMime &mime, bool shouldEmitSignal = true,bool shouldUpdateMaxDuration = true);
    ///去除特定切片
    void removeClip(const ClipMime &clipData, bool searchWhenTrackKeyEmpty = true, bool shouldEmitSignal = true);
    ///修改切片信息
    void alterClipData(const QString &key,
                       const QString &trackKey,
                       const ClipMime &mime,
                       bool searchWhenTrackKeyEmpty = true,
                       bool shouldEmitSignal = true);
    ///修改切片信息(一个变多个时)
    void alterClipData(const QString &key,
                       const QString &OriginTrackKey,
                       const QList<ClipMime> &mimes,
                       bool searchWhenTrackKeyEmpty = true);
    ///切片建组
    void madeClipGroup(QList<ClipMime> clips);
    ///切片解组
    void rescindClipGroup(const QString &groupKey);

private:
    ///移动选中的切片
    void clipMoved(double x, double y, bool isOver,bool posNotChange);
    ///对切片的移动量进行修正
    void alterClipMovement(double x, double y);
    ///处理多切片选择移动。
    void multiClipCollied(const QMap<TrackMime, QList<ClipMime>> &movement, bool posNotMoved);
#pragma  endregion
#pragma endregion

#pragma region resumeData
public:
    ///由json字符串恢复时间线数据
    bool buildFromJson(const QString &data);
    ///由json文件恢复时间线数据
    bool buildFromJsonFile(const QString &filePath);
#pragma endregion
protected:
    void wheelEvent(QWheelEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:
    ///根据时间线长度调整最大单位刻度
    void updateMaxTick();
    ///初始化控件
    void initWidgets();
    ///单帧宽度
    double percentPerUnit();
    ///通过纵向高度获取具体轨道
    bool getTrackByVerticalPos(double yPos, TrackMime &trackData);
private:
    ///更新选中缓存
    void updateSelectedSourceCache(const QString &clipId, ClipItem*clip,bool isRemove);
private:
    ///左上单位刻度
    TickDrawingView *m_tickView = nullptr;
    ///右上时间刻度
    RulerDrawingView *m_rulerView = nullptr;
    ///轨道头
    TrackHeadDrawingView *m_trackHeadView = nullptr;
    ///轨道体
    TrackBodyDrawingView *m_trackBodyView = nullptr;
private:
    friend class ClipItem;
    friend class TrackBodyItem;
    friend class TrackHeadItem;
    friend class AnchorBodyItem;
    friend class TrackBodyDrawingView;
private:
    ///被选中的所有切片，仅记录key
    QList<QString> m_selectedClips;
    ///被选中切片的缓存
    QMap<QString, ClipItem*> m_selectedClipsCache;
    ///主要的时间线数据
    TimelineMime m_timelineData;
    ///同步锁
    QMutex m_sync;
    QWaitCondition m_cond;
    ///标识是否在移动多个切片时新增了轨道
    volatile bool m_isMultiAddTrack = false;
signals:
    ///当时间线当前位置发生改变时发出
    void PositionChanged(ulong pos);
    ///当时间线尺度发生改变时发出
    void FrameTickChanged(ulong change);
    ///当轨道数据发生改变时发出对应轨道信息
    void TrackUpdated(const QString &key);
    ///对应轨道的切片数量变更事件,-1删除 1增加
    void TrackClipChanged(const QString &trackKey, const QString &clipKey, int mode);
    ///切片数据发生改变时发出
    void ClipUpdated(const QString &trackKey, const QString &clipKey);
    ///切片成/解组时发出,建议由clip订阅
    void ClipToGroup(bool isGrouped, const QString &groupKey, const QString &clipKey);

};


#endif //TIMELINEWIDGET_H
