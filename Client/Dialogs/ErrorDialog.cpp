#include "ErrorDialog.h"
#include "ui_ErrorDialog.h"

ErrorDialog::ErrorDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ErrorDialog)
{
    ui->setupUi(this);
    setWindowTitle("Error");
    setModal(true);
}

ErrorDialog::~ErrorDialog()
{
    delete ui;
}

void ErrorDialog::setErrorMessage(const QString& title, const QString& message)
{
    ui->lbl_title->setText("❌ " + title);
    ui->lbl_message->setText(message);
}

void ErrorDialog::on_pushButton_clicked()
{
    reject();
}