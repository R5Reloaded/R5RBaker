#include "cfilesystempane.h"

#include <QStandardPaths>


CFileSystemPane::CFileSystemPane(QWidget *parent)
    : QWidget{parent}
{
    vbox = new QVBoxLayout(this);
    fileSystemModel = new QFileSystemModel(this);
    treeView = new QTreeView(this);
    toolbar = new QToolBar(this);

    toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toolbar->setIconSize(QSize(16, 16));
    const QIcon importIcon = style()->standardIcon(QStyle::SP_ArrowUp);
    QAction *importAct = new QAction(importIcon, tr("&Import..."), this);
    importAct->setToolTip(tr("Import a file"));

    toolbar->addAction(importAct);

    vbox->addWidget(toolbar);
    vbox->addWidget(treeView);

    QString AppData = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir AppDataDir(AppData);

    if(!AppDataDir.exists()) {
        AppDataDir.mkpath(".");
    }


    fileSystemModel->setRootPath(QDir::rootPath());
    fileSystemModel->setReadOnly(false);

    vbox->addWidget(treeView);
    treeView->setModel(fileSystemModel);
    treeView->setRootIndex(fileSystemModel->index(AppData));
}
