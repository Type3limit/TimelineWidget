//
// Created by 58226 on 2023/2/6.
//

#include "TrackHead/trackheaddrawingview.h"
#include <QGraphicsItem>
#include "timelinedefination.h"
#include "timelinewidget.h"
#define TimelineInstance() (GET_POINTER<timelinewidget>())
trackheaddrawingview::trackheaddrawingview(QWidget *parent)
    : SelfContainedSceneView(parent)
{
    connect(TimelineInstance(), &timelinewidget::TrackUpdated, this, &trackheaddrawingview::OnTrackHeadUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    verticalScrollBar()->setStyleSheet(" QScrollBar:vertical\n"
                                       "    {\n"
                                       "        background-color: #00000000;\n"
                                       "        width: 1px;\n"
                                       "        margin: 0px;\n"
                                       "        border: 0px transparent;\n"
                                       "        border-radius: 0px;\n"
                                       "    }"
                                       "QScrollBar::handle:vertical\n"
                                       "    {\n"
                                       "        background-color: #00515151;       \n"
                                       "        min-height: 0px;\n"
                                       "        border-radius: 0px;\n"
                                       "    }"
                                       "QScrollBar::sub-line:vertical\n"
                                       "    {\n"
                                       "        margin: 0px;\n"
                                       "        border-image: url(:/qss_icons/rc/up_arrow_disabled.png);\n"
                                       "        height: 0px;\n"
                                       "        width: 0px;\n"
                                       "        subcontrol-position: top;\n"
                                       "        subcontrol-origin: margin;\n"
                                       "    }"
                                       "QScrollBar::add-line:vertical\n"
                                       "    {\n"
                                       "        margin: 0px 0px 0px 0px;\n"
                                       "        border-image: url(:/qss_icons/rc/down_arrow_disabled.png);\n"
                                       "        height: 0px;\n"
                                       "        width: 0px;\n"
                                       "        subcontrol-position: bottom;\n"
                                       "        subcontrol-origin: margin;\n"
                                       "    }"
                                       "QScrollBar::sub-line:vertical:hover,QScrollBar::sub-line:vertical:on\n"
                                       "    {\n"
                                       "        border-image: url(:/qss_icons/rc/up_arrow.png);\n"
                                       "        height: 0px;\n"
                                       "        width: 0px;\n"
                                       "        subcontrol-position: top;\n"
                                       "        subcontrol-origin: margin;\n"
                                       "    }"
                                       "  QScrollBar::add-line:vertical:hover, QScrollBar::add-line:vertical:on\n"
                                       "    {\n"
                                       "        border-image: url(:/qss_icons/rc/down_arrow.png);\n"
                                       "        height: 0px;\n"
                                       "        width: 0px;\n"
                                       "        subcontrol-position: bottom;\n"
                                       "        subcontrol-origin: margin;\n"
                                       "    }"
                                       "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical\n"
                                       "    {\n"
                                       "        background: none;\n"
                                       "    }"
                                       " QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical\n"
                                       "    {\n"
                                       "        background: none;\n"
                                       "    }");
}
bool trackheaddrawingview::addTrackHead(const trackmime& originData)
{
    //qDebug()<<"add head with key:"<<originData.id;
    auto key = originData.id;
    if(m_headItems.contains(key))
    {
        qDebug()<<"add track head failed! Already has the same key:"<<key;
        return false;
    }
    auto curData = new trackheaditem(originData);
    m_headItems.insert(key,curData);
    scene()->addItem(curData);
    return true;
}
bool trackheaddrawingview::deleteTrackHead(const QString& key)
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
trackheaditem *trackheaddrawingview::getTrackHead(const QString& key)
{
    if(!m_headItems.contains(key))
        return nullptr;
    return m_headItems[key];
}
trackheaditem *trackheaddrawingview::updateTrackHead(const QString& key, trackheaditem*curData)
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
void trackheaddrawingview::OnTrackHeadUpdate(const QString &key)
{
    if(!m_headItems.contains(key))
        return ;
    m_headItems[key]->forceUpdate();
}
void trackheaddrawingview::emptyTracks()
{
    std::for_each(m_headItems.begin(), m_headItems.end(),[&](auto item)->void
    {
       if(item!=nullptr)
       {
           SAFE_DELETE(item);
       }
    });
    m_headItems.clear();
}
void trackheaddrawingview::OnTrackBodyScroll(int pos)
{
    verticalScrollBar()->setValue(pos);
    updateGeometry();
}
