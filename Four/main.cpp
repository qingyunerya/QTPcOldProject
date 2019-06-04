#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QFont>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSize size=a.primaryScreen()->size();
    QFont font;
    font.setPixelSize(12);
    a.setFont(font);
    qDebug("%d,%d",size.width(),size.height());
    MainWindow w;
    w.setresize(size.width(),size.height());
    w.show();
    return a.exec();
}
