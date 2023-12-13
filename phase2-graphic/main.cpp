#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("暗黑版泡泡堂");//改名
    w.show();
    return a.exec();
}
