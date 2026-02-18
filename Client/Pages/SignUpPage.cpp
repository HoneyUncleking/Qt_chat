#include "SignupPage.h"
#include "ui_SignupPage.h"
#include <QMessageBox>
#include <QRegularExpression>

SignupPage::SignupPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SignupPage)
    , m_nameValid(false)
    , m_usernameValid(false)
    , m_emailValid(false)
    , m_phoneValid(false)
    , m_passwordValid(false)
    , m_confirmValid(false)
{
    ui->setupUi(this);
    ui->SignupPassword->setEchoMode(QLineEdit::Password);
    ui->signupConfirmPassword->setEchoMode(QLineEdit::Password);
    
    updateSignupButtonState();
    
    connect(ui->signupName, &QLineEdit::textChanged, this, &SignupPage::validateInputs);
    connect(ui->signupUsername, &QLineEdit::textChanged, this, &SignupPage::validateInputs);
    connect(ui->signupEmail, &QLineEdit::textChanged, this, &SignupPage::validateInputs);
    connect(ui->signupPhone, &QLineEdit::textChanged, this, &SignupPage::validateInputs);
    connect(ui->SignupPassword, &QLineEdit::textChanged, this, &SignupPage::validateInputs);
    connect(ui->signupConfirmPassword, &QLineEdit::textChanged, this, &SignupPage::validateInputs);
    connect(ui->createAccountButton, &QPushButton::clicked, this, &SignupPage::onSignupClicked);
}

SignupPage::~SignupPage()
{
    delete ui;
}

void SignupPage::validateInputs()
{
    updateSignupButtonState();
}

bool SignupPage::validateName(const QString& name)
{
    QString trimmed = name.trimmed();
    return !trimmed.isEmpty() && trimmed.length() >= 3 && trimmed.length() <= 50;
}

bool SignupPage::validateUsername(const QString& username)
{
    if (username.isEmpty()) return false;
    QRegularExpression regex("^[a-zA-Z0-9_]{3,20}$");
    return regex.match(username).hasMatch();
}

bool SignupPage::validateEmail(const QString& email)
{
    if (email.isEmpty()) return false;
    QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex.match(email).hasMatch();
}

bool SignupPage::validatePhone(const QString& phone)
{
    if (phone.isEmpty()) return false;
    QRegularExpression regex("^09[0-9]{9}$");
    return regex.match(phone).hasMatch();
}

bool SignupPage::validatePassword(const QString& password)
{
    return password.length() >= 8;
}

bool SignupPage::validateConfirmPassword()
{
    return ui->SignupPassword->text() == ui->signupConfirmPassword->text();
}

void SignupPage::setFieldValid(QLineEdit* field, bool valid)
{
    if (valid) {
        field->setStyleSheet("QLineEdit { border: 2px solid #27ae60; }");
    } else {
        field->setStyleSheet("QLineEdit { border: 2px solid #e74c3c; }");
    }
}

void SignupPage::updateSignupButtonState()
{
    m_nameValid = validateName(ui->signupName->text());
    m_usernameValid = validateUsername(ui->signupUsername->text());
    m_emailValid = validateEmail(ui->signupEmail->text());
    m_phoneValid = validatePhone(ui->signupPhone->text());
    m_passwordValid = validatePassword(ui->SignupPassword->text());
    m_confirmValid = validateConfirmPassword();
    
    setFieldValid(ui->signupName, m_nameValid);
    setFieldValid(ui->signupUsername, m_usernameValid);
    setFieldValid(ui->signupEmail, m_emailValid);
    setFieldValid(ui->signupPhone, m_phoneValid);
    setFieldValid(ui->SignupPassword, m_passwordValid);
    setFieldValid(ui->signupConfirmPassword, m_confirmValid);
    
    bool allValid = m_nameValid && m_usernameValid && m_emailValid && m_phoneValid && m_passwordValid && m_confirmValid;
    ui->createAccountButton->setEnabled(allValid);
}

void SignupPage::onSignupClicked()
{
    if (!ui->createAccountButton->isEnabled()) {
        QMessageBox::warning(this, "Error", "Please fill all fields correctly");
        return;
    }
    
    QString name = ui->signupName->text().trimmed();
    QString username = ui->signupUsername->text().trimmed();
    QString email = ui->signupEmail->text().trimmed();
    QString phone = ui->signupPhone->text().trimmed();
    QString password = ui->SignupPassword->text();
    
    ui->createAccountButton->setEnabled(false);
    ui->createAccountButton->setText("Creating account...");
    
    emit signupRequested(name, username, email, phone, password);
}

void SignupPage::clear()
{
    ui->signupName->clear();
    ui->signupUsername->clear();
    ui->signupEmail->clear();
    ui->signupPhone->clear();
    ui->SignupPassword->clear();
    ui->signupConfirmPassword->clear();
    updateSignupButtonState();
}

void SignupPage::setSignupEnabled(bool enabled)
{
    ui->createAccountButton->setEnabled(enabled);
    ui->createAccountButton->setText("Create Account");
}