#ifndef EDITPROFILEPAGE_H
#define EDITPROFILEPAGE_H

#include <QWidget>
#include <QJsonObject>
#include <QLineEdit>
#include <QPixmap>

QT_BEGIN_NAMESPACE
namespace Ui { class EditProfilePage; }
QT_END_NAMESPACE

class EditProfilePage : public QWidget {
    Q_OBJECT

public:
    explicit EditProfilePage(QWidget *parent = nullptr);
    ~EditProfilePage();
    void setProfileData(const QJsonObject& profileData);
    void clear();

signals:
    void backRequested();
    void saveRequested(const QJsonObject& updatedData);
    void imageSelected(const QString& imagePath);

private slots:
    void on_btn_back_clicked();
    void on_btn_save_clicked();
    void on_btn_select_image_clicked();

private:
    void updateSaveButtonState();
    void setFieldValid(QLineEdit* field, bool valid);
    bool validateName(const QString& name);
    bool validateUsername(const QString& username);
    bool validateEmail(const QString& email);
    bool validatePhone(const QString& phone);
    bool validatePassword(const QString& password);
    
    Ui::EditProfilePage *ui;
    bool m_nameValid;
    bool m_usernameValid;
    bool m_emailValid;
    bool m_phoneValid;
    bool m_newPasswordValid;
    bool m_confirmValid;
};

#endif