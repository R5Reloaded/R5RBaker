#ifndef CBUILDASSETDIALOG_H
#define CBUILDASSETDIALOG_H

#include <QDialog>
#include <unordered_set>
#include <shared_mutex>

namespace Ui {
class CBuildAssetDialog;
}

class CBuildAssetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CBuildAssetDialog(std::weak_ptr<class CAsset> AssetPtr, QWidget *parent = nullptr);
    ~CBuildAssetDialog();

private slots:
    void on_StartBuildButton_clicked();

    void on_CancelBuildButton_clicked();

private:
    Ui::CBuildAssetDialog *ui;
    std::weak_ptr<class CAsset> AssetPtr;

    std::unordered_set<std::shared_ptr<CAsset>> assetsBuiltThisRound;
    std::atomic<int> noAssetsToBeBuilt;
    std::shared_mutex mtx;



    void build(std::shared_ptr<CAsset> currentAssetPtr, int level = 0);
};

#endif // CBUILDASSETDIALOG_H
