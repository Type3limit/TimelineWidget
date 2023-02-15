//
// Created by 58226 on 2023/2/6.
//

#include "TrackHead/TrackHeadDrawingView.h"
#include <QGraphicsItem>
#include "TimelineDefination.h"
#include "timelinewidget.h"
#define TimelineInstance() (GET_POINTER<TimelineWidget>())
TrackHeadDrawingView::TrackHeadDrawingView(QWidget *parent)
    : SelfContainedSceneView(parent)
{
    connect(TimelineInstance(),&TimelineWidget::TrackUpdated,this,&TrackHeadDrawingView::OnTrackHeadUpdate);
}
bool TrackHeadDrawingView::addTrackHead( const TrackMime& originData)
{
    //qDebug()<<"add head with key:"<<originData.id;
    auto key = originData.id;
    if(m_headItems.contains(key))
    {
        qDebug()<<"add track head failed! Already has the same key:"<<key;
        return false;
    }
    auto curData = new TrackHeadItem(originData);
    m_headItems.insert(key,curData);
    scene()->addItem(curData);
    return true;
}
bool TrackHeadDrawingView::deleteTrackHead(const QString& key)
{
    if(m_headItems.contains(key)) {

        auto org = m_headItems[key];
        scene()->removeItem(org);
        m_headItems.remove(key);
        SAFE_DELETE(org);
        return true;
    }
    qDebug()<<"delete track head failed!key:["<<key<<"]is not exist";
    return false;
}
TrackHeadItem *TrackHeadDrawingView::getTrackHead(const QString& key)
{
    if(!m_headItems.contains(key))
        return nullptr;
    return m_headItems[key];
}
TrackHeadItem *TrackHeadDrawingView::updateTrackHead(const QString& key, TrackHeadItem*curData)
{
    if(!m_headItems.contains(key))
        return nullptr;
    auto org = m_headItems[key];
    if(org!=curData)
    {
        m_headItems[key]=curData;
        SAFE_DELETE(org);
    }
    return m_headItems[key];
}
void TrackHeadDrawingView::OnTrackHeadUpdate(const QString &key)
{
    if(!m_headItems.contains(key))
        return ;
    m_headItems[key]->forceUpdate();
}
