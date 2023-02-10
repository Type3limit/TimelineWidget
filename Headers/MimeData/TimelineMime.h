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
struct TimelineMime{
    ///TimelineKey
    QString id;
    ///Tracks,clips contained in track
    std::vector<TrackMime> tracks;
    TimelineMime()=default;
    TimelineMime(const TimelineMime& other)
    {
        id = other.id;
        tracks = (other.tracks);
    }
    explicit TimelineMime(const QString& ids)
    {
        id = (ids);
    }
#pragma  region track func
private:
    inline bool getTrackWithKey(const QString& key,TrackMime& data)
    {
        auto itr = tracks.end();
        if(!getTrackWithItr(key,itr))
            return false;
        data = *itr;
        return true;
    }
    inline bool getTrackWithItr(const QString key, std::vector<TrackMime>::iterator& itr)
    {
        itr= std::find_if(tracks.begin(), tracks.end(),[&](const TrackMime& itr)->bool
        {
            return itr.id==key;
        });
        return itr != tracks.end();
    }
public:
    ///获取所有轨道
    QList<TrackMime> getTracks()
    {
        return {tracks.begin(),tracks.end()};
    }
    ///获取指定条件的切片
    QList<TrackMime> getTracks(const std::function<bool(TrackMime)>& matchFunction)
    {
        QList<TrackMime> res;
        for(const auto& itr:tracks)
        {
            if(matchFunction(itr))
                res.push_back(itr);
        }
        return res;
    }

    bool getTrack(TrackMime& outData,const std::function<bool(TrackMime)>& matchFunction)
    {
        bool res = false;
        for(const auto& itr:tracks)
        {
            if(matchFunction(itr))
            {
                res= true;
                outData = itr;
                break;
            }
        }
        return res;
    }
    ///设置变更所有轨道
    void setTrack(const QList<TrackMime>& curClips)
    {
        tracks = std::vector<TrackMime>(curClips.begin(), curClips.end());
    }
    ///添加轨道
    void addTrack(const TrackMime& clipData)
    {
        tracks.emplace_back(clipData);
    }
    ///删除轨道
    void removeTrack(const QString& key){
        std::vector<TrackMime>::iterator itr;
        if(getTrackWithItr(key,itr))
        {
            tracks.erase(itr);//(data);
        }
    }
    ///拿到指定轨道信息
    bool getTrack( TrackMime& data,const QString& key)
    {
        return getTrackWithKey(key,data);
    }
    ///变更指定Track key的信息
    void setTrack(const QString& key ,const TrackMime& data)
    {
        std::vector<TrackMime>::iterator itr;
        if(getTrackWithItr(key,itr))
            *itr = TrackMime(data);
    }
#pragma  endregion
};
REFLECTION(TimelineMime,id,tracks);
#endif //TIMELINEMIME_H
