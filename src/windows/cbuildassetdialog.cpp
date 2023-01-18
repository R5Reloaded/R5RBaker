#include "cbuildassetdialog.h"
#include "ui_cbuildassetdialog.h"

CBuildAssetDialog::CBuildAssetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBuildAssetDialog)
{
    ui->setupUi(this);
    ui->StatusGroup->hide();
}

CBuildAssetDialog::~CBuildAssetDialog()
{
    delete ui;
}

void CBuildAssetDialog::on_StartBuildButton_clicked()
{

}


void CBuildAssetDialog::on_CancelBuildButton_clicked()
{

}

