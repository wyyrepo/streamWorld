#include "system.h"
#include <QApplication>
#include <QTextCodec>
#include <QFont>
#include <QFontDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    //为程序安装第三方艺术字体，该字体存储在资源文件中。
    QFontDatabase::addApplicationFont("://方正粗活意简体.ttf");

    System system;

    return app.exec();
}
