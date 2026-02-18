#include "LoginPage.h"
#include "ui_LoginPage.h"
#include <QMessageBox>
#include <QPainter>
#include <QRandomGenerator>

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginPage)
    , m_usernameValid(false)
    , m_passwordValid(false)
    , m_captchaValid(false)
{
    ui->setupUi(this);
    ui->loginPassword->setEchoMode(QLineEdit::Password);
    generateCaptcha();
    updateLoginButtonState();
    
    connect(ui->loginUsername, &QLineEdit::textChanged, this, &LoginPage::validateInputs);
    connect(ui->loginPassword, &QLineEdit::textChanged, this, &LoginPage::validateInputs);
    connect(ui->captchaInput, &QLineEdit::textChanged, this, &LoginPage::validateInputs);
    connect(ui->loginButton, &QPushButton::clicked, this, &LoginPage::onLoginClicked);
    connect(ui->pushButton, &QPushButton::clicked, this, &LoginPage::onRefreshCaptchaClicked);
    connect(ui->forgotPasswordButton, &QPushButton::clicked, this, &LoginPage::onForgotPasswordClicked);
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::generateCaptcha()
{
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    QString code;
    for (int i = 0; i < 6; ++i) {
        code += chars[QRandomGenerator::global()->bounded(chars.size())];
    }
    m_currentCaptcha = code;
    ui->captchaLabel_2->setText(code);
    ui->captchaInput->clear();
}

bool LoginPage::validateUsername(const QString& username)
{
    if (username.isEmpty()) return false;
    QRegularExpression regex("^[a-zA-Z0-9_]{3,20}$");
    return regex.match(username).hasMatch();
}

bool LoginPage::validatePassword(const QString& password)
{
    return password.length() >= 8;
}

bool LoginPage::validateCaptcha(const QString& captcha)
{
    return captcha == m_currentCaptcha;
}

void LoginPage::setFieldValid(QLineEdit* field, bool valid)
{
    if (valid) {
        field->setStyleSheet("QLineEdit { border: 2px solid #27ae60; }");
    } else {
        field->setStyleSheet("QLineEdit { border: 2px solid #e74c3c; }");
    }
}

void LoginPage::updateLoginButtonState()
{
    m_usernameValid = validateUsername(ui->loginUsername->text());
    m_passwordValid = validatePassword(ui->loginPassword->text());
    m_captchaValid = validateCaptcha(ui->captchaInput->text());
    
    setFieldValid(ui->loginUsername, m_usernameValid);
    setFieldValid(ui->loginPassword, m_passwordValid);
    setFieldValid(ui->captchaInput, m_captchaValid);
    
    bool allValid = m_usernameValid && m_passwordValid && m_captchaValid;
    ui->loginButton->setEnabled(allValid);
}

void LoginPage::validateInputs()
{
    updateLoginButtonState();
}

void LoginPage::onLoginClicked()
{
    if (!ui->loginButton->isEnabled()) {
        QMessageBox::warning(this, "Error", "Please fill all fields correctly");
        return;
    }
    
    QString username = ui->loginUsername->text().trimmed();
    QString password = ui->loginPassword->text();
    
    ui->loginButton->setEnabled(false);
    ui->loginButton->setText("Logging in...");
    
    emit loginRequested(username, password);
}

void LoginPage::onForgotPasswordClicked()
{
    emit navigateToForgotPassword();
}

void LoginPage::onRefreshCaptchaClicked()
{
    generateCaptcha();
}

void LoginPage::clear()
{
    ui->loginUsername->clear();
    ui->loginPassword->clear();
    ui->captchaInput->clear();
    generateCaptcha();
    updateLoginButtonState();
}

void LoginPage::setLoginEnabled(bool enabled)
{
    ui->loginButton->setEnabled(enabled);
    ui->loginButton->setText("Login");
}