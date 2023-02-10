//
// Created by 58226 on 2023/2/6.
//

#ifndef TRACKBODYDRAWINGVIEW_H
#define TRACKBODYDRAWINGVIEW_H


#include <QGraphicsView>
#include "SelfContainedScenesView.h"
#include "AnchorBodyItem.h"
#include "TrackBodyItem.h"
#include <QScrollBar>
class TrackBodyDrawingView: public SelfContainedSceneView
{
    Q_OBJECT
public:
    explicit TrackBodyDrawingView(QWidget*parent = nullptr);
    ~TrackBodyDrawingView() override;
    ///将区域滚动到指针位置
    void scrollToCursor();

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
public slots:
    void OnTrackHeadUpdate(const QString& key);

public slots:
    void ScrolledTo(int pos);
    void ScrollToPos(int pos);
    void ClipChanged(const QString& trackKey,const QString& clipKey,int mode);
    signals:
    void ScrollChanged(int pos);


private:
    volatile bool m_bIsRightButtonPressed = false;
    QPointF m_prePos;
    AnchorBodyItem* m_anchorBody = nullptr;
private:
    QMap<QString,TrackBodyItem*> m_bodyItems;
};


#endif //TRACKBODYDRAWINGVIEW_H
