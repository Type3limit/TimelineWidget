//
// Created by 58226 on 2023/2/7.
//

#ifndef ANCHORHEADITEM_H
#define ANCHORHEADITEM_H


#include <QGraphicsObject>

class anchorheaditem: public QGraphicsObject
{
    Q_OBJECT
public:
    explicit anchorheaditem(QGraphicsItem* object = nullptr);

    void forceUpdate();
    QRectF boundingRect()const override;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event)override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event)override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent * event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event)override;
public slots:
    void OnTimelinePosChanged(ulong pos);
signals:
    void ReachEdge(int direction = 1);
private:
    volatile bool m_bIsStartDrag = false;
};


#endif //ANCHORHEADITEM_H
