#ifndef CASSET_H
#define CASSET_H

#include <QObject>
#include <toml++/toml.h>

class CAsset : public QObject, public std::enable_shared_from_this<CAsset>, public IHasProperties, public IHasActions
{
    Q_OBJECT

    ASSET_META_PROPERTY_CUSTOM_SETTER(QString, Name)
    ASSET_META_PROPERTY(bool, usePhysics)
    ASSET_META_PROPERTY(bool, isStatic)
public:
    Q_INVOKABLE explicit CAsset(QString Name, QObject *parent = nullptr);

    virtual void buildBeforeChildren(){}
    virtual void buildAfterChildren(){}

    void saveMeta();
    void loadMeta();
    void deleteMeta();
    void moveMeta(QString newPath);

    QString getMetaPath();

    virtual void buildMenu(class QMenu* Menu) final override;
    virtual class QObject* getPropertiesObject(QPoint pos) final override;
    virtual void propertiesUpdated() final override;


private:

    virtual void _buildMenu(QMenu* Menu){}
signals:

};

Q_DECLARE_METATYPE(CAsset*)

#endif // CASSET_H
