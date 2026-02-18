#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include "ClientSocket.h"
#include "pages/LoginPage.h"
#include "pages/SignupPage.h"
#include "pages/MainMenuPage.h"
#include "pages/PostAdPage.h"
#include "pages/AdsListPage.h"
#include "pages/AdDetailsPage.h"
#include "pages/CartPage.h"
#include "pages/WalletPage.h"
#include "pages/ProfilePage.h"
#include "pages/EditProfilePage.h"
#include "pages/HistoryPage.h"
#include "pages/ForgotPasswordPage.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // Navigation slots
    void navigateToLogin();
    void navigateToSignup();
    void navigateToForgotPassword();
    void navigateToMainMenu();
    void navigateToPostAd();
    void navigateToViewAds();
    void navigateToCart();
    void navigateToWallet();
    void navigateToProfile();
    void navigateToEditProfile();
    void navigateToHistory();
    void navigateToAdDetails(int adId);
    
    // Auth slots
    void handleLogin(const QString& username, const QString& password);
    void handleSignup(const QString& name, const QString& username, 
                      const QString& email, const QString& phone, 
                      const QString& password);
    void handleLogout();
    
    // Message handling
    void onServerMessage(const QJsonDocument& doc);
    void onServerConnected();
    void onServerDisconnected();
    void onServerError(const QString& error);
    
    // UI helpers
    void showMessage(const QString& title, const QString& message, bool isError);
    void showBusyIndicator(bool show);

private:
    void setupPages();
    void connectSignals();

    Ui::MainWindow *ui;
    ClientSocket* m_socket;
    QStackedWidget* m_stackedWidget;
    QString m_currentUsername;
    
    // Page pointers
    LoginPage* m_loginPage;
    SignupPage* m_signupPage;
    ForgotPasswordPage* m_forgotPasswordPage;
    MainMenuPage* m_mainMenuPage;
    PostAdPage* m_postAdPage;
    AdsListPage* m_adsListPage;
    AdDetailsPage* m_adDetailsPage;
    CartPage* m_cartPage;
    WalletPage* m_walletPage;
    ProfilePage* m_profilePage;
    EditProfilePage* m_editProfilePage;
    HistoryPage* m_historyPage;
};

#endif