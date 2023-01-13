#include "cdependencytreeview.h"

CDependencyTreeView::CDependencyTreeView(QWidget *parent) : QTreeView(parent)
{

}

void CDependencyTreeView::contextMenuEvent(QContextMenuEvent *event)
{
    QModelIndex index = indexAt(event->pos());

    if(!index.isValid())
        return;

    CGraphItem* item = static_cast<CGraphItem*>(index.internalPointer());

    QMenu *menu = nullptr;

    if(auto Asset = item->Asset.lock(); Asset.get() != nullptr) {
        menu = new QMenu(this);
        Asset->buildContextMenu(menu, item);
    }

    if(menu != nullptr) {
        menu->exec(event->globalPos());
        delete menu;
    }
}
