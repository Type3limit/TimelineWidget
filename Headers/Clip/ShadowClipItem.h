//
// Created by 58226 on 2023/2/14.
//

#ifndef SHADOWCLIPITEM_H
#define SHADOWCLIPITEM_H


#include <QGraphicsItem>
class ShadowClipItem: public QGraphicsItem
{
private:
    QString m_key;
    QRectF m_drawRect;
public:
    explicit ShadowClipItem(const QString& key):m_key(key){
    }
    void setDrawRect(const QRectF&rec);
    void forceUpdate();
    QRectF boundingRect() const override;
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
};


#endif //SHADOWCLIPITEM_H
