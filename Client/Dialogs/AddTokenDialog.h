#ifndef ADDTOKENDIALOG_H
#define ADDTOKENDIALOG_H

#include <QDialog>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui { class AddTokenDialog; }
QT_END_NAMESPACE

class AddTokenDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddTokenDialog(QWidget *parent = nullptr);
    ~AddTokenDialog();

    int getAmount() const { return m_amount; }
    void setCurrentBalance(int balance);

private slots:
    void on_btn_50k_clicked();
    void on_btn_100k_clicked();
    void on_btn_200k_clicked();
    void on_btn_500k_clicked();
    void on_btn_cancel_clicked();
    void on_btn_add_clicked();
    void on_txt_amount_textChanged(const QString &text);

private:
    Ui::AddTokenDialog *ui;
    int m_amount;
    int m_currentBalance;
};

#endif