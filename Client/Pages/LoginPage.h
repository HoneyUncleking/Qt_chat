#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginPage; }
QT_END_NAMESPACE

class LoginPage : public QWidget {
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();
    void clear();
    void setLoginEnabled(bool enabled);

signals:
    void navigateToSignup();
    void navigateToForgotPassword();
    void loginRequested(const QString& username, const QString& password);
    void showMessage(const QString& title, const QString& message, bool isError);

private slots:
    void onLoginClicked();
    void onForgotPasswordClicked();
    void onRefreshCaptchaClicked();
    void validateInputs();

private:
    void generateCaptcha();
    bool validateUsername(const QString& username);
    bool validatePassword(const QString& password);
    bool validateCaptcha(const QString& captcha);
    void setFieldValid(QLineEdit* field, bool valid);
    void updateLoginButtonState();
    
    Ui::LoginPage *ui;
    QString m_currentCaptcha;
    bool m_usernameValid;
    bool m_passwordValid;
    bool m_captchaValid;
};

#endif