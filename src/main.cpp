#include "windows/mainwindow.h"

#include <QApplication>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogPane = new CLogPane();

    MainWindow w;
    w.show();

    return a.exec();
}