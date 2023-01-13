#include "casset.h"
#include "../../windows/cassetlinksdialog.h"

CAsset::CAsset(QString Name, QObject *parent)
    : QObject{parent}
{
    srand(time(nullptr));
    this->Name = Name;
}

void CAsset::buildContextMenu(QMenu *Menu, CGraphItem *associatedItem) {
    auto hierarchyActions = Menu->addMenu("Hierarchy");

    {
        auto unlinkFromParentAction = hierarchyActions->addAction("Unlink From Parent");

        connect(unlinkFromParentAction, &QAction::triggered, Menu, [associatedItem]() {
            if(auto parent = associatedItem->Parent.lock(); parent != AssetGraph->VirtualGraph) {
                AssetGraph->RemoveLink(associatedItem->Asset, parent->Asset);
            }
        });
    }
    {
        auto relationsMenuAction = hierarchyActions->addAction("View/Edit Links");
        connect(relationsMenuAction, &QAction::triggered, Menu, [this]() {
            (new CAssetLinksDialog(weak_from_this(), nullptr))->show();
        });
    }


    _buildContextMenu(Menu);
}
