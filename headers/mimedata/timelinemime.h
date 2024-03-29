//
// Created by 58226 on 2023/2/10.
//

#ifndef TIMELINEMIME_H
#define TIMELINEMIME_H
#include <QUuid>
#include <utility>
#include "timelinedefination.h"
#include "jsonserialization.h"
#include "trackmime.h"
struct TimelineMime
{
    ///TimelineKey
    QString id;
    ///Tracks,clips contained in track
    std::vector<TrackMime> tracks;
    /// clip groups key record
    std::vector<QString> clipGroupsRecord;//due to clip group may cross at least one track,so record it to timeline
    ///单位尺度的帧数
    volatile uint64_t m_ulFrameTick = 1;
    ///时间线总长度
    volatile uint64_t m_ulMaxDuration = 100;
    ///当前位置
    volatile uint64_t m_ulCurPos = 0;
    ///最大的尺度帧数
    volatile uint64_t m_maxFrameTick = 9;

    TimelineMime() = default;
    TimelineMime(const TimelineMime &other)
    {
        id = other.id;
        tracks = (other.tracks);
        clipGroupsRecord = std::vector<QString>(other.clipGroupsRecord);
        m_ulFrameTick = other.m_ulFrameTick;
        m_ulMaxDuration = other.m_ulMaxDuration;
        m_ulCurPos = other.m_ulCurPos;
        m_maxFrameTick = other.m_maxFrameTick;
    }
    explicit TimelineMime(const QString &ids)
    {
        id = (ids);
    }

#pragma region Operator
    TimelineMime &operator=(const TimelineMime &other)
    {
        id = other.id;
        tracks = std::vector<TrackMime>(other.tracks);
        clipGroupsRecord = std::vector<QString>(other.clipGroupsRecord);
        m_ulFrameTick = other.m_ulFrameTick;
        m_ulMaxDuration = other.m_ulMaxDuration;
        m_ulCurPos = other.m_ulCurPos;
        m_maxFrameTick = other.m_maxFrameTick;
        return *this;
    }
#pragma endregion

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
    void removeTrack(const QString &key)
    {
        std::vector<TrackMime>::iterator itr;
        if (getTrackWithItr(key, itr)) {
            tracks.erase(itr);
        }
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

#pragma  endregion


#pragma region clipFunc
    ///往指定Track中添加一个切片
    void addClip(const QString &trackKey, const ClipMime &mime)
    {
        std::vector<TrackMime>::iterator itr;
        if (!getTrackWithItr(trackKey, itr)) {
            return;
        }
        (*itr).addClip(mime);
    }
    ///从指定Track中删除一个切片
    void removeClip(const QString &trackKey, const QString &clipKey)
    {
        std::vector<TrackMime>::iterator itr;
        if (!getTrackWithItr(trackKey, itr)) {
            return;
        }
        (*itr).removeClip(clipKey);
    }
    ///变更一个指定切片
    void
    setClip(const QString &trackKey, const QString &clipKey, const ClipMime &mime, bool searchWhenTrackKeyFailed = true)
    {
        std::vector<TrackMime>::iterator itr;
        TrackMime curTrack;
        if (!getTrackWithItr(trackKey, itr)) {
            if (!getTrack(curTrack, [&](TrackMime curTrack) -> bool
            {
                return !(curTrack.getClip(clipKey).isDefaultData());
            })) {
                return;
            }
            getTrackWithItr(curTrack.id, itr);
        }
        (*itr).setClip(clipKey, mime);
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
        return clipGroupsRecord.end() == std::remove(clipGroupsRecord.begin(), clipGroupsRecord.end(), groupKey);
    }
    ///获取一个指定的切片信息
    ClipMime getClip(const QString &clipKey, const QString &trackKey)
    {
        QList<TrackMime> trackInSearch;
        ClipMime curClip;
        if (!trackKey.isEmpty()) {
            TrackMime curData;
            if (!getTrack(curData, trackKey)) {
                trackInSearch = getTracks();
            }
            else {
                trackInSearch.push_back(curData);
            }
        }
        else {
            trackInSearch.append(getTracks());//TrackKey is empty , find it in all tracks
        }
        for (auto curTrack: trackInSearch) {
            curClip = curTrack.getClip(clipKey);
            if (!curClip.isDefaultData())
                break;
        }
        return curClip;
    }
    ///获取一组切片
    QList<ClipMime> getClips(const QList<QString> &clipsKey)
    {
        QList<ClipMime> clips;
        for (const auto &itr: clipsKey) {
            auto curClip = getClip(itr, "");
            if (!curClip.isDefaultData()) {
                clips.push_back(curClip);
            }
        }
        return clips;
    }
    ///通过组别拿到切片
    QList<ClipMime> getClipByGroup(const QString &groupKey)
    {
        QList<ClipMime> clips;
        if (std::find(clipGroupsRecord.begin(), clipGroupsRecord.end(), groupKey) == clipGroupsRecord.end())
            return clips;//key is not record;
        for (auto &track: tracks) {
            auto curTracks = track.getClips([&](const ClipMime &curClip) -> bool
                                            {
                                                return curClip.groupId == groupKey;
                                            });
            clips.append(curTracks);
        }
        return clips;
    }
    ///获取在某个范围内的所有切片,可以指定轨道，否则返回所有轨道
    QList<ClipMime> getClipsAtPosRange(ulong startPos, ulong endPos, const QString &specificTrackKey = "")
    {
        QList<ClipMime> result;
        QList<TrackMime> tracksInsearch;
        if (specificTrackKey.isEmpty()) {
            tracksInsearch = {tracks.begin(), tracks.end()};
        }
        else {
            TrackMime curTrackData;
            if (getTrack(curTrackData, specificTrackKey)) {
                tracksInsearch.push_back(curTrackData);
            }
        }

        for (auto &curTrack: tracksInsearch) {
            result.append(curTrack.getClips([&](const ClipMime &curClip) -> bool
                                            {
                                                return curClip.startPos >= startPos && curClip.startPos < endPos;
                                            }));
        }
    }

    ///获取切片里位置最前的一个，可以指定轨道，否则获取所有轨道
    ClipMime getfirstClip(const QString &specificTrackKey = "")
    {
        ClipMime curClip;
        ulong curMinPos = UINT32_MAX;
        QList<TrackMime> tracksInsearch;
        if (specificTrackKey.isEmpty()) {
            tracksInsearch = getTracks();
        }
        else {
            TrackMime curTrack;
            if (getTrack(curTrack, specificTrackKey)) {
                tracksInsearch.push_back(curTrack);
            }
            else {
                tracksInsearch = getTracks();
            }
        }
        std::for_each(tracksInsearch.begin(), tracksInsearch.end(), [&](const TrackMime &curTrack) -> void
        {
            auto curPos = std::min_element(curTrack.clips.begin(), curTrack.clips.end(),
                                           [](const ClipMime &left, const ClipMime &right) -> bool
                                           {
                                               return left.startPos >= right.startPos;
                                           });
            if (curPos->startPos <= curMinPos) {
                curMinPos = curPos->startPos;
                curClip = *curPos;
            }
        });
        return curClip;
    }
    ///获取所有切片里最后一个,可以指定轨道，否则获取所有轨道
    ClipMime getLastClip(const QString &specificTrackKey = "")
    {
        ClipMime curClip;
        ulong curMaxPos = 0;

        QList<TrackMime> tracksInsearch;
        if (specificTrackKey.isEmpty()) {
            tracksInsearch = getTracks();
        }
        else {
            TrackMime curTrack;
            if (getTrack(curTrack, specificTrackKey)) {
                tracksInsearch.push_back(curTrack);
            }
            else {
                tracksInsearch = getTracks();
            }
        }

        std::for_each(tracksInsearch.begin(), tracksInsearch.end(), [&](const TrackMime &curTrack) -> void
        {
            auto curPos = std::max_element(curTrack.clips.begin(), curTrack.clips.end(),
                                           [&](const ClipMime &left, const ClipMime &right) -> bool
                                           {
                                               return left.endPosition() < right.endPosition();
                                           });

            if (curPos == curTrack.clips.end())
                return;
            if ((*curPos).startPos + (*curPos).duration >= curMaxPos) {
                curMaxPos = (*curPos).startPos + (*curPos).duration;
                curClip = *curPos;
            }
        });
        return curClip;
    }

#pragma endregion
};

REFLECTION(TimelineMime, id, tracks, clipGroupsRecord, m_ulFrameTick, m_ulMaxDuration, m_ulCurPos, m_maxFrameTick);
#endif //TIMELINEMIME_H
