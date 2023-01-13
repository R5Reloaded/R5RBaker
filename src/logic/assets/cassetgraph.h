#ifndef CASSETGRAPH_H
#define CASSETGRAPH_H

#include <QObject>


struct CLink {
    std::weak_ptr<CAsset> Asset1;
    std::weak_ptr<CAsset> Asset2;

    enum class Direction {
        PARENT_TO_CHILD,
        CHILD_TO_PARENT
    } Direction;


    std::weak_ptr<CAsset> parent() const {
        if(Direction == Direction::PARENT_TO_CHILD)
            return Asset1;
        else
            return Asset2;
    }

    std::weak_ptr<CAsset> child() const {
        if(Direction == Direction::PARENT_TO_CHILD)
            return Asset2;
        else
            return Asset1;
    }

    static CLink fromInverted(const CLink& current) {
        return CLink{
            current.Asset2,
            current.Asset1,
            current.Direction ==
                    Direction::PARENT_TO_CHILD?
                        Direction::CHILD_TO_PARENT :
                        Direction::PARENT_TO_CHILD
        };
    }
};

struct CGraphItem {
    std::weak_ptr<CGraphItem> Parent;
    std::weak_ptr<CAsset> Asset;
    std::vector<std::shared_ptr<CGraphItem>> Children;
};

class CAssetGraph : public QObject
{
    Q_OBJECT
public:
    explicit CAssetGraph(QObject *parent = nullptr);

    QVector<std::shared_ptr<CAsset>> LoadedAssets;
    std::map<std::weak_ptr<CAsset>, QVector<CLink>, std::owner_less<std::weak_ptr<CAsset>>> Links;
    std::shared_ptr<CGraphItem> VirtualGraph = std::make_shared<CGraphItem>();


    std::weak_ptr<CAsset> LoadAsset(CAsset* Asset);
    void UnloadAsset(std::weak_ptr<CAsset> Asset);



    QVector<CLink> GetLinksForAsset(std::weak_ptr<CAsset> Asset) const;
    QVector<CLink> GetChildrenLinksForAsset(std::weak_ptr<CAsset> Asset) const;
    QVector<CLink> GetParentsLinksForAsset(std::weak_ptr<CAsset> Asset) const;


    QVector<std::weak_ptr<CAsset>> GetRootAssets() const;
    bool IsRootAsset(std::weak_ptr<CAsset> Asset) const;


    void AddLink(std::weak_ptr<CAsset> Parent, std::weak_ptr<CAsset> Child);
    void RemoveLink(std::weak_ptr<CAsset> Asset1, std::weak_ptr<CAsset> Asset2);


    std::shared_ptr<CGraphItem> MakeGraphItemForAsset(std::weak_ptr<CAsset> Asset, std::shared_ptr<CGraphItem> Parent);

signals:
    void LinksChanged();
    void VirtualGraphAboutToBeRebuilt();
    void VirtualGraphRebuilt();

private:
    Q_PROPERTY(std::map<std::weak_ptr<CAsset>, QVector<CLink>> Links NOTIFY LinksChanged)

public slots:
    void RebuildVirtualGraph();
};

extern CAssetGraph* AssetGraph;

#endif // CASSETGRAPH_H