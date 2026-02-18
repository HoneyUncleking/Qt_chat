#ifndef SIGNUPPAGE_H
#define SIGNUPPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
namespace Ui { class SignupPage; }
QT_END_NAMESPACE

class SignupPage : public QWidget {
    Q_OBJECT

public:
    explicit SignupPage(QWidget *parent = nullptr);
    ~SignupPage();
    void clear();
    void setSignupEnabled(bool enabled);

signals:
    void navigateToLogin();
    void signupRequested(const QString& name, const QString& username,
                        const QString& email, const QString& phone,
                        const QString& password);
    void showMessage(const QString& title, const QString& message, bool isError);

private slots:
    void onSignupClicked();
    void validateInputs();

private:
    void updateSignupButtonState();
    void setFieldValid(QLineEdit* field, bool valid);
    bool validateName(const QString& name);
    bool validateUsername(const QString& username);
    bool validateEmail(const QString& email);
    bool validatePhone(const QString& phone);
    bool validatePassword(const QString& password);
    bool validateConfirmPassword();
    
    Ui::SignupPage *ui;
    bool m_nameValid;
    bool m_usernameValid;
    bool m_emailValid;
    bool m_phoneValid;
    bool m_passwordValid;
    bool m_confirmValid;
};

#endif