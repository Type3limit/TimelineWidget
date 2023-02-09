//
// Created by 58226 on 2023/2/6.
//

#ifndef TICKITEM_H
#define TICKITEM_H


#include <QGraphicsObject>
#include <QPaintEvent>
#include <QGraphicsScene>
#include "TimelineDefination.h"
class TickItem: public QGraphicsObject
{
    Q_OBJECT
public:
    explicit TickItem(QGraphicsItem*parent = nullptr): QGraphicsObject(parent)
    {

    }
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QRectF boundingRect() const override;

private :
    QString m_str{""};
};


#endif //TICKITEM_H
