//
// Created by 58226 on 2023/2/22.
//

#include "MimeData/ClipRange.h"
void ClipRange::buildFromTimelineData(QList<TrackMime> data)
{
    m_datas.clear();
    std::for_each(data.begin(), data.end(), [&](TrackMime &cur) -> void
    {
        QMap<QString, QList<uint64_t>> clipsData;
        std::sort(cur.clips.begin(), cur.clips.end(), [&](const ClipMime &left, const ClipMime &right) -> bool
        {
            return left.startPos < right.startPos;
        });

        for (const auto &itr: cur.clips) {
            clipsData.insert(itr.id, {itr.startPos, itr.startPos + itr.duration});
        }
        m_datas.insert(cur.id, clipsData);
    });
}
void ClipRange::oneClipChanged(const ClipMime &clipData)
{
    if (!m_datas.contains(clipData.trackId)
        || m_datas[clipData.trackId].contains(clipData.id))
        return;
    m_datas[clipData.trackId][clipData.id] = {clipData.startPos, clipData.startPos + clipData.duration};
}
bool ClipRange::hasCollision(const QString &trackKey, const QString &clipKey, QList<QString> &collisionItem)
{
    if (!m_datas.contains(trackKey))
        return false;
    auto begin = m_datas[trackKey].begin();
    auto itr = begin;
    auto end = m_datas[trackKey].end();
    auto head = m_datas[trackKey][clipKey][HEAD];
    auto tail = m_datas[trackKey][clipKey][TAIL];
    while (itr != end) {
        if (itr.key() == clipKey)
            continue;
        auto curStart = itr.value()[HEAD];
//        if(curStart>tail)//默认来看，除去被更改过的切片，其他切片应该是有序的
//            break; // 2023-2-22 是否存在一种情况……两个切片同时被修改？
//        无论如何，返回与指定切片相交的所有切片，即完整地遍历轨道
        auto curEnd = itr.value()[TAIL];
        if (curStart <= tail && head >= curEnd) {
            collisionItem.push_back(itr.key());
        }
        itr++;
    }
    return collisionItem.count() > 0;
}
bool ClipRange::isClipExistAt(uint64_t pos, const QString &trackKey, QList<QString> &datas)
{
    if (!m_datas.contains(trackKey))
       return false;
    auto begin = m_datas[trackKey].begin();
    auto itr = begin;
    auto end = m_datas[trackKey].end();
    while(itr != end)
    {
        if(itr.value()[HEAD]<=pos&&itr.value()[TAIL]>=pos)
            datas.push_back(itr.key());
    }
    return datas.count()>0;
}

