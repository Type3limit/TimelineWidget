//
// Created by 58226 on 2023/2/6.
//

#ifndef RULERDRAWINGVIEW_H
#define RULERDRAWINGVIEW_H

#include <QGraphicsView>
#include "selfcontainedscenesview.h"
#include "ruleritem.h"
#include "anchorheaditem.h"
#include <QScrollBar>
class RulerDrawingView: public SelfContainedSceneView
{
    Q_OBJECT
public:
    explicit RulerDrawingView(QWidget* parent = nullptr);
    ~RulerDrawingView()
    {
        SAFE_DELETE(m_ruler);
    }
    void rulerUpdate();
    void setDrawingAreaSize(int width, int height) override;
protected:
    void mousePressEvent(QMouseEvent* event)override;
    void mouseMoveEvent(QMouseEvent* evt)override;

private:
    RulerItem* m_ruler = nullptr;
    AnchorHeadItem* m_anchorHead = nullptr;
    public slots:
    void onTrackBodyScroll(int pos);
    signals:
    void onAnchorHeadReachEdge(int direction);
};


#endif //RULERDRAWINGVIEW_H
