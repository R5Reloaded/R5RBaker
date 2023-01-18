#include "cnewassetdialog.h"
#include "ui_cnewassetdialog.h"
#include <QMessageBox>
#include <QDir>

CNewAssetDialog::CNewAssetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CNewAssetDialog)
{
    ui->setupUi(this);

    ui->typeCombo->addItems({
                                "CAsset*",
                                "CRPak*"
                            });
}

CNewAssetDialog::~CNewAssetDialog()
{
    delete ui;
}

void CNewAssetDialog::on_buttonBox_accepted()
{
    if(ui->nameEdit->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "Name cannot be blank");
        return;
    }
    const QDir path(WorkingDirectory->filePath(ui->nameEdit->text()));

    if(!path.isAbsolute()) {
        QMessageBox::critical(this, "Error", "Path is invalid");
        return;
    }

    qDebug() << ui->typeCombo->currentText().toUtf8();

    QMetaType assetType = QMetaType::fromName(ui->typeCombo->currentText().toUtf8());

    if(!assetType.isValid()) {
        QMessageBox::critical(this, "Error", "Asset type is invalid");
        return;
    }

    const QMetaObject* assetMeta = assetType.metaObject();
    QObject* newAssetObject = assetMeta->newInstance(Q_ARG(QString, ui->nameEdit->text()));
    CAsset* newAsset = qobject_cast<CAsset*>(newAssetObject);

    AssetGraph->LoadAsset(newAsset);

}


void CNewAssetDialog::on_buttonBox_rejected()
{
    this->close();
}
