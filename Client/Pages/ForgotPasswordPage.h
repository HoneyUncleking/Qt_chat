#ifndef FORGOTPASSWORDPAGE_H
#define FORGOTPASSWORDPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class ForgotPasswordPage; }
QT_END_NAMESPACE

class ForgotPasswordPage : public QWidget {
    Q_OBJECT

public:
    explicit ForgotPasswordPage(QWidget *parent = nullptr);
    ~ForgotPasswordPage();

signals:
    void navigateToLogin();

private slots:
    void on_btn_send_reset_clicked();
    void on_lbl_back_to_logi_linkActivated();

private:
    Ui::ForgotPasswordPage *ui;
};

#endif