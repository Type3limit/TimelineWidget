//
// Created by 58226 on 2023/2/7.
//

#ifndef TRACKBODYITEM_H
#define TRACKBODYITEM_H


#include <QGraphicsItem>
#include "MimeData/TrackMime.h"
class TrackBodyItem: public QGraphicsItem
{
public:
    explicit TrackBodyItem(QGraphicsItem* parent= nullptr);
    explicit TrackBodyItem(const TrackMime& curData);
    TrackMime getMimeData() const;
    void forceUpdate();
protected:
    QRectF boundingRect()const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
private:
    QString m_mimeKey;
};


#endif //TRACKBODYITEM_H
