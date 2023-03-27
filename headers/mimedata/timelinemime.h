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
struct timelinemime
{
    ///TimelineKey
    QString id;
    ///Tracks,clips contained in track
    std::vector<trackmime> tracks;
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

    timelinemime() = default;
    timelinemime(const timelinemime &other)
    {
        id = other.id;
        tracks = (other.tracks);
        clipGroupsRecord = std::vector<QString>(other.clipGroupsRecord);
        m_ulFrameTick = other.m_ulFrameTick;
        m_ulMaxDuration = other.m_ulMaxDuration;
        m_ulCurPos = other.m_ulCurPos;
        m_maxFrameTick = other.m_maxFrameTick;
    }
    explicit timelinemime(const QString &ids)
    {
        id = (ids);
    }

#pragma region Operator
    timelinemime &operator=(const timelinemime &other)
    {
        id = other.id;
        tracks = std::vector<trackmime>(other.tracks);
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
    inline bool getTrackWithKey(const QString &key, trackmime &data)
    {
        auto itr = tracks.end();
        if (!getTrackWithItr(key, itr))
            return false;
        data = *itr;
        return true;
    }
    inline bool getTrackWithItr(const QString key, std::vector<trackmime>::iterator &itr)
    {
        itr = std::find_if(tracks.begin(), tracks.end(), [&](const trackmime &itr) -> bool
        {
            return itr.id == key;
        });
        return itr != tracks.end();
    }
public:
    ///获取所有轨道
    QList<trackmime> getTracks()
    {
        return {tracks.begin(), tracks.end()};
    }
    ///获取指定条件的所有切片
    QList<trackmime> getTracks(const std::function<bool(trackmime)> &matchFunction)
    {
        QList<trackmime> res;
        for (const auto &itr: tracks) {
            if (matchFunction(itr))
                res.push_back(itr);
        }
        return res;
    }
    ///获取指定条件的第一个切片
    bool getTrack(trackmime &outData, const std::function<bool(trackmime)> &matchFunction)
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
    void setTrack(const QList<trackmime> &curClips)
    {
        tracks = std::vector<trackmime>(curClips.begin(), curClips.end());
    }
    ///添加轨道
    void addTrack(const trackmime &clipData)
    {
        tracks.emplace_back(clipData);
    }
    ///删除轨道,返回被删除的轨道
    void removeTrack(const QString &key)
    {
        std::vector<trackmime>::iterator itr;
        if (getTrackWithItr(key, itr)) {
            tracks.erase(itr);
        }
    }
    ///拿到指定轨道信息
    bool getTrack(trackmime &data, const QString &key)
    {
        return getTrackWithKey(key, data);
    }
    ///变更指定Track key的信息
    void setTrack(const QString &key, const trackmime &data)
    {
        std::vector<trackmime>::iterator itr;
        if (getTrackWithItr(key, itr))
            *itr = trackmime(data);
    }

#pragma  endregion


#pragma region clipFunc
    ///往指定Track中添加一个切片
    void addClip(const QString &trackKey, const clipmime &mime)
    {
        std::vector<trackmime>::iterator itr;
        if (!getTrackWithItr(trackKey, itr)) {
            return;
        }
        (*itr).addClip(mime);
    }
    ///从指定Track中删除一个切片
    void removeClip(const QString &trackKey, const QString &clipKey)
    {
        std::vector<trackmime>::iterator itr;
        if (!getTrackWithItr(trackKey, itr)) {
            return;
        }
        (*itr).removeClip(clipKey);
    }
    ///变更一个指定切片
    void
    setClip(const QString &trackKey, const QString &clipKey, const clipmime &mime, bool searchWhenTrackKeyFailed = true)
    {
        std::vector<trackmime>::iterator itr;
        trackmime curTrack;
        if (!getTrackWithItr(trackKey, itr)) {
            if (!getTrack(curTrack, [&](trackmime curTrack) -> bool
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
    clipmime getClip(const QString &clipKey, const QString &trackKey)
    {
        QList<trackmime> trackInSearch;
        clipmime curClip;
        if (!trackKey.isEmpty()) {
            trackmime curData;
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
    QList<clipmime> getClips(const QList<QString> &clipsKey)
    {
        QList<clipmime> clips;
        for (const auto &itr: clipsKey) {
            auto curClip = getClip(itr, "");
            if (!curClip.isDefaultData()) {
                clips.push_back(curClip);
            }
        }
        return clips;
    }
    ///通过组别拿到切片
    QList<clipmime> getClipByGroup(const QString &groupKey)
    {
        QList<clipmime> clips;
        if (std::find(clipGroupsRecord.begin(), clipGroupsRecord.end(), groupKey) == clipGroupsRecord.end())
            return clips;//key is not record;
        for (auto &track: tracks) {
            auto curTracks = track.getClips([&](const clipmime &curClip) -> bool
                                            {
                                                return curClip.groupId == groupKey;
                                            });
            clips.append(curTracks);
        }
        return clips;
    }
    ///获取在某个范围内的所有切片,可以指定轨道，否则返回所有轨道
    QList<clipmime> getClipsAtPosRange(ulong startPos, ulong endPos, const QString &specificTrackKey = "")
    {
        QList<clipmime> result;
        QList<trackmime> tracksInsearch;
        if (specificTrackKey.isEmpty()) {
            tracksInsearch = {tracks.begin(), tracks.end()};
        }
        else {
            trackmime curTrackData;
            if (getTrack(curTrackData, specificTrackKey)) {
                tracksInsearch.push_back(curTrackData);
            }
        }

        for (auto &curTrack: tracksInsearch) {
            result.append(curTrack.getClips([&](const clipmime &curClip) -> bool
                                            {
                                                return curClip.startPos >= startPos && curClip.startPos < endPos;
                                            }));
        }
    }

    ///获取切片里位置最前的一个，可以指定轨道，否则获取所有轨道
    clipmime getfirstClip(const QString &specificTrackKey = "")
    {
        clipmime curClip;
        ulong curMinPos = UINT32_MAX;
        QList<trackmime> tracksInsearch;
        if (specificTrackKey.isEmpty()) {
            tracksInsearch = getTracks();
        }
        else {
            trackmime curTrack;
            if (getTrack(curTrack, specificTrackKey)) {
                tracksInsearch.push_back(curTrack);
            }
            else {
                tracksInsearch = getTracks();
            }
        }
        std::for_each(tracksInsearch.begin(), tracksInsearch.end(), [&](const trackmime &curTrack) -> void
        {
            auto curPos = std::min_element(curTrack.clips.begin(), curTrack.clips.end(),
                                           [](const clipmime &left, const clipmime &right) -> bool
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
    clipmime getLastClip(const QString &specificTrackKey = "")
    {
        clipmime curClip;
        ulong curMaxPos = 0;

        QList<trackmime> tracksInsearch;
        if (specificTrackKey.isEmpty()) {
            tracksInsearch = getTracks();
        }
        else {
            trackmime curTrack;
            if (getTrack(curTrack, specificTrackKey)) {
                tracksInsearch.push_back(curTrack);
            }
            else {
                tracksInsearch = getTracks();
            }
        }

        std::for_each(tracksInsearch.begin(), tracksInsearch.end(), [&](const trackmime &curTrack) -> void
        {
            auto curPos = std::max_element(curTrack.clips.begin(), curTrack.clips.end(),
                                           [&](const clipmime &left, const clipmime &right) -> bool
                                           {
                                               return left.startPos + left.duration <= right.startPos + right.duration;
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

REFLECTION(timelinemime, id, tracks, clipGroupsRecord, m_ulFrameTick, m_ulMaxDuration, m_ulCurPos, m_maxFrameTick);
#endif //TIMELINEMIME_H
