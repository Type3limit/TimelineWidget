//
// Created by 58226 on 2023/2/7.
//

#ifndef TRACKHEADITEM_H
#define TRACKHEADITEM_H


#include <QGraphicsItem>
#include "MimeData/trackmime.h"
class trackheaditem: public QGraphicsItem
{
public:
    explicit trackheaditem(QGraphicsItem* parent);
    explicit trackheaditem(const trackmime& curData, QGraphicsItem* parent= nullptr);
    trackmime getMimeData() const;
    void forceUpdate();

protected:
    QRectF boundingRect()const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
private:
    QString m_mimeKey;
};


#endif //TRACKHEADITEM_H
