//
// Created by 58226 on 2023/2/6.
//

#ifndef TRACKBODYDRAWINGVIEW_H
#define TRACKBODYDRAWINGVIEW_H


#include <QGraphicsView>
#include "selfcontainedscenesview.h"
#include "anchorbodyitem.h"
#include "trackbodyitem.h"
#include "trackselectionitem.h"
#include <QScrollBar>
class trackbodydrawingview: public SelfContainedSceneView
{
    Q_OBJECT
public:
    explicit trackbodydrawingview(QWidget*parent = nullptr);
    ~trackbodydrawingview() override;
    ///将区域滚动到指针位置
    void scrollToCursor();
    void setDrawingAreaSize(int width, int height) override;
protected:
    void wheelEvent(QWheelEvent *event)override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

public:
    ///添加一个项
    bool addTrackBody(const trackmime& originData);
    ///删除一个项
    bool deleteTrackBody(const QString& key);
    ///获取一个项
    trackbodyitem* getTrackBody(const QString& key);
    ///更改一个项
    trackbodyitem* updateTrackBody(const QString& key, trackbodyitem* curData);
    ///删除所有轨道
    void emptyTracks();
    //获取vertical scroll bar 值
    int getVScroll();
public slots:
    void OnTrackHeadUpdate(const QString& key);
    void ScrolledTo(int pos);
    void VScrolledTo(int pos);
    void ScrollToPos(int pos);
    void ClipChanged(const QString& trackKey,const QString& clipKey,int mode);
    void singleClipChanged(const QString& trackKey,const QString& clipKey);
    void onCursorPosChanged(ulong pos);
signals:
    void ScrollChanged(int pos);
    void VScollChanged(int pos);
private:
    volatile bool m_bIsRightButtonPressed = false;
    volatile bool m_bIsMultiSelectionMode = false;
    QPointF m_selectionStart;
    QPointF m_selectionEnd;
    QPointF m_prePos;
    anchorbodyitem* m_anchorBody = nullptr;
    trackselectionitem* m_selectObj = nullptr;
private:
    QMap<QString, trackbodyitem*> m_bodyItems;
};


#endif //TRACKBODYDRAWINGVIEW_H
