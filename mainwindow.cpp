//
// Created by 58226 on 2023/2/6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "IntervalWatcher.h"

MainWindow::MainWindow(QWidget *parent)
    :
    QWidget(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //外部给入一个最大长度来确定时间线绘制区
    ui->widget->setMaxDuration(20000);
    //指针位置
    ui->widget->setCurPos(150);

    QString ids[3];
    for(int i =0;i<3;i++)
    {
        ids[i] = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
        ui->widget->addTrack(ids[i],(SpecificType)i%3+1,-1);
    }


    for(int i =0 ;i<1;i++)
    {
        for(int j = 0;j<3;j++)
        {
            ClipMime clip(QUuid::createUuid().toString().remove("{").remove("}").remove("-"),ids[j%3],(ulong)i*200+i*100,(ulong)200,(static_cast<SpecificType>(i%3+1)));
            ui->widget->addClip(j,clip);
        }
    }

    ui->widget->setFrameTick(35);
}

MainWindow::~MainWindow()
{
    IntervalWatcher iw;
    iw.start();
    sstream ss;
    TO_JSON( ss,ui->widget->getTimeMime());
    qDebug()<<ss.str().c_str();
    iw.stop();
    qDebug()<<iw.milliSecond()<<"ms after timeline serialize";
    delete ui;
}
