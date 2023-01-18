#include "cfilesystempane.h"

#include <QStandardPaths>


CFileSystemPane::CFileSystemPane(QWidget *parent)
    : QWidget{parent}
{
    vbox = new QVBoxLayout(this);
    fileSystemModel = new QFileSystemModel(this);
    treeView = new QTreeView(this);

    vbox->addWidget(treeView);

    QString AppData = WorkingDirectory->path();
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
