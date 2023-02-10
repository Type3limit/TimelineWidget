//
// Created by 58226 on 2023/2/9.
//

#ifndef CLIPITEM_H
#define CLIPITEM_H


#include <QGraphicsItem>
#include "MimeData/ClipMime.h"
#include "MimeData/TrackMime.h"
class ClipItem: public QGraphicsItem
{
public:
    explicit ClipItem(QGraphicsItem* parent=nullptr);
    explicit ClipItem(const QString& key,QGraphicsItem* parent=nullptr);
    bool insertToTrack(const QString& trackKey);
    bool removeFromTrack();
    ClipMime getMimeData(bool searchWhenTrackKeyEmpty = true) const;
    TrackMime getTrackData(bool searchWhenTrackKeyEmpty = true)const;
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    QString m_mimeKey;
    QString m_trackMimeKey;
};


#endif //CLIPITEM_H
