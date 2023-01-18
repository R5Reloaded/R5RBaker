#ifndef CASSETLINKSDIALOG_H
#define CASSETLINKSDIALOG_H

#include <QDialog>
#include <QObject>

namespace Ui {
class CAssetLinksDialog;
}

class CAssetLinksDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CAssetLinksDialog(class std::weak_ptr<CAsset> Asset, QWidget *parent = nullptr);
    ~CAssetLinksDialog();

private:
    Ui::CAssetLinksDialog* ui;

    std::weak_ptr<class CAsset> AssetPtr;

    void refreshChildrenGroup();
    void refreshParentsGroup();
signals:

private slots:
    void on_AddChildLinkButton_clicked();
    void on_AddParentLinkButton_clicked();
    void on_ChildrenListWidget_customContextMenuRequested(const QPoint &pos);
    void on_ParentsListWidget_customContextMenuRequested(const QPoint &pos);
};

#endif // CASSETLINKSDIALOG_H
