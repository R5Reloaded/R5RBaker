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
        item->buildMenu(menu);
        Asset->buildMenu(menu);
    }

    if(menu != nullptr) {
        menu->exec(event->globalPos());
        delete menu;
    }
}

QObject *CDependencyTreeView::getPropertiesObject(QPoint pos)
{
    pos = this->mapFromGlobal(pos);
    auto idx = indexAt(pos);

    if(idx.isValid()) {
        auto hasProps = static_cast<CGraphItem*>(idx.internalPointer());
        if(hasProps) {
            return hasProps->getPropertiesObject(pos);
        }
    }
    return nullptr;
}
