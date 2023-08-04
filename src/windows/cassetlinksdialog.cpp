#include "cassetlinksdialog.h"
#include "ui_cassetlinksdialog.h"

#include <QDialogButtonBox>
#include <QGroupBox>
#include <QListWidget>
#include <QMessageBox>

CAssetLinksDialog::CAssetLinksDialog(std::weak_ptr<CAsset> AssetPtr, QWidget *parent) : QDialog{parent}, ui(new Ui::CAssetLinksDialog()), AssetPtr(AssetPtr)
{
    ui->setupUi(this);

    auto Asset = AssetPtr.lock().get();
    if(!Asset) {
        close();
        return;
    }

    setWindowTitle(QString("%1 Links").arg(Asset->getName()));
    setModal(true);

    refreshChildrenGroup();
    refreshParentsGroup();
}

CAssetLinksDialog::~CAssetLinksDialog()
{
    delete ui;
}

void CAssetLinksDialog::refreshChildrenGroup()
{
    std::shared_ptr<CAsset> Asset = AssetPtr.lock();
    if(!Asset) {
        QMessageBox::critical(this, "Error", "Couldn't add link to Asset as it isn't loaded anymore!");
        return;
    }

    // CHILDREN LIST WIDGET

    ui->ChildrenListWidget->clear();
    for(auto& link : AssetGraph->GetLinksForAsset(AssetPtr)) {
        if(link.parent().lock().get() == Asset.get() && link.child().lock()) {
            auto child = link.child().lock();

            QListWidgetItem* item = new QListWidgetItem(QString("%1: [%2]").arg(child->getName(), child->metaObject()->className()));
            item->setData(Qt::UserRole, QVariant::fromValue(link.child()));
            ui->ChildrenListWidget->addItem(item);
        }
    }

    // CHILDREN ADD COMBO

    ui->AddChildCombo->clear();
    for(std::weak_ptr<CAsset>& assetPtr : AssetGraph->GetAllAssets()) {
        auto otherAsset = assetPtr.lock();

        if(otherAsset == Asset || AssetGraph->AreLinked(otherAsset, Asset))
            continue;

        ui->AddChildCombo->addItem(QString("%1: [%2]").arg(otherAsset->getName(), otherAsset->metaObject()->className()), QVariant::fromValue(assetPtr));
    }

}

void CAssetLinksDialog::refreshParentsGroup()
{
    std::shared_ptr<CAsset> Asset = AssetPtr.lock();
    if(!Asset) {
        QMessageBox::critical(this, "Error", "Couldn't add link to Asset as it isn't loaded anymore!");
        return;
    }

    // PARENTS LIST WIDGET

    ui->ParentsListWidget->clear();
    for(auto& link : AssetGraph->GetLinksForAsset(AssetPtr)) {
        if(link.child().lock().get() == Asset.get() && link.parent().lock()) {
            auto parent = link.parent().lock();

            QListWidgetItem* item = new QListWidgetItem(QString("%1: [%2]").arg(parent->getName(), parent->metaObject()->className()));
            item->setData(Qt::UserRole, QVariant::fromValue(link.parent()));
            ui->ParentsListWidget->addItem(item);
        }
    }

    // PARENTS ADD COMBO

    ui->AddParentCombo->clear();
    for(std::weak_ptr<CAsset>& assetPtr : AssetGraph->GetAllAssets()) {
        auto otherAsset = assetPtr.lock();

        if(otherAsset == Asset || AssetGraph->AreLinked(otherAsset, Asset))
            continue;

        ui->AddParentCombo->addItem(QString("%1: [%2]").arg(otherAsset->getName(), otherAsset->metaObject()->className()), QVariant::fromValue(assetPtr));
    }
}

void CAssetLinksDialog::on_AddChildLinkButton_clicked()
{
    if(ui->AddChildCombo->currentData().isValid()) {
        std::weak_ptr<CAsset> otherAssetPtr = ui->AddChildCombo->currentData().value<std::weak_ptr<CAsset>>();
        if(auto otherAsset = otherAssetPtr.lock(); otherAsset) {
            AssetGraph->AddLink(AssetPtr, otherAssetPtr);
        }
    }
    refreshChildrenGroup();
}


void CAssetLinksDialog::on_AddParentLinkButton_clicked()
{
    if(ui->AddParentCombo->currentData().isValid()) {
        std::weak_ptr<CAsset> otherAssetPtr = ui->AddParentCombo->currentData().value<std::weak_ptr<CAsset>>();
        if(auto otherAsset = otherAssetPtr.lock(); otherAsset) {
            AssetGraph->AddLink(otherAssetPtr, AssetPtr);
        }
    }
    refreshParentsGroup();
}


void CAssetLinksDialog::on_ChildrenListWidget_customContextMenuRequested(const QPoint &pos)
{
    auto item = ui->ChildrenListWidget->itemAt(pos);
    if(!item) return;
    std::weak_ptr<CAsset> otherAssetPtr = item->data(Qt::UserRole).value<std::weak_ptr<CAsset>>();
    if(otherAssetPtr.expired()) return;

    QMenu* menu = new QMenu(this);
    QAction* removeAction = menu->addAction("Remove");

    connect(removeAction, &QAction::triggered, this, [this, otherAssetPtr]() {
        AssetGraph->RemoveLink(AssetPtr, otherAssetPtr);
        refreshChildrenGroup();
    });

    menu->exec(ui->ChildrenListWidget->mapToGlobal(pos));
    delete menu;
}


void CAssetLinksDialog::on_ParentsListWidget_customContextMenuRequested(const QPoint &pos)
{
    auto item = ui->ParentsListWidget->itemAt(pos);
    if(!item) return;
    std::weak_ptr<CAsset> otherAssetPtr = item->data(Qt::UserRole).value<std::weak_ptr<CAsset>>();
    if(otherAssetPtr.expired()) return;

    QMenu* menu = new QMenu(this);
    QAction* removeAction = menu->addAction("Remove");

    connect(removeAction, &QAction::triggered, this, [this, otherAssetPtr]() {
        AssetGraph->RemoveLink(AssetPtr, otherAssetPtr);
        refreshParentsGroup();
    });

    menu->exec(ui->ParentsListWidget->mapToGlobal(pos));
    delete menu;
}

