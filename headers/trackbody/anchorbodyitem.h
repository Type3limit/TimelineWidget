//
// Created by 58226 on 2023/2/7.
//

#ifndef ANCHORBODYITEM_H
#define ANCHORBODYITEM_H


#include <QGraphicsItem>
class AnchorBodyItem: public QGraphicsObject
{
    Q_OBJECT
public:
    AnchorBodyItem(QGraphicsItem* parent = nullptr);
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
    void onTimelinePosChanged(ulong pos);

private:
    volatile bool m_bIsStartDrag = false;
};


#endif //ANCHORBODYITEM_H
