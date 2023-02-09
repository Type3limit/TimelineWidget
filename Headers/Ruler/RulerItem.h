//
// Created by 58226 on 2023/2/7.
//

#ifndef RULERITEM_H
#define RULERITEM_H


#include <QGraphicsItem>
#include "ExtensionMethods.h"
class RulerItem: public QGraphicsItem
{
public:
    explicit RulerItem(QGraphicsObject* parent = nullptr): QGraphicsItem(parent)
    {

    }
    QRectF boundingRect()const override;
    void OnLenthChange();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
private:
    QVector<QLine> m_lines;
    QVector<QString>m_pointText;
    int m_DrawStart;
    int m_DrawEnd;
};


#endif //RULERITEM_H
