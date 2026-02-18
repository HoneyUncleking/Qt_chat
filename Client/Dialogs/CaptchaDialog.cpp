#include "CaptchaDialog.h"
#include "ui_CaptchaDialog.h"
#include <QRandomGenerator>
#include <QMessageBox>

CaptchaDialog::CaptchaDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CaptchaDialog)
    , m_captchaCode(generateCaptcha())
{
    ui->setupUi(this);
    setWindowTitle("Captcha Verification");
    setModal(true);
}

CaptchaDialog::~CaptchaDialog()
{
    delete ui;
}

QString CaptchaDialog::generateCaptcha()
{
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString code;
    for (int i = 0; i < 6; ++i) {
        code += chars[QRandomGenerator::global()->bounded(chars.size())];
    }
    return code;
}

void CaptchaDialog::on_btn_refresh_clicked()
{
    m_captchaCode = generateCaptcha();
    ui->captchaLabel->setText(m_captchaCode);
}

void CaptchaDialog::on_btn_submit_clicked()
{
    QString userInput = ui->captchaInput->text().toUpper();
    if (userInput == m_captchaCode) {
        accept();
    } else {
        QMessageBox::warning(this, "Captcha Error", "Incorrect captcha!");
        m_captchaCode = generateCaptcha();
        ui->captchaLabel->setText(m_captchaCode);
        ui->captchaInput->clear();
    }
}