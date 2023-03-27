//
// Created by 58226 on 2023/2/6.
//

#ifndef TICKDRAWINGVIEW_H
#define TICKDRAWINGVIEW_H


#include <QGraphicsView>
#include <QWeakPointer>
#include "tickitem.h"
#include "selfcontainedscenesview.h"
class TickDrawingView: public SelfContainedSceneView
{
    Q_OBJECT
public:
    explicit TickDrawingView(QWidget* parent = nullptr)
    {
        m_tickItem = new TickItem();
        m_scene->addItem(m_tickItem);
    }
    ~TickDrawingView()
    {
        SAFE_DELETE(m_tickItem);
    }
protected:

    TickItem* m_tickItem = nullptr;
};


#endif //TICKDRAWINGVIEW_H
