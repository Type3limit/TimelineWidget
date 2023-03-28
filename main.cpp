#include <QApplication>
#include <QPushButton>
#include "mainwindow.h"
#include "MimeData/trackmime.h"
#include "intervalwatcher.h"

int main(int argc, char *argv[])
{
    static const char ENV_VAR_QT_DEVICE_PIXEL_RATIO[] = "QT_DEVICE_PIXEL_RATIO";
    if (!qEnvironmentVariableIsSet(ENV_VAR_QT_DEVICE_PIXEL_RATIO)
        && !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR")
        && !qEnvironmentVariableIsSet("QT_SCALE_FACTOR")
        && !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS")) {
        QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    }
    QApplication a(argc, argv);
    MainWindow window;
    window.show();
#ifdef QT_DEBUG
    ///序列化测试
//    intervalwatcher sp;
//    sp.start();
//    TrackMime data {"json序列化中文测试",1,SpecificType::Sticker,true,false};
//    S_STREAM ss;
//    TO_JSON(ss,data);
//    sp.stop();
//    qDebug()<<ss.str().c_str();
//    qDebug()<<QString::number(sp.milliSecond())+"ms after track serialize ";
//    sp.start();
//    TrackMime data2;
//    FROM_JSON(data2,ss.str().c_str());
//    qDebug()<<data2.toString();
//    sp.stop();
//    qDebug()<<QString::number(sp.milliSecond())+"ms after track deserialize";
#endif
    return QApplication::exec();
}
