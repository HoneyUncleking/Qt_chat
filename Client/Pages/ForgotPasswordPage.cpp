#include "ForgotPasswordPage.h"
#include "ui_ForgotPasswordPage.h"
#include <QMessageBox>
#include <QRegularExpression>

ForgotPasswordPage::ForgotPasswordPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ForgotPasswordPage)
{
    ui->setupUi(this);
    ui->lbl_reset_link->setVisible(false);
}

ForgotPasswordPage::~ForgotPasswordPage()
{
    delete ui;
}

void ForgotPasswordPage::on_btn_send_reset_clicked()
{
    QString email = ui->txt_forget_email->text().trimmed();
    
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your email");
        return;
    }
    
    QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    if (!regex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Error", "Please enter a valid email");
        return;
    }
    
    ui->lbl_reset_link->setText("✅ Reset link sent to " + email);
    ui->lbl_reset_link->setVisible(true);
    ui->txt_forget_email->clear();
}

void ForgotPasswordPage::on_lbl_back_to_logi_linkActivated()
{
    emit navigateToLogin();
}