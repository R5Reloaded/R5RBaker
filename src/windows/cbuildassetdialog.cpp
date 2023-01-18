#include "cbuildassetdialog.h"
#include "ui_cbuildassetdialog.h"

CBuildAssetDialog::CBuildAssetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CBuildAssetDialog)
{
    ui->setupUi(this);
}

CBuildAssetDialog::~CBuildAssetDialog()
{
    delete ui;
}
