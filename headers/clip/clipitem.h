//
// Created by 58226 on 2023/2/9.
//

#ifndef CLIPITEM_H
#define CLIPITEM_H


#include <QGraphicsItem>
#include "mimedata/clipmime.h"
#include "mimedata/trackmime.h"
#include "shadowclipitem.h"
#include "clipdraghandle.h"
class ClipItem: public QGraphicsItem
{
public:
    explicit ClipItem(QGraphicsItem *parent = nullptr);
    explicit ClipItem(const QString &key, QGraphicsItem *parent = nullptr);
    ~ClipItem();
    bool insertToTrack(const QString &trackKey);
    bool removeFromTrack();
    ClipMime getMimeData(const QString &clipKey, bool searchWhenTrackKeyEmpty = true) const;
    TrackMime getTrackData(const QString &trackKey, bool searchWhenTrackKeyEmpty = true) const;
    void forceUpdate();
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    void init();
    void clipDrag(double x, double y);
    void removeShadow();
    ClipMime stopClipDrag(bool &isSceneNull, bool posNotMove, bool isMultiMode = false);
    bool checkForCollision(ClipMime &mime, const QString &originTrackKey = "");
    bool preCheckForCollision(ClipMime &mime, TrackMime &targetTrack);
    void checkExpandHandle(const QRectF &clipRect);
    double getStartPosAfterDragMove(ClipMime &mime,bool shouldIgnorePos);
private:
    volatile bool m_isGrouped = false;
    volatile bool m_isDragMoved = false;
    volatile bool m_isOnHover = false;
    volatile bool m_isMouseDrag = false;
    volatile bool m_isLeftExpand = false;
    volatile bool m_isRightExpand = false;
    volatile bool m_isRemoved = false;
    QPointF m_prePoint;
    QPointF m_originPoint;
    QString m_groupId;
    QString m_mimeKey;
    QString m_trackMimeKey;

    QRectF m_shadowRect;
    QRectF m_originRect;
    ShadowClipItem* m_shadow = nullptr;
    ClipDragHandle* m_leftHandle = nullptr;
    ClipDragHandle* m_rightHandle = nullptr;
    //for test
    QPixmap m_image;

    friend class TimelineWidget;
    friend class TrackBodyItem;
    friend class TrackBodyDrawingView;
};


#endif //CLIPITEM_H
