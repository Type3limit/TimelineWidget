//
// Created by 58226 on 2023/2/9.
//

#ifndef TIMELINEDATAMANAGER_H
#define TIMELINEDATAMANAGER_H


#include <QMutex>
#include <QWaitCondition>
#include "MimeData/TrackMime.h"
#include <QRectF>
class TimelineDataManager
{

private:
    volatile QRectF m_leftTopRect;
    volatile QRectF m_leftBottomRect;
    volatile QRectF m_rightBottomRect;
    volatile QRectF m_rightTopRect;
    ///单位尺度的帧数
    volatile ulong m_ulFrameTick = 1;
    ///时间线总长度
    volatile ulong m_ulMaxDuration = 0;
    ///当前位置
    volatile ulong m_ulCurPos = 0;
    ///最大的尺度帧数
    volatile ulong m_maxFrameTick = 9;
    QMap<QString,TrackMime> m_tracks;
    QMutex m_sync;
    QWaitCondition m_cond;
};


#endif //TIMELINEDATAMANAGER_H
