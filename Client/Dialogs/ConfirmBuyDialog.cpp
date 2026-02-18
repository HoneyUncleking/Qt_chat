#include "ConfirmBuyDialog.h"
#include "ui_ConfirmBuyDialog.h"

ConfirmBuyDialog::ConfirmBuyDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfirmBuyDialog)
    , m_confirmed(false)
{
    ui->setupUi(this);
    setWindowTitle("Confirm Purchase");
    setModal(true);
}

ConfirmBuyDialog::~ConfirmBuyDialog()
{
    delete ui;
}

void ConfirmBuyDialog::setAdDetails(const QString& title, int price, const QString& seller)
{
    ui->lbl_title->setText("Confirm Purchase");
    ui->lbl_product_name->setText(title);
    ui->lbl_price_value->setText(QString("%1 Toman").arg(price));
    ui->lbl_seller_value->setText("@" + seller);
}

void ConfirmBuyDialog::on_btn_confirm_clicked()
{
    m_confirmed = true;
    accept();
}

void ConfirmBuyDialog::on_btn_cancel_clicked()
{
    m_confirmed = false;
    reject();
}