//
// Created by 58226 on 2023/2/6.
//

#ifndef SELFCONTAINEDSCENESVIEW_H
#define SELFCONTAINEDSCENESVIEW_H
#include <QGraphicsView>
#include "ExtensionMethods.h"
#include <QScrollBar>
class SelfContainedSceneView: public QGraphicsView
{
    Q_OBJECT
public:
    explicit SelfContainedSceneView(QWidget* parent = nullptr)
    {

        setScene( getScene());
        setStyleSheet("    border:  1px solid rgb(0,0,0);\n"
                      "    background-color: #28282E;\n"
                      "    font-family: \"Cascadia Code\";");//"    border-radius: 5px;\n"

        horizontalScrollBar()->setStyleSheet(
            " QScrollBar:horizontal\n"
            "    {\n"
            "        height: 9px;\n"
            "        margin: 0px 0px 0px 0px;\n"
            "        border: 0px transparent #2A2929;\n"
            "        border-radius: 5px;\n"
            "        background-color:#000000;    \n"
            "    }"
            " QScrollBar::handle:horizontal\n"
            "    {\n"
            "        background-color: #515151;      \n"
            "        min-width: 2px;\n"
            "        border-radius: 5px;\n"
            "    }"
            " QScrollBar::add-line:horizontal\n"
            "    {\n"
            "        margin: 0px;\n"
            "        border-image: url(:/qss_icons/rc/right_arrow_disabled.png);\n"
            "        width: 9px;\n"
            "        height: 9px;\n"
//            "        subcontrol-position: right;\n"
//            "        subcontrol-origin: margin;\n"
            "    }"
            " QScrollBar::sub-line:horizontal\n"
            "    {\n"
            "        margin: 0px;\n"
            "        border-image: url(:/qss_icons/rc/left_arrow_disabled.png);\n"
            "        height: 9px;\n"
            "        width: 9px;\n"
//            "        subcontrol-position: left;\n"
//            "        subcontrol-origin: margin;\n"
            "    }"
            " QScrollBar::add-line:horizontal:hover,QScrollBar::add-line:horizontal:on\n"
            "    {\n"
            "        border-image: url(:/qss_icons/rc/right_arrow.png);\n"
            "        height: 9px;\n"
            "        width: 9px;\n"
//            "        subcontrol-position: right;\n"
//            "        subcontrol-origin: margin;\n"
            "    }"
            " QScrollBar::sub-line:horizontal:hover, QScrollBar::sub-line:horizontal:on\n"
            "    {\n"
            "        border-image: url(:/qss_icons/rc/left_arrow.png);\n"
            "        height: 9px;\n"
            "        width: 9px;\n"
//            "        subcontrol-position: left;\n"
//            "        subcontrol-origin: margin;\n"
            "    }"
            " QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:horizontal\n"
            "    {\n"
            "        background: none;\n"
            "    }"
            " QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal\n"
            "    {\n"
            "        background: none;\n"
            "    }\n"

        );
        verticalScrollBar()->setStyleSheet(" QScrollBar:vertical\n"
                                           "    {\n"
                                           "        background-color: #000000;\n"
                                           "        width: 9px;\n"
                                           "        margin: 0px;\n"
                                           "        border: 0px transparent;\n"
                                           "        border-radius: 5px;\n"
                                           "    }"
                                           "QScrollBar::handle:vertical\n"
                                           "    {\n"
                                           "        background-color: #515151;       \n"
                                           "        min-height: 2px;\n"
                                           "        border-radius: 5px;\n"
                                           "    }"
                                           "QScrollBar::sub-line:vertical\n"
                                           "    {\n"
                                           "        margin: 0px;\n"
                                           "        border-image: url(:/qss_icons/rc/up_arrow_disabled.png);\n"
                                           "        height: 9px;\n"
                                           "        width: 9px;\n"
                                           "        subcontrol-position: top;\n"
                                           "        subcontrol-origin: margin;\n"
                                           "    }"
                                           "QScrollBar::add-line:vertical\n"
                                           "    {\n"
                                           "        margin: 0px 0px 0px 0px;\n"
                                           "        border-image: url(:/qss_icons/rc/down_arrow_disabled.png);\n"
                                           "        height: 9px;\n"
                                           "        width: 9px;\n"
                                           "        subcontrol-position: bottom;\n"
                                           "        subcontrol-origin: margin;\n"
                                           "    }"
                                           "QScrollBar::sub-line:vertical:hover,QScrollBar::sub-line:vertical:on\n"
                                           "    {\n"
                                           "        border-image: url(:/qss_icons/rc/up_arrow.png);\n"
                                           "        height: 9px;\n"
                                           "        width: 9px;\n"
                                           "        subcontrol-position: top;\n"
                                           "        subcontrol-origin: margin;\n"
                                           "    }"
                                           "  QScrollBar::add-line:vertical:hover, QScrollBar::add-line:vertical:on\n"
                                           "    {\n"
                                           "        border-image: url(:/qss_icons/rc/down_arrow.png);\n"
                                           "        height: 9px;\n"
                                           "        width: 9px;\n"
                                           "        subcontrol-position: bottom;\n"
                                           "        subcontrol-origin: margin;\n"
                                           "    }"
                                           "QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical\n"
                                           "    {\n"
                                           "        background: none;\n"
                                           "    }"
                                           " QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical\n"
                                           "    {\n"
                                           "        background: none;\n"
                                           "    }");
    }
    ~SelfContainedSceneView()
    {
        SAFE_DELETE(m_scene);
    }
    ///设置画布宽高
    virtual void setDrawingAreaSize(int width, int height)
    {
        m_nWidth = width;
        m_nHeight = height;
        if(m_scene)
        {
            m_scene->setSceneRect(0,0,m_nWidth,m_nHeight);
        }
    }
    ///获取画布宽高
    virtual QSize getSizeofDrawingArea()
    {
        return {m_nWidth,m_nHeight};
    }
    ///获取可视区
    virtual const QRectF getViewPortRect() const
    {
        return mapToScene(viewport()->geometry()).boundingRect();
    }

    virtual QGraphicsScene* getScene()
    {
        m_scene = new QGraphicsScene(this);
        return m_scene;
    }
protected:
    QGraphicsScene* m_scene = nullptr;
    int m_nWidth = 0;
    int m_nHeight =0;
};
#endif //SELFCONTAINEDSCENESVIEW_H
