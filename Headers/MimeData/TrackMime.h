//
// Created by 58226 on 2023/2/8.
//

#ifndef TRACKMIME_H
#define TRACKMIME_H
#include <QUuid>
#include "TimelineDefination.h"
#include "JsonSerialization.h"

struct TrackMime
{

///track unique id
    QString id="";
    ///track index
    int index = -1;
    ///track type
    SpecificType type = SpecificType::None;
    ///track visible status
    bool isVisible = true;
    ///track lock status
    bool isLocked = false;

    TrackMime()= default;

    TrackMime(const char *const string, int i, SpecificType type, bool visible=true, bool locked=false)
    {
        id =(string);//wcscpy(id,string);
        index= i;
        this->type = type;
        isVisible = visible;
        isLocked = locked;
    }
    TrackMime(const QString& string, int i, SpecificType type, bool visible=true, bool locked=false)
    {
        id = string;
        index= i;
        this->type = type;
        isVisible = visible;
        isLocked = locked;
    }
    TrackMime(const TrackMime&other)
    {
        id= other.id;
        index=other.index;
        type = other.type;
        isVisible = other.isVisible;
        isLocked = other.isLocked;
    }

    bool operator < (const TrackMime& other) const
    {
        return this->index<other.index;
    }
    TrackMime operator=(TrackMime other)
    {
        id = other.id;//memcpy(id,other.id, wcslen(other.id));
        index=other.index;
        type = other.type;
        isVisible = other.isVisible;
        isLocked = other.isLocked;
        return *this;
    }
    bool operator ==(const TrackMime& other)const
    {
        return (0==id==other.id)&&(index==other.index)&&(type==other.type)
        &&(isVisible==other.isVisible)&&(isLocked==isVisible);
    }

    QString toString()const
    {
        return "TrackMime{id:"+this->id+" index:"+QString::number(this->index)+" type:"
            +QString::number(this->type)+" isVisible:"+QVariant(this->isVisible).toString()
            +" isLocked:"+QVariant(this->isLocked).toString()+"}";
    }

    static bool isDefaultData(const TrackMime& other)
    {
        return (0==(other.id.length()))&&(other.index==-1)&&(other.type==SpecificType::None);
    }
};
REFLECTION(TrackMime,id,index,type,isVisible,isLocked);

#endif //TRACKMIME_H
