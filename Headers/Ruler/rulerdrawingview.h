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
class rulerdrawingview: public SelfContainedSceneView
{
    Q_OBJECT
public:
    explicit rulerdrawingview(QWidget* parent = nullptr);
    ~rulerdrawingview()
    {
        SAFE_DELETE(m_ruler);
    }
    void rulerUpdate();
    void setDrawingAreaSize(int width, int height) override;
protected:
    void mousePressEvent(QMouseEvent* event)override;
    void mouseMoveEvent(QMouseEvent* evt)override;

private:
    ruleritem* m_ruler = nullptr;
    anchorheaditem* m_anchorHead = nullptr;
    public slots:
    void OnTrackBodyScroll(int pos);
    signals:
    void OnAnchorHeadReachEdge(int direction);
};


#endif //RULERDRAWINGVIEW_H
