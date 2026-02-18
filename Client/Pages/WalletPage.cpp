#include "WalletPage.h"
#include "ui_WalletPage.h"
#include <QJsonObject>

WalletPage::WalletPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WalletPage)
    , m_balance(0)
{
    ui->setupUi(this);
    clear();
}

WalletPage::~WalletPage()
{
    delete ui;
}

void WalletPage::setBalance(int balance)
{
    m_balance = balance;
    ui->lbl_balance_value->setText(QString("%1 Toman").arg(balance));
}

void WalletPage::setTransactions(const QJsonArray& transactions)
{
    ui->list_transactions->clear();
    
    for (const auto& item : transactions) {
        QJsonObject obj = item.toObject();
        addTransactionItem(obj);
    }
}

void WalletPage::addTransactionItem(const QJsonObject& transaction)
{
    QString description = transaction["description"].toString();
    int amount = transaction["amount"].toInt();
    QString type = transaction["type"].toString();
    
    QString sign = (type == "credit") ? "+" : "-";
    QString text = QString("%1 %2%3 Toman").arg(description).arg(sign).arg(amount);
    
    QListWidgetItem* item = new QListWidgetItem(text);
    ui->list_transactions->addItem(item);
}

void WalletPage::clear()
{
    setBalance(0);
    ui->list_transactions->clear();
}

void WalletPage::on_btn_back_clicked()
{
    emit backRequested();
}

void WalletPage::on_btn_charge_clicked()
{
    emit chargeRequested();
}

void WalletPage::on_btn_history_clicked()
{
    emit historyRequested();
}