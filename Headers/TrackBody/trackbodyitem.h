//
// Created by 58226 on 2023/2/7.
//

#ifndef TRACKBODYITEM_H
#define TRACKBODYITEM_H


#include <QGraphicsItem>
#include "MimeData/trackmime.h"
#include "Clip/clipitem.h"
class trackbodyitem: public QGraphicsItem
{
public:
    explicit trackbodyitem(QGraphicsItem* parent= nullptr);
    explicit trackbodyitem(const trackmime& curData, QGraphicsItem* parent= nullptr);
    ~trackbodyitem();
    trackmime getMimeData() const;
    void forceUpdate();
public:
    void addClipItem(const QString& itemKey);
    void removeClipItem(const QString& itemKey);
    void updateClipItem(const QString& itemKey);
    clipitem* getClipItem(const QString& itemKey);
protected:
    QRectF boundingRect()const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)override;
private:
    QString m_mimeKey;
    QRectF m_updateRect;
    QMap<QString, clipitem*>m_clips;
    QList<clipitem*> m_removeLists;

};


#endif //TRACKBODYITEM_H
