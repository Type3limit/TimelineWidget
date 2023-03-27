//
// Created by 58226 on 2023/2/16.
//

#ifndef TRACKSELECTIONITEM_H
#define TRACKSELECTIONITEM_H


#include <QGraphicsItem>
class trackselectionitem: public QGraphicsItem
{
private:
    QPointF m_startPos;
    QPointF m_endPos;
public:
    trackselectionitem()=default;
    QRectF boundingRect() const override;
    void setPos(QPointF start,QPointF end);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    friend class trackbodydrawingview;
};


#endif //TRACKSELECTIONITEM_H
