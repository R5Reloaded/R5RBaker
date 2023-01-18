#include "casset.h"
#include "../../windows/cassetlinksdialog.h"
#include "qmetaobject.h"
#include <QMetaObject>
#include <QDir>
#include <QMenu>
#include <QVariant>

CAsset::CAsset(QString Name, QObject *parent)
    : QObject{parent}
{
    setName(Name);
    setObjectName(Name);
}

QString CAsset::getName() const
{
    return Name;
}

void CAsset::setName(const QString &newName)
{
    if (Name == newName)
        return;
    moveMeta(newName);
    emit NameChanged();
}

void CAsset::loadMeta()
{
    QFile metaFile(WorkingDirectory->absoluteFilePath(Name + ".meta"));

    if(!metaFile.exists()) {
        saveMeta();
        return;
    }

    try {
        toml::table metaTable = toml::parse(metaFile.readAll().toStdString());
        for(auto& kv : metaTable) {
            if(QVariant objectProperty = this->property(kv.first.data()); objectProperty.isValid()) {
                switch(kv.second.type()) {
                case toml::node_type::integer:
                    objectProperty.setValue(kv.second.as_integer()->get());
                    break;
                case toml::node_type::floating_point:
                    objectProperty.setValue(kv.second.as_floating_point()->get());
                    break;
                case toml::node_type::string:
                    objectProperty.setValue(QString::fromStdString(kv.second.as_string()->get()));
                case toml::node_type::boolean:
                    objectProperty.setValue(kv.second.as_boolean()->get());
                default:
                    *LogPane << "unhandled loadMeta type";
                }

            }
        }
    } catch(const toml::parse_error& err) {
        qDebug() << err.description().data();
    }
}

void CAsset::deleteMeta()
{
    QFile metaFile(getMetaPath());
    metaFile.remove();
}

void CAsset::moveMeta(QString newPath)
{
    deleteMeta();
    Name = newPath;
    saveMeta();
}

QString CAsset::getMetaPath()
{
    return WorkingDirectory->absoluteFilePath(Name + ".meta");
}

void CAsset::saveMeta()
{

    toml::table metaTable{};

    for(int i = 0; i < this->metaObject()->propertyCount(); i++) {
        QMetaProperty property = this->metaObject()->property(i);
        QVariant val = property.read(this);
        if(val.isValid()) {
            if(val.canConvert<QString>()) {
                metaTable.emplace(property.name(), val.toString().toStdString());
            } else if(val.canConvert<float>()) {
                metaTable.emplace(property.name(), val.toFloat());
            } else if(val.canConvert<int>()) {
                metaTable.emplace(property.name(), val.toInt());
            } else if(val.canConvert<bool>()) {
                metaTable.emplace(property.name(), val.toBool());
            } else {
                *LogPane << "ERROR: COULD NOT SAVE PROPERTY " << property.name() << " for " << getName();
            }
        }
    }

    QDir().mkpath(QFileInfo(getMetaPath()).path());
    QFile metaFile(getMetaPath());

    metaFile.open(QFile::WriteOnly);
    std::stringstream s;
    s << metaTable;
    metaFile.write(s.str().c_str());
    metaFile.close();
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

