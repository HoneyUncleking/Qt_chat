#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_socket(new ClientSocket(this))
    , m_stackedWidget(new QStackedWidget(this))
{
    ui->setupUi(this);
    
    // Set central widget
    setCentralWidget(m_stackedWidget);
    setWindowTitle("KalaNet - Online Marketplace");
    resize(800, 600);
    
    // Setup socket connections
    connect(m_socket, &ClientSocket::messageReceived, 
            this, &MainWindow::onServerMessage);
    connect(m_socket, &ClientSocket::connected, 
            this, &MainWindow::onServerConnected);
    connect(m_socket, &ClientSocket::disconnected, 
            this, &MainWindow::onServerDisconnected);
    connect(m_socket, &ClientSocket::errorOccurred, 
            this, &MainWindow::onServerError);
    
    // Create all pages
    setupPages();
    
    // Connect to server
    m_socket->connectToServer();
    
    // Start with login page
    navigateToLogin();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPages()
{
    // Create pages
    m_loginPage = new LoginPage(this);
    m_signupPage = new SignupPage(this);
    m_forgotPasswordPage = new ForgotPasswordPage(this);
    m_mainMenuPage = new MainMenuPage(this);
    m_postAdPage = new PostAdPage(this);
    m_adsListPage = new AdsListPage(this);
    m_adDetailsPage = new AdDetailsPage(this);
    m_cartPage = new CartPage(this);
    m_walletPage = new WalletPage(this);
    m_profilePage = new ProfilePage(this);
    m_editProfilePage = new EditProfilePage(this);
    m_historyPage = new HistoryPage(this);
    
    // Add to stacked widget
    m_stackedWidget->addWidget(m_loginPage);
    m_stackedWidget->addWidget(m_signupPage);
    m_stackedWidget->addWidget(m_forgotPasswordPage);
    m_stackedWidget->addWidget(m_mainMenuPage);
    m_stackedWidget->addWidget(m_postAdPage);
    m_stackedWidget->addWidget(m_adsListPage);
    m_stackedWidget->addWidget(m_adDetailsPage);
    m_stackedWidget->addWidget(m_cartPage);
    m_stackedWidget->addWidget(m_walletPage);
    m_stackedWidget->addWidget(m_profilePage);
    m_stackedWidget->addWidget(m_editProfilePage);
    m_stackedWidget->addWidget(m_historyPage);
    
    // Connect page signals
    connectSignals();
}

void MainWindow::connectSignals()
{
    // Login page
    connect(m_loginPage, &LoginPage::navigateToSignup, 
            this, &MainWindow::navigateToSignup);
    connect(m_loginPage, &LoginPage::navigateToForgotPassword, 
            this, &MainWindow::navigateToForgotPassword);
    connect(m_loginPage, &LoginPage::loginRequested, 
            this, &MainWindow::handleLogin);
    connect(m_loginPage, &LoginPage::showMessage, 
            this, &MainWindow::showMessage);
    
    // Signup page
    connect(m_signupPage, &SignupPage::navigateToLogin, 
            this, &MainWindow::navigateToLogin);
    connect(m_signupPage, &SignupPage::signupRequested, 
            this, &MainWindow::handleSignup);
    connect(m_signupPage, &SignupPage::showMessage, 
            this, &MainWindow::showMessage);
    
    // Forgot password page
    connect(m_forgotPasswordPage, &ForgotPasswordPage::navigateToLogin, 
            this, &MainWindow::navigateToLogin);
    
    // Main menu
    connect(m_mainMenuPage, &MainMenuPage::navigateToPostAd, 
            this, &MainWindow::navigateToPostAd);
    connect(m_mainMenuPage, &MainMenuPage::navigateToViewAds, 
            this, &MainWindow::navigateToViewAds);
    connect(m_mainMenuPage, &MainMenuPage::navigateToCart, 
            this, &MainWindow::navigateToCart);
    connect(m_mainMenuPage, &MainMenuPage::navigateToWallet, 
            this, &MainWindow::navigateToWallet);
    connect(m_mainMenuPage, &MainMenuPage::navigateToProfile, 
            this, &MainWindow::navigateToProfile);
    connect(m_mainMenuPage, &MainMenuPage::logout, 
            this, &MainWindow::handleLogout);
    
    // Post ad page
    connect(m_postAdPage, &PostAdPage::backRequested, 
            this, &MainWindow::navigateToMainMenu);
    connect(m_postAdPage, &PostAdPage::postAdRequested, 
            [this](const QJsonObject& adData) {
        QJsonObject data = adData;
        data["username"] = m_currentUsername;
        m_socket->sendMessage(Protocol::createMessage(MessageType::CREATE_AD, data));
    });
    
    // Ads list page
    connect(m_adsListPage, &AdsListPage::adClicked, 
            this, &MainWindow::navigateToAdDetails);
    connect(m_adsListPage, &AdsListPage::searchClicked, 
            [this](const QString& text) {
        // Handle search
        QJsonObject data;
        data["search"] = text;
        m_socket->sendMessage(Protocol::createMessage(MessageType::GET_ADS, data));
    });
    
    // Ad details page
    connect(m_adDetailsPage, &AdDetailsPage::backRequested, 
            this, &MainWindow::navigateToViewAds);
    connect(m_adDetailsPage, &AdDetailsPage::cartRequested, 
            this, &MainWindow::navigateToCart);
    connect(m_adDetailsPage, &AdDetailsPage::addToCartRequested, 
            [this](int adId) {
        QJsonObject data;
        data["adId"] = adId;
        data["username"] = m_currentUsername;
        m_socket->sendMessage(Protocol::createMessage(MessageType::ADD_TO_CART, data));
        showMessage("Cart", "Item added to cart!", false);
    });
    connect(m_adDetailsPage, &AdDetailsPage::buyNowRequested, 
            [this](int adId) {
        QJsonObject data;
        data["adId"] = adId;
        data["username"] = m_currentUsername;
        m_socket->sendMessage(Protocol::createMessage(MessageType::BUY_NOW, data));
    });
    
    // Cart page
    connect(m_cartPage, &CartPage::backRequested, 
            this, &MainWindow::navigateToMainMenu);
    connect(m_cartPage, &CartPage::checkoutRequested, 
            [this]() {
        QJsonObject data;
        data["username"] = m_currentUsername;
        m_socket->sendMessage(Protocol::createMessage(MessageType::CHECKOUT, data));
    });
    connect(m_cartPage, &CartPage::removeItemRequested, 
            [this](int itemId) {
        QJsonObject data;
        data["itemId"] = itemId;
        data["username"] = m_currentUsername;
        m_socket->sendMessage(Protocol::createMessage(MessageType::REMOVE_FROM_CART, data));
    });
    
    // Wallet page
    connect(m_walletPage, &WalletPage::backRequested, 
            this, &MainWindow::navigateToMainMenu);
    connect(m_walletPage, &WalletPage::chargeRequested, 
            [this]() {
        // Open add token dialog
        // You'll need to create this dialog
    });
    
    // Profile page
    connect(m_profilePage, &ProfilePage::backRequested, 
            this, &MainWindow::navigateToMainMenu);
    connect(m_profilePage, &ProfilePage::editRequested, 
            this, &MainWindow::navigateToEditProfile);
    connect(m_profilePage, &ProfilePage::historyRequested, 
            this, &MainWindow::navigateToHistory);
    connect(m_profilePage, &ProfilePage::walletRequested, 
            this, &MainWindow::navigateToWallet);
    
    // Edit profile page
    connect(m_editProfilePage, &EditProfilePage::backRequested, 
            this, &MainWindow::navigateToProfile);
    connect(m_editProfilePage, &EditProfilePage::saveRequested, 
            [this](const QJsonObject& data) {
        QJsonObject updateData = data;
        updateData["username"] = m_currentUsername;
        m_socket->sendMessage(Protocol::createMessage(MessageType::UPDATE_PROFILE, updateData));
    });
    
    // History page
    connect(m_historyPage, &HistoryPage::backRequested, 
            this, &MainWindow::navigateToProfile);
}

void MainWindow::navigateToLogin()
{
    m_stackedWidget->setCurrentWidget(m_loginPage);
}

void MainWindow::navigateToSignup()
{
    m_stackedWidget->setCurrentWidget(m_signupPage);
}

void MainWindow::navigateToForgotPassword()
{
    m_stackedWidget->setCurrentWidget(m_forgotPasswordPage);
}

void MainWindow::navigateToMainMenu()
{
    m_mainMenuPage->setUsername(m_currentUsername);
    m_stackedWidget->setCurrentWidget(m_mainMenuPage);
}

void MainWindow::navigateToPostAd()
{
    m_stackedWidget->setCurrentWidget(m_postAdPage);
}

void MainWindow::navigateToViewAds()
{
    m_stackedWidget->setCurrentWidget(m_adsListPage);
    // Request ads from server
    m_socket->sendMessage(Protocol::createMessage(MessageType::GET_ADS));
}

void MainWindow::navigateToCart()
{
    m_stackedWidget->setCurrentWidget(m_cartPage);
    // Request cart data
    QJsonObject data;
    data["username"] = m_currentUsername;
    m_socket->sendMessage(Protocol::createMessage(MessageType::GET_CART, data));
}

void MainWindow::navigateToWallet()
{
    m_stackedWidget->setCurrentWidget(m_walletPage);
    // Request balance
    QJsonObject data;
    data["username"] = m_currentUsername;
    m_socket->sendMessage(Protocol::createMessage(MessageType::GET_BALANCE, data));
}

void MainWindow::navigateToProfile()
{
    m_stackedWidget->setCurrentWidget(m_profilePage);
    // Request profile data
    QJsonObject data;
    data["username"] = m_currentUsername;
    m_socket->sendMessage(Protocol::createMessage(MessageType::GET_PROFILE, data));
}

void MainWindow::navigateToEditProfile()
{
    m_stackedWidget->setCurrentWidget(m_editProfilePage);
    // Load current profile data
    QJsonObject data;
    data["username"] = m_currentUsername;
    m_socket->sendMessage(Protocol::createMessage(MessageType::GET_PROFILE, data));
}

void MainWindow::navigateToHistory()
{
    m_stackedWidget->setCurrentWidget(m_historyPage);
    // Request history
    QJsonObject data;
    data["username"] = m_currentUsername;
    m_socket->sendMessage(Protocol::createMessage(MessageType::GET_HISTORY, data));
}

void MainWindow::navigateToAdDetails(int adId)
{
    m_stackedWidget->setCurrentWidget(m_adDetailsPage);
    // Request ad details
    QJsonObject data;
    data["adId"] = adId;
    m_socket->sendMessage(Protocol::createMessage(MessageType::GET_AD_DETAILS, data));
}

void MainWindow::handleLogin(const QString& username, const QString& password)
{
    m_currentUsername = username;
    QJsonObject data;
    data["username"] = username;
    data["password"] = password;
    m_socket->sendMessage(Protocol::createMessage(MessageType::LOGIN, data));
    showBusyIndicator(true);
}

void MainWindow::handleSignup(const QString& name, const QString& username, 
                             const QString& email, const QString& phone, 
                             const QString& password)
{
    QJsonObject data;
    data["name"] = name;
    data["username"] = username;
    data["email"] = email;
    data["phone"] = phone;
    data["password"] = password;
    m_socket->sendMessage(Protocol::createMessage(MessageType::SIGNUP, data));
    showBusyIndicator(true);
}

void MainWindow::handleLogout()
{
    m_currentUsername.clear();
    m_loginPage->clear();
    navigateToLogin();
}

void MainWindow::onServerMessage(const QJsonDocument& doc)
{
    showBusyIndicator(false);
    
    MessageType type = Protocol::getType(doc);
    QJsonObject data = Protocol::getData(doc);
    
    switch(type) {
        case MessageType::LOGIN_RESPONSE:
            if (data["success"].toBool()) {
                navigateToMainMenu();
            } else {
                showMessage("Error", data["message"].toString(), true);
            }
            break;
            
        case MessageType::SIGNUP_RESPONSE:
            if (data["success"].toBool()) {
                showMessage("Success", "Account created successfully!", false);
                navigateToLogin();
            } else {
                showMessage("Error", data["message"].toString(), true);
            }
            break;
            
        case MessageType::PROFILE_DATA:
            m_profilePage->setProfileData(data);
            m_editProfilePage->setProfileData(data);
            break;
            
        case MessageType::BALANCE_UPDATED:
            m_walletPage->setBalance(data["balance"].toInt());
            break;
            
        case MessageType::ADS_LIST:
            // Parse and show ads
            // m_adsListPage->setAds(data["ads"].toArray());
            break;
            
        case MessageType::CART_DATA:
            m_cartPage->setCartData(data["items"].toArray());
            break;
            
        case MessageType::HISTORY_DATA:
            m_historyPage->setHistoryData(data["history"].toArray());
            break;
            
        case MessageType::GET_AD_DETAILS:
            m_adDetailsPage->setAdData(data);
            break;
            
        case MessageType::ERROR:
            showMessage("Error", data["message"].toString(), true);
            break;
            
        case MessageType::SUCCESS:
            showMessage("Success", data["message"].toString(), false);
            break;
            
        default:
            qDebug() << "Unknown message type:" << static_cast<int>(type);
    }
}

void MainWindow::onServerConnected()
{
    qDebug() << "Connected to server";
}

void MainWindow::onServerDisconnected()
{
    qDebug() << "Disconnected from server";
    if (!m_currentUsername.isEmpty()) {
        showMessage("Connection Lost", "Reconnecting...", true);
    }
}

void MainWindow::onServerError(const QString& error)
{
    showMessage("Connection Error", error, true);
}

void MainWindow::showMessage(const QString& title, const QString& message, bool isError)
{
    if (isError) {
        QMessageBox::critical(this, title, message);
    } else {
        QMessageBox::information(this, title, message);
    }
}

void MainWindow::showBusyIndicator(bool show)
{
    if (show) {
        setCursor(Qt::WaitCursor);
    } else {
        setCursor(Qt::ArrowCursor);
    }
}