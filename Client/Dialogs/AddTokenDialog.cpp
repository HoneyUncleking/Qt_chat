#include "AddTokenDialog.h"
#include "ui_AddTokenDialog.h"
#include <QMessageBox>

AddTokenDialog::AddTokenDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddTokenDialog)
    , m_amount(0)
    , m_currentBalance(0)
{
    ui->setupUi(this);
    setWindowTitle("Add Tokens");
    setModal(true);
    ui->btn_add->setEnabled(false);
}

AddTokenDialog::~AddTokenDialog()
{
    delete ui;
}

void AddTokenDialog::setCurrentBalance(int balance)
{
    m_currentBalance = balance;
    ui->lbl_current_balance_value->setText(QString("%1 Toman").arg(balance));
}

void AddTokenDialog::on_txt_amount_textChanged(const QString &text)
{
    bool ok;
    int amount = text.toInt(&ok);
    if (ok && amount >= 1000 && amount <= 10000000) {
        m_amount = amount;
        ui->btn_add->setEnabled(true);
    } else {
        ui->btn_add->setEnabled(false);
    }
}

void AddTokenDialog::on_btn_50k_clicked()
{
    ui->txt_amount->setText("50000");
}

void AddTokenDialog::on_btn_100k_clicked()
{
    ui->txt_amount->setText("100000");
}

void AddTokenDialog::on_btn_200k_clicked()
{
    ui->txt_amount->setText("200000");
}

void AddTokenDialog::on_btn_500k_clicked()
{
    ui->txt_amount->setText("500000");
}

void AddTokenDialog::on_btn_cancel_clicked()
{
    reject();
}

void AddTokenDialog::on_btn_add_clicked()
{
    if (m_amount > 0) {
        accept();
    }
}