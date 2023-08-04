#include "mainwindow.h"
#include "../panes/cfilesystempane.h"
#include "../panes/cdependencypane.h"
#include "../panes/cinspectorpane.h"
#include "../editors/ctesteditor.h"
#include "DockAreaWidget.h"
#include "cnewassetdialog.h"
#include <QMenuBar>
#include <QLabel>
#include <QTimer>

void MainWindow::buildMenu()
{
    auto assetsMenu = menuBar()->addMenu("Assets");
    auto logMenu = menuBar()->addMenu("Log");
    /*auto preferencesMenu = */menuBar()->addMenu("Preferences");


    auto clearLogAction = logMenu->addAction(style()->standardIcon(QStyle::SP_MessageBoxCritical), tr("&Clear Log"));
    connect(clearLogAction, &QAction::triggered, LogPane, &CLogPane::clearLog);

    QAction* newAssets = assetsMenu->addAction("Create New Asset");
    connect(newAssets, &QAction::triggered, this, [this]() {
       auto newAssetDialog = new CNewAssetDialog(this);
       newAssetDialog->exec();
       delete newAssetDialog;
    });


    QAction* saveAssets = assetsMenu->addAction("Save All Assets");
    connect(saveAssets, &QAction::triggered, this, []() {
       for(auto& i : AssetGraph->GetAllAssets()) {
           if(auto asset = i.lock(); asset) {
               asset->saveMeta();
           }
       }
       AssetGraph->SaveLinksMeta();
    });


    QAction* loadAllAssets = assetsMenu->addAction("Load All Assets");
    connect(loadAllAssets, &QAction::triggered, this, []() {
        QDirIterator it(WorkingDirectory->path(), { "*.meta" }, QDir::Files, QDirIterator::Subdirectories);
        while(it.hasNext()) {
            QString metaPath = it.next();
            if(QFileInfo(metaPath).fileName().startsWith("$")) continue; // "$" metafiles are handled separately

            *LogPane << QString("Attempting load asset: %0").arg(metaPath);
            metaPath.chop(5 /* ".meta" */);
            AssetGraph->LoadAsset(
                new CAsset(WorkingDirectory->relativeFilePath(metaPath))
            );
        }
        AssetGraph->LoadLinksMeta();
    });
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setGeometry(700, 300, 1200, 800);
    buildMenu();

    dockManager = new ads::CDockManager(this);
    //dockManager->setStyleSheet("");

    CTestEditor* testEditor = new CTestEditor();
    ads::CDockWidget* dockWidget3 = new ads::CDockWidget("Editor");
    dockWidget3->setWidget(testEditor);
    ads::CDockAreaWidget* CentralDockArea = dockManager->setCentralWidget(dockWidget3);
    CentralDockArea->setAllowedAreas(ads::DockWidgetArea::OuterDockAreas);

    CFileSystemPane* fsView = new CFileSystemPane();
    ads::CDockWidget* dockWidget = new ads::CDockWidget("Filesystem");
    dockWidget->setWidget(fsView);
    dockManager->addDockWidget(ads::LeftDockWidgetArea, dockWidget);

    CDependencyPane* depView = new CDependencyPane();
    ads::CDockWidget* dockWidget1 = new ads::CDockWidget("Dependencies");
    dockWidget1->setWidget(depView);
    dockManager->addDockWidget(ads::RightDockWidgetArea, dockWidget1);

    ads::CDockWidget* dockWidget2 = new ads::CDockWidget("Log");
    dockWidget2->setWidget(LogPane);
    dockManager->addDockWidget(ads::BottomDockWidgetArea, dockWidget2);

    inspectorPane = new CInspectorPane();
    ads::CDockWidget* dockWidget4 = new ads::CDockWidget("Inspector");
    dockWidget4->setWidget(inspectorPane);
    dockManager->addDockWidget(ads::RightDockWidgetArea, dockWidget4);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *ev = static_cast<QMouseEvent *>(event);

        auto pos = dockManager->mapFromGlobal(ev->globalPosition()).toPoint();
        auto obj = dockManager->childAt(pos);
        while(obj && !dynamic_cast<IHasProperties*>(obj)) {
            obj = obj->parentWidget();
        }
        if(obj) {
            auto hasProps = dynamic_cast<IHasProperties*>(obj);
            if(hasProps) {
                inspectorPane->setInspectedObject(hasProps->getPropertiesObject(ev->globalPosition().toPoint()));
            }
        }

    }
    return QObject::eventFilter(watched, event);
}


