//
// Created by 58226 on 2023/2/16.
//

#ifndef TRACKSELECTIONITEM_H
#define TRACKSELECTIONITEM_H


#include <QGraphicsItem>
class TrackSelectionItem: public QGraphicsItem
{
private:
    QPointF m_startPos;
    QPointF m_endPos;
public:
    TrackSelectionItem()=default;
    QRectF boundingRect() const override;
    void setPos(QPointF start,QPointF end);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};


#endif //TRACKSELECTIONITEM_H
