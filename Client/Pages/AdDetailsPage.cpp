#include "AdDetailsPage.h"
#include "ui_AdDetailsPage.h"
#include <QPixmap>

AdDetailsPage::AdDetailsPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AdDetailsPage)
    , m_currentAdId(-1)
{
    ui->setupUi(this);
    clear();
}

AdDetailsPage::~AdDetailsPage()
{
    delete ui;
}

void AdDetailsPage::setAdData(const QJsonObject& adData)
{
    m_currentAd = adData;
    m_currentAdId = adData["id"].toInt(-1);
    
    QString title = m_currentAd["title"].toString("");
    QString description = m_currentAd["description"].toString("");
    int price = m_currentAd["price"].toInt(0);
    
    QString infoText = QString("📱 %1\n%2\n💰 %3 Toman").arg(title).arg(description).arg(price);
    ui->lbl_info->setText(infoText);
    
    QString imagePath = m_currentAd["imagePath"].toString();
    if (!imagePath.isEmpty()) {
        QPixmap image(imagePath);
        if (!image.isNull()) {
            ui->lbl_image->setPixmap(image.scaled(131, 131, Qt::KeepAspectRatio));
        }
    }
    
    QString status = m_currentAd["status"].toString("available");
    bool isAvailable = (status == "available");
    ui->btn_add_to_cart->setEnabled(isAvailable);
    ui->btn_buy->setEnabled(isAvailable);
}

void AdDetailsPage::clear()
{
    m_currentAd = QJsonObject();
    m_currentAdId = -1;
    ui->lbl_image->clear();
    ui->lbl_info->setText("");
    ui->btn_add_to_cart->setEnabled(false);
    ui->btn_buy->setEnabled(false);
}

void AdDetailsPage::on_btn_back_clicked()
{
    emit backRequested();
}

void AdDetailsPage::on_btn_cart_clicked()
{
    emit cartRequested();
}

void AdDetailsPage::on_btn_add_to_cart_clicked()
{
    if (m_currentAdId > 0) {
        emit addToCartRequested(m_currentAdId);
    }
}

void AdDetailsPage::on_btn_buy_clicked()
{
    if (m_currentAdId > 0) {
        emit buyNowRequested(m_currentAdId);
    }
}