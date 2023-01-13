#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "DockManager.h"
#include "DockAreaWidget.h"
#include <QPlainTextEdit>
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    ads::CDockManager *dockManager;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


signals:

};
#endif // MAINWINDOW_H
