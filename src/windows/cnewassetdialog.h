#ifndef CNEWASSETDIALOG_H
#define CNEWASSETDIALOG_H

#include <QDialog>

namespace Ui {
class CNewAssetDialog;
}

class CNewAssetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CNewAssetDialog(QWidget *parent = nullptr);
    ~CNewAssetDialog();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::CNewAssetDialog *ui;
};

#endif // CNEWASSETDIALOG_H
