#include "cassetlinksdialog.h"

#include <QDialogButtonBox>
#include <QGroupBox>
#include <QListWidget>

CAssetLinksDialog::CAssetLinksDialog(std::weak_ptr<CAsset> AssetPtr, QWidget *parent) : QDialog{parent}
{
    auto Asset = AssetPtr.lock().get();

    if(!Asset) {
        close();
        return;
    }

    setWindowTitle(QString("%1 Links").arg(Asset->Name));
    //setModal(true);

    auto vbox = new QVBoxLayout(this);
    {
        {
            auto childrenGroup = new QGroupBox("Children", this);
            vbox->addWidget(childrenGroup);

            auto vbox = new QVBoxLayout;
            childrenGroup->setLayout(vbox);

            auto childrenList = new QListWidget;
            vbox->addWidget(childrenList);

            auto refreshChildrenList = [Asset, AssetPtr, childrenList]() {
                childrenList->clear();
                for(auto& link : AssetGraph->GetLinksForAsset(AssetPtr)) {
                    if(link.parent().lock().get() == Asset && link.child().lock()) {
                        auto child = link.child().lock();
                        childrenList->addItem(QString("%1: [%2]").arg(child->Name, child->metaObject()->className()));
                    }
                }
            };
            refreshChildrenList();
            connect(AssetGraph, &CAssetGraph::LinksChanged, childrenList, [refreshChildrenList]() {
                refreshChildrenList();
            });
        }

        {
            auto parentsGroup = new QGroupBox("Parents", this);
            vbox->addWidget(parentsGroup);

            auto vbox = new QVBoxLayout;
            parentsGroup->setLayout(vbox);

            auto parentsList = new QListWidget;
            vbox->addWidget(parentsList);

            auto refreshParentsList = [Asset, AssetPtr, parentsList]() {
                parentsList->clear();
                for(auto& link : AssetGraph->GetLinksForAsset(AssetPtr)) {
                    if(link.child().lock().get() == Asset && link.parent().lock()) {
                        auto parent = link.parent().lock();
                        parentsList->addItem(QString("%1: [%2]").arg(parent->Name, parent->metaObject()->className()));
                    }
                }
            };

            refreshParentsList();
            connect(AssetGraph, &CAssetGraph::LinksChanged, parentsList, [refreshParentsList]() {
                refreshParentsList();
            });
        }



    }
}
