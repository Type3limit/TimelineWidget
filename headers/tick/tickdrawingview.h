//
// Created by 58226 on 2023/2/6.
//

#ifndef TICKDRAWINGVIEW_H
#define TICKDRAWINGVIEW_H


#include <QGraphicsView>
#include <QWeakPointer>
#include "tickitem.h"
#include "selfcontainedscenesview.h"
class tickdrawingview: public SelfContainedSceneView
{
    Q_OBJECT
public:
    explicit tickdrawingview(QWidget* parent = nullptr)
    {
        m_tickItem = new tickitem();
        m_scene->addItem(m_tickItem);
    }
    ~tickdrawingview()
    {
        SAFE_DELETE(m_tickItem);
    }
protected:

    tickitem* m_tickItem = nullptr;
};


#endif //TICKDRAWINGVIEW_H
