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

    QVector<QString> List;
public:
    explicit CLogPane(QWidget *parent = nullptr);

    CLogPane& operator<<(QVariant entry);

    void clearLog();
signals:

};

#endif // CLOGPANE_H
