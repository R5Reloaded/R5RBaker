#include "windows/mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QStandardPaths>


QString* WorkingDirectory;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // initialize the globals that arent part of a CType
    WorkingDirectory = new QString(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    LogPane = new CLogPane();
    AssetGraph = new CAssetGraph();

    MainWindow w;
    w.show();

    a.installEventFilter(&w);

    return a.exec();
}
