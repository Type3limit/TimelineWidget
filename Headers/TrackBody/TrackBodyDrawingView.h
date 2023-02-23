//
// Created by 58226 on 2023/2/6.
//

#ifndef TRACKBODYDRAWINGVIEW_H
#define TRACKBODYDRAWINGVIEW_H


#include <QGraphicsView>
#include "SelfContainedScenesView.h"
#include "AnchorBodyItem.h"
#include "TrackBodyItem.h"
#include "TrackSelectionItem.h"
#include <QScrollBar>
class TrackBodyDrawingView: public SelfContainedSceneView
{
    Q_OBJECT
public:
    explicit TrackBodyDrawingView(QWidget*parent = nullptr);
    ~TrackBodyDrawingView() override;
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
    bool addTrackBody(const TrackMime& originData);
    ///删除一个项
    bool deleteTrackBody(const QString& key);
    ///获取一个项
    TrackBodyItem* getTrackBody(const QString& key);
    ///更改一个项
    TrackBodyItem* updateTrackBody(const QString& key, TrackBodyItem* curData);
    ///删除所有轨道
    void emptyTracks();

public slots:
    void OnTrackHeadUpdate(const QString& key);
    void ScrolledTo(int pos);
    void ScrollToPos(int pos);
    void ClipChanged(const QString& trackKey,const QString& clipKey,int mode);
    void singleClipChanged(const QString& trackKey,const QString& clipKey);
signals:
    void ScrollChanged(int pos);
private:
    volatile bool m_bIsRightButtonPressed = false;
    volatile bool m_bIsMultiSelectionMode = false;
    QPointF m_selectionStart;
    QPointF m_selectionEnd;
    QPointF m_prePos;
    AnchorBodyItem* m_anchorBody = nullptr;
    TrackSelectionItem* m_selectObj = nullptr;
private:
    QMap<QString,TrackBodyItem*> m_bodyItems;
};


#endif //TRACKBODYDRAWINGVIEW_H
