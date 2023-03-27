//
// Created by 58226 on 2023/2/6.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "intervalwatcher.h"

MainWindow::MainWindow(QWidget *parent)
    :
    QWidget(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    intervalwatcher iw;
    iw.start();
    sstream ss;
    TO_JSON( ss,ui->widget->getTimeMime());
    //qDebug()<<ss.str().c_str();
    ExtensionMethods::QStringExtension::writeAllText("./test.json", QString::fromStdString(ss.str()));
    iw.stop();
    qDebug()<<iw.milliSecond()<<"ms after timeline serialize";
    delete ui;
}
void MainWindow::showEvent(QShowEvent *event)
{
    //ui->widget->setFrameTick(25);
    QWidget::showEvent(event);
    //for test ,not necessary
    if(!isFirstShow)
        return;
    intervalwatcher iw;
    iw.start();
    isFirstShow = false;
    if(ExtensionMethods::QStringExtension::isFileExist("./test.json")&&
        ui->widget->buildFromJsonFile("./test.json"))
    {
        iw.stop();
        qDebug()<<iw.milliSecond()<<"ms after show event finished";
        return;
    }


    QString ids[3];
    for(int i =0;i<3;i++)
    {
        ids[i] = QUuid::createUuid().toString().remove("{").remove("}").remove("-");
        ui->widget->addTrack(ids[i],(SpecificType)i%3+1,-1);
    }

    for(int i =0 ;i<10;i++)
    {
        for(int j = 0;j<3;j++)
        {
            ClipMime clip(QUuid::createUuid().toString().remove("{").remove("}").remove("-"), ids[j%3], (ulong)i*1200+j*100, (ulong)1200, (static_cast<SpecificType>(i%3+1)));
            ui->widget->addClip(ids[j%3],clip);
        }
    }
    ui->widget->adaptTimelineLength();
    iw.stop();
    qDebug()<<iw.milliSecond()<<"ms after show event finished";
}
