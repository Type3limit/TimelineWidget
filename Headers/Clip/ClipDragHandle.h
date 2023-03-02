//
// Created by 58226 on 2023/3/1.
//

#ifndef CLIPDRAGHANDLE_H
#define CLIPDRAGHANDLE_H


#include <QGraphicsItem>
class ClipDragHandle: public QGraphicsItem
{
private:
    QRectF m_drawingRect;
    bool m_isAddToScene = false;
public:
    ClipDragHandle(QGraphicsItem* parent=nullptr){
        setFlags(ItemSendsGeometryChanges);
        setAcceptHoverEvents(true);
        //setAcceptedMouseButtons(Qt::MouseButton::LeftButton);
        setZValue(100);
    }
    void setDrawingRect(QRectF rect)
    {
        m_drawingRect = rect;
    }
    void forceUpdate()
    {
        prepareGeometryChange();
        update();
    }
    void setAddToScene(bool added)
    {
        m_isAddToScene = added;
    }
    bool isAddToScene() const
    {
        return m_isAddToScene;
    }
protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};


#endif //CLIPDRAGHANDLE_H
