//
// Created by 58226 on 2023/2/9.
//

#ifndef CLIPMIME_H
#define CLIPMIME_H
#include <QUuid>
#include <utility>
#include "TimelineDefination.h"
#include "JsonSerialization.h"
struct ClipMime
{
    //clip id
    QString id = "";
    //parent track id
    QString trackId = "";
    ///clip pos of track
    ulong startPos = 0;
    ///clip duration
    ulong duration = 0;
    ///clip type
    SpecificType type = SpecificType::None;
    ///clip groups id
    QString groupId;
    ///clip source file info
    QString sourceFile;
    ///clip extension info(stay temporarily)
    QString extension;

    ClipMime() = default;
    ClipMime(const ClipMime &other)
    {
        id = other.id;
        trackId = other.trackId;
        startPos = other.startPos;
        duration = other.duration;
        type = other.type;
        groupId = other.groupId;
        sourceFile = other.sourceFile;
        extension = other.extension;
    }
    ClipMime(const QString& id,const QString& trackId,ulong startPos,ulong duration,SpecificType type,const QString& groupId="",const QString& sourceFile="",const QString& extension="")
    {
        this->id = id;
        this->trackId = trackId;
        this->startPos = startPos;
        this->duration = duration;
        this->type = type;
        this->groupId= groupId;
        this->sourceFile = sourceFile;
        this->extension = extension;
    }

#pragma region overloadOperator
    bool operator<(const ClipMime &other) const
    {
        return this->startPos < other.startPos;
    }
    ClipMime& operator=(const ClipMime &other)
    {
        id = other.id;
        trackId = other.trackId;
        startPos = other.startPos;
        duration = other.duration;
        type = other.type;
        groupId = other.groupId;
        sourceFile = other.sourceFile;
        extension = other.extension;
        return *this;
    }
    bool operator==(const ClipMime &other) const
    {
        return std::tie(id, trackId, type, startPos, duration, sourceFile, groupId, extension) ==
            std::tie(other.id,
                     other.trackId,
                     other.type,
                     other.startPos,
                     other.duration,
                     other.sourceFile,
                     other.groupId,
                     other.extension);
    }
#pragma endregion
#pragma region func
    QString toString() const
    {
        return "ClipMime{id:" + this->id + " track id:" + this->trackId + " type:"
            + QString::number(this->type) + " startPos:" + QString::number(this->startPos)
            + " duration:" + QString::number(this->duration) + " group id:" + this->groupId
            + " source file:" + this->sourceFile + " extension:" + this->extension + "}";
    }

    bool isDefaultData() const
    {
        return isDefaultData(*this);
    }
    static bool isDefaultData(const ClipMime &other)
    {
        return (0 == (other.id.length())) && (other.trackId.length() == 0) && (other.type == SpecificType::None)
            && other.startPos == 0 && other.duration == 0;
    }
#pragma endregion
};

REFLECTION(ClipMime, id, trackId, startPos, duration, type, groupId, sourceFile, extension);
#endif //CLIPMIME_H
