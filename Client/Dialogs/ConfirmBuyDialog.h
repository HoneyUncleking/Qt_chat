#ifndef CONFIRMBUYDIALOG_H
#define CONFIRMBUYDIALOG_H

#include <QDialog>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class ConfirmBuyDialog; }
QT_END_NAMESPACE

class ConfirmBuyDialog : public QDialog {
    Q_OBJECT

public:
    explicit ConfirmBuyDialog(QWidget *parent = nullptr);
    ~ConfirmBuyDialog();

    void setAdDetails(const QString& title, int price, const QString& seller);
    bool isConfirmed() const { return m_confirmed; }

private slots:
    void on_btn_confirm_clicked();
    void on_btn_cancel_clicked();

private:
    Ui::ConfirmBuyDialog *ui;
    bool m_confirmed;
};

#endif