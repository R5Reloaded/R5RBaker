#ifndef CRMDL_H
#define CRMDL_H


class CRMdl : public CAsset
{
    Q_OBJECT

    ASSET_META_PROPERTY(bool, usePhysics)
    ASSET_META_PROPERTY(bool, isStatic)
public:
    Q_INVOKABLE explicit CRMdl(QString Name, QObject *parent = nullptr);

    // CAsset interface
public:
    void buildBeforeChildren() override;
    void buildAfterChildren() override;

private:
    void _buildMenu(QMenu *Menu) override;
};

#endif // CRMDL_H
