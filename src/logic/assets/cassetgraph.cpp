#include "cassetgraph.h"

CAssetGraph::CAssetGraph(QObject *parent)
    : QObject{parent}
{
    connect(this, &CAssetGraph::GraphChanged, this, &CAssetGraph::RebuildVirtualGraph);
}

std::weak_ptr<CAsset> CAssetGraph::LoadAsset(CAsset *Asset)
{
    std::shared_ptr<CAsset> ptr(Asset);
    LoadedAssets.push_back(ptr);
    //connect(Asset, &QObject::propertyChanged, this, &CAssetGraph::LinksChanged);
    return std::weak_ptr(ptr);
}

void CAssetGraph::UnloadAsset(std::weak_ptr<CAsset> Asset)
{
    LoadedAssets.removeIf([Asset](std::shared_ptr<CAsset> ptr) {
        return ptr == Asset.lock();
    });
    emit GraphChanged();
}


QVector<CLink> CAssetGraph::GetLinksForAsset(std::weak_ptr<CAsset> Asset) const
{
    if(!Links.count(Asset)) return QVector<CLink>();
    return Links.at(Asset);
}

QVector<CLink> CAssetGraph::GetChildrenLinksForAsset(std::weak_ptr<CAsset> Asset) const
{
    if(!Links.count(Asset)) return QVector<CLink>();
    QVector<CLink> Result;
    for(auto& link : Links.at(Asset)) {
        if(link.Direction == CLink::Direction::PARENT_TO_CHILD)
            Result.push_back(link);
    }
    return Result;
}

QVector<CLink> CAssetGraph::GetParentsLinksForAsset(std::weak_ptr<CAsset> Asset) const
{
    if(!Links.count(Asset)) return QVector<CLink>();
    QVector<CLink> Result;
    for(auto& link : Links.at(Asset)) {
        if(link.Direction == CLink::Direction::CHILD_TO_PARENT)
            Result.push_back(link);
    }
    return Result;
}

QVector<std::weak_ptr<CAsset>> CAssetGraph::GetRootAssets() const
{
    QVector<std::weak_ptr<CAsset>> Assets;

    for(auto& kv : Links) {
        if(IsRootAsset(kv.first))
            Assets.push_back(kv.first);
    }

    return Assets;
}

bool CAssetGraph::IsRootAsset(std::weak_ptr<CAsset> Asset) const
{
    if(!Links.count(Asset)) return false;
    for(auto& link : Links.at(Asset)) {
        if(link.Direction == CLink::Direction::CHILD_TO_PARENT)
            return false;
    }
    return true;
}


void CAssetGraph::AddLink(std::weak_ptr<CAsset> Parent, std::weak_ptr<CAsset> Child)
{
    CLink forwardLink = CLink{Parent, Child, CLink::Direction::PARENT_TO_CHILD};
    CLink backLink = CLink{Child, Parent, CLink::Direction::CHILD_TO_PARENT};

    Links[Parent].emplaceBack(forwardLink);
    Links[Child].emplaceBack(backLink);

    emit GraphChanged();
}

void CAssetGraph::RemoveLink(std::weak_ptr<CAsset> Asset1, std::weak_ptr<CAsset> Asset2)
{
    Links[Asset1].removeIf([&](CLink Link) {
        return Link.Asset2.lock() == Asset1.lock() || Link.Asset2.lock() == Asset2.lock();
    });
    Links[Asset2].removeIf([&](CLink Link) {
        return Link.Asset2.lock() == Asset1.lock() || Link.Asset2.lock() == Asset2.lock();
    });

    emit GraphChanged();
}

void CAssetGraph::RebuildVirtualGraph() {
    *LogPane << "Rebuilding Virtual Graph";

    for (auto &p : VirtualGraph->Children) {
      p.reset();
    }

    VirtualGraph->Children.clear();


    emit VirtualGraphAboutToBeRebuilt();
    VirtualGraph->Children.clear();
    for(std::weak_ptr<CAsset>& asset : GetRootAssets()) {
        VirtualGraph->Children.emplace_back(MakeGraphItemForAsset(asset, VirtualGraph));
    }
    emit VirtualGraphRebuilt();
    *LogPane << "Virtual Graph Rebuilt";
}

std::shared_ptr<CGraphItem> CAssetGraph::MakeGraphItemForAsset(std::weak_ptr<CAsset> Asset, std::shared_ptr<CGraphItem> Parent) {
    std::shared_ptr item = std::make_shared<CGraphItem>();

    item->Asset = Asset;
    item->Parent = Parent;

    for(auto& link : Links[Asset]) {
        if(link.Direction == CLink::Direction::PARENT_TO_CHILD) {
            std::shared_ptr<CGraphItem> child = MakeGraphItemForAsset(link.child(), item);
            if(child)
                item->Children.emplace_back(child);
        }
    }

    return item;
}

CAssetGraph* AssetGraph = new CAssetGraph();
