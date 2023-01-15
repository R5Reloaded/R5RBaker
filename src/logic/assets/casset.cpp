#include "casset.h"
#include "../../windows/cassetlinksdialog.h"
#include <QMenu>

QString CAsset::getName() const
{
    return Name;
}

void CAsset::setName(const QString &newName)
{
    if (Name == newName)
        return;
    Name = newName;
    emit NameChanged();
}

CAsset::CAsset(QString Name, QObject *parent)
    : QObject{parent}
{
    srand(time(nullptr));
    this->Name = Name;
    setObjectName(Name);
}

void CAsset::saveMeta()
{

}

void CAsset::buildMenu(QMenu *Menu)
{
    auto assetMenu = Menu->addMenu(QString("As %1").arg(metaObject()->className()));
    auto linksMenuAction = assetMenu->addAction("View/Edit Links");
    connect(linksMenuAction, &QAction::triggered, Menu, [this]() {
        (new CAssetLinksDialog(weak_from_this(), nullptr))->show();
    });

    auto unloadAssetAction = assetMenu->addAction("Unload");
    connect(unloadAssetAction, &QAction::triggered, Menu, [this]() {
        AssetGraph->UnloadAsset(weak_from_this());
    });


    _buildMenu(assetMenu);
}


QObject* CAsset::getPropertiesObject(QPoint pos)
{
    return this;
}

void CAsset::propertiesUpdated()
{
    AssetGraph->RebuildVirtualGraph();
}
