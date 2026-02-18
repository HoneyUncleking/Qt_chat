#ifndef WALLETPAGE_H
#define WALLETPAGE_H

#include <QWidget>
#include <QJsonArray>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class WalletPage; }
QT_END_NAMESPACE

class WalletPage : public QWidget {
    Q_OBJECT

public:
    explicit WalletPage(QWidget *parent = nullptr);
    ~WalletPage();
    void setBalance(int balance);
    void setTransactions(const QJsonArray& transactions);
    void clear();

signals:
    void backRequested();
    void chargeRequested();
    void historyRequested();

private slots:
    void on_btn_back_clicked();
    void on_btn_charge_clicked();
    void on_btn_history_clicked();

private:
    void addTransactionItem(const QJsonObject& transaction);
    
    Ui::WalletPage *ui;
    int m_balance;
};

#endif