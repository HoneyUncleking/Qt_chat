#ifndef MAINMENUPAGE_H
#define MAINMENUPAGE_H

#include <QWidget>
#include <QListWidgetItem>

QT_BEGIN_NAMESPACE
namespace Ui { class MainMenuPage; }
QT_END_NAMESPACE

class MainMenuPage : public QWidget {
    Q_OBJECT

public:
    explicit MainMenuPage(QWidget *parent = nullptr);
    ~MainMenuPage();
    void setUsername(const QString& username);

signals:
    void navigateToPostAd();
    void navigateToViewAds();
    void navigateToCart();
    void navigateToWallet();
    void navigateToProfile();
    void logout();

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MainMenuPage *ui;
};

#endif