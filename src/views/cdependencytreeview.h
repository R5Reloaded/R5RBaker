#ifndef CDEPENDENCYTREEVIEW_H
#define CDEPENDENCYTREEVIEW_H

#include <QTreeView>
#include <QObject>
#include <QMenu>
#include <QActionGroup>

class CDependencyTreeView : public QTreeView, public IHasProperties
{
    Q_OBJECT
public:
    explicit CDependencyTreeView(QWidget *parent = nullptr);

    // QWidget interface
protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

    // IHasProperties interface
public:
    QObject *getPropertiesObject(QPoint pos) override;

    // IHasProperties interface
public:
    void propertiesUpdated() override;
};

#endif // CDEPENDENCYTREEVIEW_H
