#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPlainTextEdit>
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    class ads::CDockManager *dockManager;
    class CInspectorPane* inspectorPane;

    void buildMenu();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


signals:


    // QWidget interface
protected:

    // QObject interface
public:
    bool eventFilter(QObject *watched, QEvent *event) override;
};
#endif // MAINWINDOW_H
