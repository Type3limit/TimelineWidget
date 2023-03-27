//
// Created by 58226 on 2023/2/6.
//

#ifndef TRACKHEADDRAWINGVIEW_H
#define TRACKHEADDRAWINGVIEW_H


#include <QGraphicsView>
#include <QMutex>
#include <QWaitCondition>
#include "selfcontainedscenesview.h"
#include "trackheaditem.h"
#include "timelinedefination.h"
class trackheaddrawingview: public SelfContainedSceneView
{
public:
    explicit trackheaddrawingview(QWidget* parent = nullptr);
public:
    ///添加一个项
    bool addTrackHead(const trackmime& originData);
    ///删除一个项
    bool deleteTrackHead(const QString& key);
    ///获取一个项
    trackheaditem* getTrackHead(const QString& key);
    ///更改一个项
    trackheaditem* updateTrackHead(const QString& key, trackheaditem* curData);
    ///清楚所有项
    void emptyTracks();
public slots:
    void OnTrackHeadUpdate(const QString& key);
    void OnTrackBodyScroll(int pos);
private:
    QMap<QString, trackheaditem*> m_headItems;
};


#endif //TRACKHEADDRAWINGVIEW_H
