#ifndef CRPAK_H
#define CRPAK_H

#include "../casset.h"
#include <QObject>

class CRPak : public CAsset
{
    Q_OBJECT
public:
    explicit CRPak(QString Name, QObject *parent = nullptr);
    // CAsset interface
private:
    void _buildMenu(QMenu *Menu) override;

    // CAsset interface
public:
    void buildBeforeChildren() override;
    void buildAfterChildren() override;
};

#endif // CRPAK_H
