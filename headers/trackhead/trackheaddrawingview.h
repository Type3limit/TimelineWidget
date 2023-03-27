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
class TrackHeadDrawingView: public SelfContainedSceneView
{
public:
    explicit TrackHeadDrawingView(QWidget* parent = nullptr);
public:
    ///添加一个项
    bool addTrackHead(const TrackMime& originData);
    ///删除一个项
    bool deleteTrackHead(const QString& key);
    ///获取一个项
    TrackHeadItem* getTrackHead(const QString& key);
    ///更改一个项
    TrackHeadItem* updateTrackHead(const QString& key, TrackHeadItem* curData);
    ///清楚所有项
    void emptyTracks();
public slots:
    void onTrackHeadUpdate(const QString& key);
    void onTrackBodyScroll(int pos);
private:
    QMap<QString, TrackHeadItem*> m_headItems;
};


#endif //TRACKHEADDRAWINGVIEW_H
