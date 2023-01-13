#ifndef CLOGPANE_H
#define CLOGPANE_H

#include <QWidget>
#include <QListView>
#include <QStringListModel>
#include <QBoxLayout>
#include <QToolBar>


class CLogPane : public QWidget
{
    Q_OBJECT

    QListView* ListView;
    QStringListModel* ListModel;
    QToolBar* ToolBar;

    QVector<QString> List;
public:
    explicit CLogPane(QWidget *parent = nullptr);

    CLogPane& operator<<(QVariant entry);

signals:

};

extern CLogPane* LogPane;

#endif // CLOGPANE_H
