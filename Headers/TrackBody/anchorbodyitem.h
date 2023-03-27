//
// Created by 58226 on 2023/2/7.
//

#ifndef ANCHORBODYITEM_H
#define ANCHORBODYITEM_H


#include <QGraphicsItem>
class anchorbodyitem: public QGraphicsObject
{
    Q_OBJECT
public:
    anchorbodyitem(QGraphicsItem* parent = nullptr);
    void forceUpdate();
protected:
    QRectF boundingRect()const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event)override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
public slots:
    void OnTimelinePosChanged(ulong pos);

private:
    volatile bool m_bIsStartDrag = false;
};


#endif //ANCHORBODYITEM_H
