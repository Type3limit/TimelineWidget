//
// Created by 58226 on 2023/2/6.
//

#ifndef TRACKHEADDRAWINGVIEW_H
#define TRACKHEADDRAWINGVIEW_H


#include <QGraphicsView>
#include <QMutex>
#include <QWaitCondition>
#include "SelfContainedScenesView.h"
#include "TrackHeadItem.h"
#include "TimelineDefination.h"
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
    ///使一个项重绘
public slots:
    void OnTrackHeadUpdate(const QString& key);
private:
    QMap<QString,TrackHeadItem*> m_headItems;
};


#endif //TRACKHEADDRAWINGVIEW_H
