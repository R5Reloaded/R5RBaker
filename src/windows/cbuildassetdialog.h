#ifndef CBUILDASSETDIALOG_H
#define CBUILDASSETDIALOG_H

#include <QDialog>

namespace Ui {
class CBuildAssetDialog;
}

class CBuildAssetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CBuildAssetDialog(QWidget *parent = nullptr);
    ~CBuildAssetDialog();

private slots:
    void on_StartBuildButton_clicked();

    void on_CancelBuildButton_clicked();

private:
    Ui::CBuildAssetDialog *ui;
};

#endif // CBUILDASSETDIALOG_H
