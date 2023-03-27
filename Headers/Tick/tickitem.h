//
// Created by 58226 on 2023/2/6.
//

#ifndef TICKITEM_H
#define TICKITEM_H


#include <QGraphicsObject>
#include <QPaintEvent>
#include <QGraphicsScene>
#include "timelinedefination.h"
class tickitem: public QGraphicsObject
{
    Q_OBJECT
public:
    explicit tickitem(QGraphicsItem*parent = nullptr): QGraphicsObject(parent)
    {

    }
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF boundingRect() const override;

private :
    QString m_str{""};
};


#endif //TICKITEM_H
