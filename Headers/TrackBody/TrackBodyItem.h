//
// Created by 58226 on 2023/2/7.
//

#ifndef TRACKBODYITEM_H
#define TRACKBODYITEM_H


#include <QGraphicsItem>
#include "MimeData/TrackMime.h"
#include "Clip/ClipItem.h"
class TrackBodyItem: public QGraphicsItem
{
public:
    explicit TrackBodyItem(QGraphicsItem* parent= nullptr);
    explicit TrackBodyItem(const TrackMime& curData,QGraphicsItem* parent= nullptr);
    ~TrackBodyItem();
    TrackMime getMimeData() const;
    void forceUpdate();
public:
    void addClipItem(const QString& itemKey);
    void removeClipItem(const QString& itemKey);
    void updateClipItem(const QString& itemKey);
    ClipItem* getClipItem(const QString& itemKey);
protected:
    QRectF boundingRect()const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
private:
    QString m_mimeKey;
    QRectF m_updateRect;
    QMap<QString,ClipItem*>m_clips;
    QList<ClipItem*> m_removeLists;

};


#endif //TRACKBODYITEM_H
