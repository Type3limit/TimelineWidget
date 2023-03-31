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

    void addClipItem(const QString &itemKey,const QString& trackKey);
    void removeClipItem(const QString& itemKey);
    void updateClipItem(const QString& itemKey);
    ClipItem* getClipItem(const QString& itemKey);
    ///获取一个项
    TrackBodyItem* getTrackBody(const QString& key);
    ///更改一个项
    TrackBodyItem* updateTrackBody(const QString& key, TrackBodyItem* curData);
    ///删除所有轨道
    void emptyTracks();
    //获取vertical scroll bar 值
    int getVScroll();
    /// 释放所有被标识为删除的切片
    void removeAllDeletedClip();
public slots:
    void onTrackHeadUpdate(const QString& key);
    void scrolledTo(int pos);
    void vScrolledTo(int pos);
    void scrollToPos(int pos);
    void clipChanged(const QString& trackKey, const QString& clipKey, int mode);
    void singleClipChanged(const QString& trackKey,const QString& clipKey);
    void onCursorPosChanged(ulong pos);
signals:
    void scrollChanged(int pos);
    void vScollChanged(int pos);
private:
    volatile bool m_bIsRightButtonPressed = false;
    volatile bool m_bIsMultiSelectionMode = false;
    QPointF m_selectionStart;
    QPointF m_selectionEnd;
    QPointF m_prePos;
    AnchorBodyItem* m_anchorBody = nullptr;
    TrackSelectionItem* m_selectObj = nullptr;
private:
    QMap<QString, TrackBodyItem*> m_bodyItems;
    QMap<QString, ClipItem*> m_clips;
    QList<ClipItem*> m_removedClips;
};


#endif //TRACKBODYDRAWINGVIEW_H
