//
// Created by 58226 on 2023/2/9.
//

#ifndef CLIPMIME_H
#define CLIPMIME_H
#include <QUuid>
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
    ///clip extension info(stay Temporarily)
    QString extension;
};

REFLECTION(ClipMime,id,trackId,startPos,duration,type,groupId,sourceFile,extension);
#endif //CLIPMIME_H
