//
// Created by 58226 on 2023/2/6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    :
    QWidget(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //外部给入一个最大长度来确定时间线绘制区
    ui->widget->setMaxDuration(2500);
    //指针位置
    ui->widget->setCurPos(150);
    //添加轨道
    ui->widget->addTrack(QUuid::createUuid(),SpecificType::Sticker,-1);
    ui->widget->addTrack(QUuid::createUuid(),SpecificType::Subtitle,3);
    ui->widget->addTrack(QUuid::createUuid(),SpecificType::Effect,0);
}

MainWindow::~MainWindow()
{
    delete ui;
}
