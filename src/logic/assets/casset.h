#ifndef CASSET_H
#define CASSET_H

#include <QObject>
#include <toml++/toml.h>

class CAsset : public QObject, public std::enable_shared_from_this<CAsset>, public IHasProperties, public IHasActions
{
    Q_OBJECT

    ASSET_META_PROPERTY(QString, mek)
public:
    QString Name;
    QString SavePath;

    QString getName() const;
    void setName(const QString &newName);


    explicit CAsset(QString Name, QObject *parent = nullptr);

    virtual void buildBeforeChildren(){}
    virtual void buildAfterChildren(){}

    void saveMeta();
    void loadMeta();

    virtual void buildMenu(class QMenu* Menu) override;
    virtual class QObject* getPropertiesObject(QPoint pos) override;
    virtual void propertiesUpdated() override;


private:
    virtual void _buildMenu(QMenu* Menu){}

    Q_PROPERTY(QString Name READ getName WRITE setName NOTIFY NameChanged)

signals:
    void NameChanged();
};

#endif // CASSET_H
