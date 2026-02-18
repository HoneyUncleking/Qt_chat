#include "MainMenuPage.h"
#include "ui_MainMenuPage.h"

MainMenuPage::MainMenuPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainMenuPage)
{
    ui->setupUi(this);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainMenuPage::on_listWidget_itemClicked);
}

MainMenuPage::~MainMenuPage()
{
    delete ui;
}

void MainMenuPage::setUsername(const QString& username)
{
    ui->label->setText("Welcome, " + username + "!");
}

void MainMenuPage::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString text = item->text();
    
    if (text.contains("Post Ad")) {
        emit navigateToPostAd();
    } else if (text.contains("View Ads")) {
        emit navigateToViewAds();
    } else if (text.contains("Shopping Cart")) {
        emit navigateToCart();
    } else if (text.contains("Wallet")) {
        emit navigateToWallet();
    } else if (text.contains("Profile")) {
        emit navigateToProfile();
    } else if (text.contains("Exit")) {
        emit logout();
    }
}