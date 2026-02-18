#include "EditProfilePage.h"
#include "ui_EditProfilePage.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QRegularExpression>

EditProfilePage::EditProfilePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EditProfilePage)
    , m_nameValid(false)
    , m_usernameValid(false)
    , m_emailValid(false)
    , m_phoneValid(false)
    , m_newPasswordValid(true)
    , m_confirmValid(true)
{
    ui->setupUi(this);
    ui->txt_current_password->setEchoMode(QLineEdit::Password);
    ui->txt_new_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_7->setEchoMode(QLineEdit::Password);
    clear();
    updateSaveButtonState();
}

EditProfilePage::~EditProfilePage()
{
    delete ui;
}

void EditProfilePage::setProfileData(const QJsonObject& profileData)
{
    ui->txt_name->setText(profileData["name"].toString());
    ui->txt_username->setText(profileData["username"].toString());
    ui->txt_email->setText(profileData["email"].toString());
    ui->txt_phone->setText(profileData["phone"].toString());
    
    QString avatarPath = profileData["avatar"].toString();
    if (!avatarPath.isEmpty()) {
        QPixmap avatar(avatarPath);
        if (!avatar.isNull()) {
            ui->lbl_avatar->setPixmap(avatar.scaled(171, 91, Qt::KeepAspectRatio));
        }
    }
}

bool EditProfilePage::validateName(const QString& name)
{
    return !name.trimmed().isEmpty() && name.length() >= 3;
}

bool EditProfilePage::validateUsername(const QString& username)
{
    if (username.isEmpty()) return false;
    QRegularExpression regex("^[a-zA-Z0-9_]{3,20}$");
    return regex.match(username).hasMatch();
}

bool EditProfilePage::validateEmail(const QString& email)
{
    if (email.isEmpty()) return false;
    QRegularExpression regex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex.match(email).hasMatch();
}

bool EditProfilePage::validatePhone(const QString& phone)
{
    if (phone.isEmpty()) return false;
    QRegularExpression regex("^09[0-9]{9}$");
    return regex.match(phone).hasMatch();
}

bool EditProfilePage::validatePassword(const QString& password)
{
    return password.isEmpty() || password.length() >= 8;
}

void EditProfilePage::setFieldValid(QLineEdit* field, bool valid)
{
    if (valid) {
        field->setStyleSheet("QLineEdit { border: 2px solid #27ae60; }");
    } else {
        field->setStyleSheet("QLineEdit { border: 2px solid #e74c3c; }");
    }
}

void EditProfilePage::updateSaveButtonState()
{
    m_nameValid = validateName(ui->txt_name->text());
    m_usernameValid = validateUsername(ui->txt_username->text());
    m_emailValid = validateEmail(ui->txt_email->text());
    m_phoneValid = validatePhone(ui->txt_phone->text());
    
    QString newPass = ui->txt_new_password->text();
    if (!newPass.isEmpty()) {
        m_newPasswordValid = validatePassword(newPass);
        m_confirmValid = (newPass == ui->lineEdit_7->text());
    } else {
        m_newPasswordValid = true;
        m_confirmValid = true;
    }
    
    setFieldValid(ui->txt_name, m_nameValid);
    setFieldValid(ui->txt_username, m_usernameValid);
    setFieldValid(ui->txt_email, m_emailValid);
    setFieldValid(ui->txt_phone, m_phoneValid);
    setFieldValid(ui->txt_new_password, m_newPasswordValid);
    setFieldValid(ui->lineEdit_7, m_confirmValid);
    
    bool allValid = m_nameValid && m_usernameValid && m_emailValid && m_phoneValid && m_newPasswordValid && m_confirmValid;
    ui->btn_save->setEnabled(allValid);
}

void EditProfilePage::on_btn_back_clicked()
{
    emit backRequested();
}

void EditProfilePage::on_btn_save_clicked()
{
    if (!ui->btn_save->isEnabled()) {
        QMessageBox::warning(this, "Error", "Please fill all fields correctly");
        return;
    }
    
    QJsonObject updatedData;
    updatedData["name"] = ui->txt_name->text().trimmed();
    updatedData["username"] = ui->txt_username->text().trimmed();
    updatedData["email"] = ui->txt_email->text().trimmed();
    updatedData["phone"] = ui->txt_phone->text().trimmed();
    
    if (!ui->txt_current_password->text().isEmpty()) {
        updatedData["currentPassword"] = ui->txt_current_password->text();
    }
    
    if (!ui->txt_new_password->text().isEmpty()) {
        updatedData["newPassword"] = ui->txt_new_password->text();
    }
    
    emit saveRequested(updatedData);
}

void EditProfilePage::on_btn_select_image_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select Profile Image",
        QDir::homePath(), "Images (*.png *.jpg *.jpeg *.bmp)");
    
    if (!fileName.isEmpty()) {
        QPixmap image(fileName);
        if (!image.isNull()) {
            ui->lbl_avatar->setPixmap(image.scaled(171, 91, Qt::KeepAspectRatio));
            emit imageSelected(fileName);
        }
    }
}

void EditProfilePage::clear()
{
    ui->txt_name->clear();
    ui->txt_username->clear();
    ui->txt_email->clear();
    ui->txt_phone->clear();
    ui->txt_current_password->clear();
    ui->txt_new_password->clear();
    ui->lineEdit_7->clear();
    ui->lbl_avatar->clear();
    updateSaveButtonState();
}