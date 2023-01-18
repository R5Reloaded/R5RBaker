#include "cassetgraph.h"

//
//
CAssetGraph* AssetGraph;
//
//


CAssetGraph::CAssetGraph(QObject *parent)
    : QObject{parent}
{
    connect(this, &CAssetGraph::GraphChanged, this, &CAssetGraph::RebuildVirtualGraph);
}

std::weak_ptr<CAsset> CAssetGraph::LoadAsset(CAsset *Asset)
{
    if(!Asset) return std::shared_ptr<CAsset>();

    std::shared_ptr<CAsset> ptr(Asset);
    LoadedAssets.push_back(ptr);

    emit GraphChanged();

    return std::weak_ptr(ptr);
}

void CAssetGraph::UnloadAsset(std::weak_ptr<CAsset> Asset)
{
    auto links = GetLinksForAsset(Asset);
    for (auto& link : links) {
        RemoveLink(link.Asset1, link.Asset2);
    }
    LoadedAssets.removeIf([Asset](std::shared_ptr<CAsset> ptr) {
        return ptr == Asset.lock();
    });
    emit GraphChanged();
}

void CAssetGraph::ClearGraph()
{
    LoadedAssets.clear();
    Links.clear();
    VirtualGraph.reset();
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
    for(auto& asset : LoadedAssets) {
        if(IsRootAsset(asset))
            Assets.push_back(asset);
    }
    return Assets;
}

QVector<std::weak_ptr<CAsset>> CAssetGraph::GetAllAssets() const
{
    QVector<std::weak_ptr<CAsset>> Assets;
    for(auto& asset : LoadedAssets) {
            Assets.push_back(asset);
    }
    return Assets;
}

bool CAssetGraph::IsRootAsset(std::weak_ptr<CAsset> Asset) const
{
    if(!Links.count(Asset)) return true;
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

bool CAssetGraph::AreLinked(std::weak_ptr<CAsset> Asset1, std::weak_ptr<CAsset> Asset2)
{
    auto links = GetLinksForAsset(Asset1);
    for (const auto& link : links) {
        if (link.Asset1.lock() == Asset2.lock() || link.Asset2.lock() == Asset2.lock()) {
            return true;
        }
    }
    return false;
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

