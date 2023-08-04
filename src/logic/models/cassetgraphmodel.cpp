#include "cassetgraphmodel.h"



CAssetGraphModel::CAssetGraphModel(QObject *parent)
    : QAbstractItemModel{parent}
{
    connect(AssetGraph, &CAssetGraph::VirtualGraphAboutToBeRebuilt, this, &CAssetGraphModel::beginResetModel);
    connect(AssetGraph, &CAssetGraph::VirtualGraphRebuilt, this, &CAssetGraphModel::endResetModel);

}

QModelIndex CAssetGraphModel::index(int row, int column, const QModelIndex &parent) const
{
    CGraphItem* parentItem;

    if(!parent.isValid())
        parentItem = AssetGraph->VirtualGraph.get();
    else
        parentItem = static_cast<CGraphItem*>(parent.internalPointer());


    return createIndex(row, column, parentItem->Children.at(row).get());
}

QModelIndex CAssetGraphModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    CGraphItem* item = static_cast<CGraphItem*>(child.internalPointer());
    CGraphItem* parent = item->Parent.lock().get();


    if (parent == nullptr)
        return QModelIndex();

    if (parent == AssetGraph->VirtualGraph.get())
        return QModelIndex();

    for(int i = 0; i < parent->Parent.lock()->Children.size(); i++) {
        if(parent->Parent.lock()->Children[i].get() == parent) {
            return createIndex(i, 0, parent);
        }
    }

    return QModelIndex();
}

int CAssetGraphModel::rowCount(const QModelIndex &parent) const
{

    if(!parent.isValid())
        return AssetGraph->VirtualGraph->Children.size();

    CGraphItem* item = static_cast<CGraphItem*>(parent.internalPointer());

    return item->Children.size();
}

int CAssetGraphModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QVariant CAssetGraphModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    switch(role) {
    case Qt::DisplayRole: {
        CGraphItem* item = static_cast<CGraphItem*>(index.internalPointer());
        if(auto Asset = item->Asset.lock(); Asset.get() != nullptr)
            return Asset->getName();
        else
            return "Asset Destroyed";
    }
    case Qt::DecorationRole: {
        return QApplication::style()->standardIcon(QStyle::SP_DirIcon);
        return QVariant();
    }
    default:
        return QVariant();
    }

    return QVariant();
}
