//
// Created by 58226 on 2023/2/9.
//

#ifndef CLIPITEM_H
#define CLIPITEM_H


#include <QGraphicsItem>
#include "MimeData/ClipMime.h"
#include "MimeData/TrackMime.h"
#include "ShadowClipItem.h"
#include "ClipDragHandle.h"
class ClipItem: public QGraphicsItem
{
public:
    explicit ClipItem(QGraphicsItem* parent=nullptr);
    explicit ClipItem(const QString& key,QGraphicsItem* parent=nullptr);
    ~ClipItem();
    bool insertToTrack(const QString& trackKey);
    bool removeFromTrack();
    ClipMime getMimeData(const QString& clipKey,bool searchWhenTrackKeyEmpty = true) const;
    TrackMime getTrackData(const QString& trackKey,bool searchWhenTrackKeyEmpty = true)const;
    void forceUpdate();
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
private:
    void Init();
    void clipDrag(int x,int y);
    void stopClipDrag();
    void checkForCollision(ClipMime mime,const QString& originTrackKey="");
    void checkExpandHandle(const QRectF& clipRect);
private:
    volatile bool m_isGrouped = false;
    volatile bool m_isDragMoved= false;
    volatile bool m_isOnHover= false;
    volatile bool m_isMouseDrag = false;

    volatile bool m_isLeftExpand =false;
    volatile bool m_isRightExpand = false;

    QPointF m_prePoint ;
    QString m_groupId;
    QString m_mimeKey;
    QString m_trackMimeKey;
    QRectF m_shadowRect;
    QRectF m_originRect;
    ShadowClipItem* m_shadow = nullptr;
    ClipDragHandle* m_leftHandle = nullptr;
    ClipDragHandle* m_rightHandle = nullptr;
    QPixmap m_image;

    friend class TimelineWidget;
    friend class TrackBodyItem;
};


#endif //CLIPITEM_H
