//
// Created by 58226 on 2023/2/8.
//

#ifndef TRACKMIME_H
#define TRACKMIME_H
#include <QUuid>
#include "timelinedefination.h"
#include "jsonserialization.h"
#include "clipmime.h"

struct trackmime
{
    ///track unique id
    QString id = "";
    ///track index
    int index = -1;
    ///track type
    SpecificType type = SpecificType::None;
    ///track visible status
    bool isVisible = true;
    ///track lock status
    bool isLocked = false;
    ///clips in track
    std::vector<clipmime> clips;
    trackmime() = default;
    trackmime(const QString &string, int i, SpecificType type, bool visible = true, bool locked = false)
    {
        id = string;
        index = i;
        this->type = type;
        isVisible = visible;
        isLocked = locked;
    }
    trackmime(const trackmime &other)
    {
        id = other.id;
        index = other.index;
        type = other.type;
        isVisible = other.isVisible;
        isLocked = other.isLocked;
        clips = std::vector(other.clips);
    }
#pragma region overloadOperators
    bool operator<(const trackmime &other) const
    {
        return this->index < other.index;
    }
    trackmime &operator=(const trackmime &other)
    {
        id = other.id;
        index = other.index;
        type = other.type;
        isVisible = other.isVisible;
        isLocked = other.isLocked;
        clips = std::vector(other.clips);
        return *this;
    }
    bool operator==(const trackmime &other) const
    {
        return (id == other.id) && (index == other.index) && (type == other.type)
            && (isVisible == other.isVisible) && (isLocked == isVisible);
    }
#pragma endregion

#pragma region func
    QString toString() const
    {

        QString clipStr;
        for (const auto &itr: clips) {
            clipStr += itr.toString() + '\n';
        }
        return "trackmime{id:" + this->id + " index:" + QString::number(this->index) + " type:"
            + QString::number(this->type) + " isVisible:" + QVariant(this->isVisible).toString()
            + " isLocked:" + QVariant(this->isLocked).toString() + " clips:[" + clipStr + "]}";
    }
    bool isDefaultData() const
    {
        return isDefaultData(*this);
    }
    static bool isDefaultData(const trackmime &other)
    {
        return (0 == (other.id.length())) && (other.index == -1) && (other.type == SpecificType::None);
    }
#pragma endregion

#pragma region clip options
private:
    inline bool getClipWithKey(const QString &key, clipmime &data)
    {
        auto itr = clips.end();
        if (!getClipWithItr(key, itr))
            return false;
        data = *itr;
        return true;
    }
    inline bool getClipWithItr(const QString key, std::vector<clipmime>::iterator &itr)
    {
        itr = std::find_if(clips.begin(), clips.end(), [&](const clipmime &itr) -> bool
        {
            return itr.id == key;
        });
        return itr != clips.end();
    }

public:
    ///获取所有切片
    QList<clipmime> getClips()
    {
        return {clips.begin(), clips.end()};
    }
    ///获取指定条件的切片
    QList<clipmime> getClips(const std::function<bool(const clipmime&)> &matchFunction)
    {
        QList<clipmime> res;
        for (const auto &itr: clips) {
            if (matchFunction(itr))
                res.push_back(itr);
        }
        return res;
    }

    ///设置变更所有切片
    void setClips(const QList<clipmime> &curClips)
    {
        clips = std::vector<clipmime>(curClips.begin(), curClips.end());
    }
    ///添加切片
    void addClip(const clipmime &clipData)
    {
        clips.emplace_back(clipData);
    }
    ///删除切片
    void removeClip(const QString &key)
    {
        std::vector<clipmime>::iterator itr;
        if (getClipWithItr(key, itr)) {
            clips.erase(itr);//(data);
        }
    }
    ///拿到指定切片信息
    clipmime getClip(const QString &key)
    {
        std::vector<clipmime>::iterator itr;
        if (!getClipWithItr(key, itr))
            return {};
        return *itr;
    }
    ///变更指定clip key 的信息
    void setClip(const QString &key, const clipmime &data)
    {
        std::vector<clipmime>::iterator itr;
        if (getClipWithItr(key, itr)) {
            *itr = clipmime(data);
        }
    }
#pragma endregion
};

REFLECTION(trackmime, id, index, type, isVisible, isLocked, clips);

#endif //TRACKMIME_H
