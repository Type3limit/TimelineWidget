//
// Created by 58226 on 2023/2/9.
//

#ifndef CLIPITEM_H
#define CLIPITEM_H


#include <QGraphicsItem>
#include "MimeData/clipmime.h"
#include "MimeData/trackmime.h"
#include "shadowclipitem.h"
#include "clipdraghandle.h"
class clipitem: public QGraphicsItem
{
public:
    explicit clipitem(QGraphicsItem* parent=nullptr);
    explicit clipitem(const QString& key, QGraphicsItem* parent=nullptr);
    ~clipitem();
    bool insertToTrack(const QString& trackKey);
    bool removeFromTrack();
    clipmime getMimeData(const QString& clipKey, bool searchWhenTrackKeyEmpty = true) const;
    trackmime getTrackData(const QString& trackKey, bool searchWhenTrackKeyEmpty = true)const;
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
    clipmime stopClipDrag(bool isMultiMode = false);
    bool checkForCollision(clipmime& mime, const QString& originTrackKey="");
    bool preCheckForCollision(clipmime& mime, trackmime &targetTrack);
    void checkExpandHandle(const QRectF& clipRect);
    ulong getStartPosAfterDragMove(clipmime& mime);
private:
    volatile bool m_isGrouped = false;
    volatile bool m_isDragMoved= false;
    volatile bool m_isOnHover= false;
    volatile bool m_isMouseDrag = false;
    volatile bool m_shouldIgnoreMultiTrackAdd =false;
    volatile bool m_isLeftExpand =false;
    volatile bool m_isRightExpand = false;
    volatile bool m_isRemoved= false;
    QPointF m_prePoint ;
    QString m_groupId;
    QString m_mimeKey;
    QString m_trackMimeKey;
    QString m_multiSelectionPrefixedTrackKey="";
    QRectF m_shadowRect;
    QRectF m_originRect;
    shadowclipitem* m_shadow = nullptr;
    clipdraghandle* m_leftHandle = nullptr;
    clipdraghandle* m_rightHandle = nullptr;
    QPixmap m_image;

    friend class timelinewidget;
    friend class trackbodyitem;
};


#endif //CLIPITEM_H