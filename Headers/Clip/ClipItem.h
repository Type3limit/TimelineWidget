//
// Created by 58226 on 2023/2/9.
//

#ifndef CLIPITEM_H
#define CLIPITEM_H


#include <QGraphicsItem>
#include "MimeData/ClipMime.h"
#include "MimeData/TrackMime.h"
#include "ShadowClipItem.h"
class ClipItem: public QGraphicsItem
{
public:
    explicit ClipItem(QGraphicsItem* parent=nullptr);
    explicit ClipItem(const QString& key,QGraphicsItem* parent=nullptr);
    ~ClipItem();
    bool insertToTrack(const QString& trackKey);
    bool removeFromTrack();
    ClipMime getMimeData(bool searchWhenTrackKeyEmpty = true) const;
    TrackMime getTrackData(bool searchWhenTrackKeyEmpty = true)const;
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
private:
    volatile bool m_isGrouped = false;
    volatile bool m_isDragMoved= false;
    volatile bool m_isOnHover= false;
    QPointF m_prePoint ;
    QString m_groupId;
    QString m_mimeKey;
    QString m_trackMimeKey;
    QRectF m_shadowRect;
    QRectF m_originRect;
    ShadowClipItem* m_shadow = nullptr;
    QImage m_image;
    void Init();
};


#endif //CLIPITEM_H
