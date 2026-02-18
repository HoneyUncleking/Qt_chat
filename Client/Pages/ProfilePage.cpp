#include "ProfilePage.h"
#include "ui_ProfilePage.h"

ProfilePage::ProfilePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ProfilePage)
{
    ui->setupUi(this);
    clear();
}

ProfilePage::~ProfilePage()
{
    delete ui;
}

void ProfilePage::setProfileData(const QJsonObject& profileData)
{
    m_profileData = profileData;
    
    ui->lbl_name_value->setText(profileData["name"].toString("Not set"));
    ui->lbl_username_value->setText("@" + profileData["username"].toString("username"));
    ui->lbl_email_value->setText(profileData["email"].toString("Not set"));
    ui->lbl_phone_value->setText(profileData["phone"].toString("Not set"));
    ui->lbl_joined_value->setText(profileData["joined"].toString("Unknown"));
    
    QString avatarPath = profileData["avatar"].toString();
    if (!avatarPath.isEmpty()) {
        QPixmap avatar(avatarPath);
        if (!avatar.isNull()) {
            ui->lbl_avatar->setPixmap(avatar.scaled(131, 81, Qt::KeepAspectRatio));
        }
    }
}

void ProfilePage::clear()
{
    ui->lbl_name_value->setText("---");
    ui->lbl_username_value->setText("---");
    ui->lbl_email_value->setText("---");
    ui->lbl_phone_value->setText("---");
    ui->lbl_joined_value->setText("---");
    ui->lbl_avatar->clear();
}

void ProfilePage::on_btn_back_clicked()
{
    emit backRequested();
}

void ProfilePage::on_btn_edit_clicked()
{
    emit editRequested();
}

void ProfilePage::on_btn_history_clicked()
{
    emit historyRequested();
}

void ProfilePage::on_btn_wallet_clicked()
{
    emit walletRequested();
}