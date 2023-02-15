//
// Created by 58226 on 2023/2/10.
//

#ifndef TIMELINEMIME_H
#define TIMELINEMIME_H
#include <QUuid>
#include <utility>
#include "TimelineDefination.h"
#include "JsonSerialization.h"
#include "TrackMime.h"
struct TimelineMime
{
    ///TimelineKey
    QString id;
    ///Tracks,clips contained in track
    std::vector<TrackMime> tracks;
    /// clip groups key record
    std::vector<QString> clipGroupsRecord;//due to clip group may cross at least one track,so record it to timeline
    ///单位尺度的帧数
    volatile ulong m_ulFrameTick = 1;
    ///时间线总长度
    volatile ulong m_ulMaxDuration = 0;
    ///当前位置
    volatile ulong m_ulCurPos = 0;
    ///最大的尺度帧数
    volatile ulong m_maxFrameTick = 9;

    TimelineMime() = default;
    TimelineMime(const TimelineMime &other)
    {
        id = other.id;
        tracks = (other.tracks);
    }
    explicit TimelineMime(const QString &ids)
    {
        id = (ids);
    }
#pragma  region track func
private:
    inline bool getTrackWithKey(const QString &key, TrackMime &data)
    {
        auto itr = tracks.end();
        if (!getTrackWithItr(key, itr))
            return false;
        data = *itr;
        return true;
    }
    inline bool getTrackWithItr(const QString key, std::vector<TrackMime>::iterator &itr)
    {
        itr = std::find_if(tracks.begin(), tracks.end(), [&](const TrackMime &itr) -> bool
        {
            return itr.id == key;
        });
        return itr != tracks.end();
    }
public:
    ///获取所有轨道
    QList<TrackMime> getTracks()
    {
        return {tracks.begin(), tracks.end()};
    }
    ///获取指定条件的所有切片
    QList<TrackMime> getTracks(const std::function<bool(TrackMime)> &matchFunction)
    {
        QList<TrackMime> res;
        for (const auto &itr: tracks) {
            if (matchFunction(itr))
                res.push_back(itr);
        }
        return res;
    }
    ///获取指定条件的第一个切片
    bool getTrack(TrackMime &outData, const std::function<bool(TrackMime)> &matchFunction)
    {
        bool res = false;
        for (const auto &itr: tracks) {
            if (matchFunction(itr)) {
                res = true;
                outData = itr;
                break;
            }
        }
        return res;
    }
    ///设置变更所有轨道
    void setTrack(const QList<TrackMime> &curClips)
    {
        tracks = std::vector<TrackMime>(curClips.begin(), curClips.end());
    }
    ///添加轨道
    void addTrack(const TrackMime &clipData)
    {
        tracks.emplace_back(clipData);
    }
    ///删除轨道,返回被删除的轨道
    TrackMime removeTrack(const QString &key)
    {
        std::vector<TrackMime>::iterator itr;
        if (getTrackWithItr(key, itr)) {
            tracks.erase(itr);
            return *itr;
        }
        return {};
    }
    ///拿到指定轨道信息
    bool getTrack(TrackMime &data, const QString &key)
    {
        return getTrackWithKey(key, data);
    }
    ///变更指定Track key的信息
    void setTrack(const QString &key, const TrackMime &data)
    {
        std::vector<TrackMime>::iterator itr;
        if (getTrackWithItr(key, itr))
            *itr = TrackMime(data);
    }
    ///往指定Track中添加一个切片
    void addClip(const QString&trackKey,ClipMime&mime)
    {
        std::vector<TrackMime>::iterator itr;
        if (!getTrackWithItr(trackKey, itr))
        {
            return;
        }
        (*itr).addClip(mime);
    }
    ///从指定Track中删除一个切片
    void removeClip(const QString&trackKey,const QString& clipKey)
    {
        std::vector<TrackMime>::iterator itr;
        if (!getTrackWithItr(trackKey, itr))
        {
            return;
        }
        (*itr).removeClip(clipKey);
    }
    ///添加一个组信息
    void addGroup(const QString &groupKey)
    {
        for (auto &itr: clipGroupsRecord) {
            if (itr == groupKey)//do not add same group key
                return;
        }
        clipGroupsRecord.emplace_back(groupKey);
    }

    ///删除一个组信息
    bool removeGroup(const QString &groupKey)
    {
        return clipGroupsRecord.end()==std::remove(clipGroupsRecord.begin(), clipGroupsRecord.end(),groupKey);
    }

    ///通过组别拿到切片
    QList<ClipMime> getClipByGroup(const QString &groupKey)
    {
        QList<ClipMime> clips;
        if (std::find(clipGroupsRecord.begin(), clipGroupsRecord.end(), groupKey) == clipGroupsRecord.end())
            return clips;//key is not record;
        for (auto track: tracks) {
            auto curTracks = track.getClips([&](const ClipMime &curClip) -> bool{
                                                return curClip.groupId == groupKey;
                                            });
            clips.append(curTracks);
        }
        return clips;
    }

#pragma  endregion
};

REFLECTION(TimelineMime, id, tracks,m_ulFrameTick,m_ulMaxDuration,m_ulCurPos,m_maxFrameTick);
#endif //TIMELINEMIME_H
