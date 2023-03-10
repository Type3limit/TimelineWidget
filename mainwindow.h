//
// Created by 58226 on 2023/2/6.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>


QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow: public QWidget
{
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void showEvent(QShowEvent *event) override;
private:
    Ui::MainWindow *ui;
    volatile bool isFirstShow = true;
};


#endif //MAINWINDOW_H
