#include "cbuildassetdialog.h"
#include "ui_cbuildassetdialog.h"

#include <QSet>
#include <QThread>
#include <unordered_set>
#include <mutex>
#include <QtConcurrent/QtConcurrentRun>
#include <QtConcurrent/QtConcurrentMap>

CBuildAssetDialog::CBuildAssetDialog(std::weak_ptr<CAsset> AssetPtr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBuildAssetDialog),
    AssetPtr(AssetPtr)
{
    ui->setupUi(this);
    ui->StatusGroup->hide();
    setWindowTitle(QString("Build %0").arg(AssetPtr.lock().get()->getName()));
}

CBuildAssetDialog::~CBuildAssetDialog()
{
    delete ui;
}

void CBuildAssetDialog::on_StartBuildButton_clicked()
{
    ui->StatusGroup->show();
    ui->StartBuildButton->setDisabled(true);
    ui->CancelBuildButton->setDisabled(false);
    build(AssetPtr.lock());
    ui->StartBuildButton->setDisabled(false);
    ui->CancelBuildButton->setDisabled(true);
}


void CBuildAssetDialog::on_CancelBuildButton_clicked()
{
    close();
}

void CBuildAssetDialog::build(std::shared_ptr<CAsset> currentAssetPtr, int level)
{
        if(!level) {
            assetsBuiltThisRound.clear();
            noAssetsToBeBuilt = 0;
        }

        auto currentAsset = currentAssetPtr.get();
        assetsBuiltThisRound.insert(currentAssetPtr);
        noAssetsToBeBuilt++;

        ui->buildProgressBar->setFormat(QString("Pre-Building %0 (%v/%m)").arg(currentAsset->getName()));
        ui->buildProgressBar->setValue(assetsBuiltThisRound.size() - 1);
        ui->buildProgressBar->setMaximum(noAssetsToBeBuilt);
        currentAsset->buildBeforeChildren();


        for(auto& link : AssetGraph->GetChildrenLinksForAsset(currentAssetPtr)) {
            if(link.child().expired() || assetsBuiltThisRound.find(link.child().lock()) != assetsBuiltThisRound.end())
                continue;

            build(link.child().lock(), level + 1);
        }

        QThread::msleep(1000);
        ui->buildProgressBar->setFormat(QString("After-Building %0 (%v/%m)").arg(currentAsset->getName()));
        currentAsset->buildAfterChildren();
        ui->buildProgressBar->setValue(assetsBuiltThisRound.size());
}

