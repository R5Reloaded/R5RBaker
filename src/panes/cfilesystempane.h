#ifndef CFILESYSTEMPANEL_H
#define CFILESYSTEMPANEL_H

#include <QFileSystemModel>
#include <QTreeView>
#include <QWidget>
#include <QLayout>
#include <QToolBar>

class CFileSystemPane : public QWidget
{
    Q_OBJECT

private:
    QFileSystemModel* fileSystemModel;
    QVBoxLayout* vbox;
    QTreeView* treeView;
    QToolBar* toolbar;
public:
    explicit CFileSystemPane(QWidget *parent = nullptr);

signals:

public slots:
};

#endif // CFILESYSTEMPANEL_H
