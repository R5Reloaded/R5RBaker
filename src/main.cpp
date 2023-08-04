#include "logic/assets/types/crmdl.h"
#include "logic/assets/types/crpak.h"
#include "windows/mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QStandardPaths>
#include <QStyleFactory>


QDir* WorkingDirectory;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle(QStyleFactory::create("Fusion"));
    QPalette p = qApp->palette();
    p.setColor(QPalette::Window,QColor(53,53,53));
    p.setColor(QPalette::WindowText,Qt::white);
    p.setColor(QPalette::Disabled,QPalette::WindowText,QColor(127,127,127));
    p.setColor(QPalette::Base,QColor(42,42,42));
    p.setColor(QPalette::AlternateBase,QColor(66,66,66));
    p.setColor(QPalette::ToolTipBase,Qt::white);
    p.setColor(QPalette::ToolTipText,Qt::white);
    p.setColor(QPalette::Text,Qt::white);
    p.setColor(QPalette::Disabled,QPalette::Text,QColor(127,127,127));
    p.setColor(QPalette::Dark,QColor(35,35,35));
    p.setColor(QPalette::Shadow,QColor(20,20,20));
    p.setColor(QPalette::Button,QColor(53,53,53));
    p.setColor(QPalette::ButtonText,Qt::white);
    p.setColor(QPalette::Disabled,QPalette::ButtonText,QColor(127,127,127));
    p.setColor(QPalette::BrightText,Qt::red);
    p.setColor(QPalette::Link,QColor(42,130,218));
    p.setColor(QPalette::Highlight,QColor(42,130,218));
    p.setColor(QPalette::Disabled,QPalette::Highlight,QColor(80,80,80));
    p.setColor(QPalette::HighlightedText,Qt::white);
    p.setColor(QPalette::Disabled,QPalette::HighlightedText,QColor(127,127,127));

    qApp->setPalette(p);


    // initialize the globals that arent part of a CType
    WorkingDirectory = new QDir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    LogPane = new CLogPane();
    AssetGraph = new CAssetGraph();


    qRegisterMetaType<std::shared_ptr<CAsset>>("std::shared_ptr<CAsset>");
    qRegisterMetaType<std::weak_ptr<CAsset>>("std::weak_ptr<CAsset>");

    qRegisterMetaType<CAsset*>();
    qRegisterMetaType<CRPak*>();
    qRegisterMetaType<CRMdl*>();

    MainWindow w;
    w.show();

    a.installEventFilter(&w);

    return a.exec();
}
