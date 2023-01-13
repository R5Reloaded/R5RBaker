#ifndef CDEPENDENCYTREEVIEW_H
#define CDEPENDENCYTREEVIEW_H

#include <QTreeView>
#include <QObject>
#include <QMenu>
#include <QActionGroup>

class CDependencyTreeView : public QTreeView
{
    Q_OBJECT
public:
    explicit CDependencyTreeView(QWidget *parent = nullptr);

    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
};

#endif // CDEPENDENCYTREEVIEW_H
