#include "mainwindow.h"
#include "../panes/cfilesystempane.h"
#include "../panes/cdependencypane.h"
#include "../panes/cinspectorpane.h"
#include "../editors/ctesteditor.h"
#include "DockAreaWidget.h"
#include <QMenuBar>
#include <QLabel>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    dockManager = new ads::CDockManager(this);

    menuBar()->addMenu("File");
    menuBar()->addMenu("Edit");
    menuBar()->addMenu("Preferences");

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


