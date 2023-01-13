#ifndef CASSETLINKSDIALOG_H
#define CASSETLINKSDIALOG_H

#include <QDialog>
#include <QObject>

class CAssetLinksDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CAssetLinksDialog(class std::weak_ptr<CAsset> Asset, QWidget *parent = nullptr);

signals:

};

#endif // CASSETLINKSDIALOG_H
