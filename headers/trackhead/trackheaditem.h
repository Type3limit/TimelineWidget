//
// Created by 58226 on 2023/2/7.
//

#ifndef TRACKHEADITEM_H
#define TRACKHEADITEM_H


#include <QGraphicsItem>
#include "MimeData/trackmime.h"
class TrackHeadItem: public QGraphicsItem
{
public:
    explicit TrackHeadItem(QGraphicsItem* parent);
    explicit TrackHeadItem(const TrackMime& curData, QGraphicsItem* parent= nullptr);
    ~TrackHeadItem();
    TrackMime getMimeData() const;
    void forceUpdate();
protected:
    QRectF boundingRect()const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
private:
    QString m_mimeKey;
public:
    bool m_isRemoved = false;

};


#endif //TRACKHEADITEM_H
