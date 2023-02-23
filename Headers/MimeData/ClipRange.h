//
// Created by 58226 on 2023/2/22.
//

#ifndef CLIPRANGE_H
#define CLIPRANGE_H
#include <QMap>
#include <Qset>
#include "TimelineMime.h"

const int HEAD=0;
const int TAIL=1;
struct ClipRange
{
public:
    ClipRange()= default;
    explicit ClipRange(QList<TrackMime>& data)
    {
        buildFromTimelineData(data);
    }
    ///由TimelineMime 获取切片范围值
    void buildFromTimelineData(QList<TrackMime> data);
    ///一个切片的数据发生改变
    void oneClipChanged(const ClipMime& clipData);
    ///查询某个轨道上的某个切片是否存在碰撞
    bool hasCollision(const QString& trackKey,const QString& clipKey,QList<QString>& collisionItem);
private:
    QMap<QString,QMap<QString,QList<uint64_t>>> m_datas;
};

template<typename A, typename B>
QPair<B,A> flip_pair(const QPair<A,B> &p)
{
    return QPair<B,A>(p.second, p.first);
}

template<typename A, typename B>
QMap<B,A> flip_map(const QMap<A,B> &src)
{
    QMap<B,A> dst;
    std::transform(src.begin(), src.end(), std::inserter(dst, dst.begin()),
                   flip_pair<A,B>);
    return dst;
}




#endif //CLIPRANGE_H
