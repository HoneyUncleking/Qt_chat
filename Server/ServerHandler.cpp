#include "ServerHandler.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QDebug>

ServerHandler::ServerHandler(qintptr socketDescriptor, QObject* parent)
    : QObject(parent), socket(nullptr) {
    
    socket = new QTcpSocket(this);
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        return;
    }
    
    connect(socket, &QTcpSocket::readyRead, this, &ServerHandler::handleMessage);
    connect(socket, &QTcpSocket::disconnected, this, &ServerHandler::deleteLater);
}

ServerHandler::~ServerHandler() {
    socket->disconnectFromHost();
}

void ServerHandler::sendResponse(MessageType type, const QJsonObject& data) {
    QJsonObject response = Protocol::createMessage(type, data);
    QJsonDocument doc(response);
    socket->write(doc.toJson());
    socket->write("\n");
    socket->flush();
}

User* ServerHandler::getCurrentUser() {
    return Database::getUser(currentUser);
}

void ServerHandler::handleMessage() {
    while (socket->bytesAvailable()) {
        QByteArray data = socket->readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        
        if (!doc.isObject()) continue;
        
        MessageType type = Protocol::getType(doc);
        QJsonObject payload = doc.object()["data"].toObject();
        
        switch (type) {
        case MessageType::LOGIN:
            {
                QString username = payload["username"].toString();
                QString password = payload["password"].toString();
                QString passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex();
                
                User* user = Database::getUser(username);
                if (user && user->passwordHash == passwordHash) {
                    currentUser = username;
                    QJsonObject data;
                    data["username"] = username;
                    data["balance"] = user->balance;
                    data["isAdmin"] = user->isAdmin;
                    data["success"] = true;
                    sendResponse(MessageType::LOGIN_RESPONSE, data);
                } else {
                    sendResponse(MessageType::LOGIN_RESPONSE, {{"success", false}, {"message", "Invalid credentials"}});
                }
                break;
            }
            
        case MessageType::SIGNUP:
            {
                User user;
                user.username = payload["username"].toString();
                user.passwordHash = QCryptographicHash::hash(payload["password"].toString().toUtf8(), QCryptographicHash::Sha256).toHex();
                user.phone = payload["phone"].toString();
                user.email = payload["email"].toString();
                user.name = payload["name"].toString();
                
                if (Database::addUser(user)) {
                    sendResponse(MessageType::SIGNUP_RESPONSE, {{"success", true}, {"message", "Account created"}});
                } else {
                    sendResponse(MessageType::SIGNUP_RESPONSE, {{"success", false}, {"message", "Username exists"}});
                }
                break;
            }
            
        case MessageType::GET_ADS:
            {
                QList<Ad> approvedAds = Database::getAds(Ad::APPROVED);
                QJsonArray adsArray;
                for (const auto& ad : approvedAds) {
                    QJsonObject adObj;
                    adObj["id"] = ad.id;
                    adObj["title"] = ad.title;
                    adObj["description"] = ad.description;
                    adObj["price"] = ad.price;
                    adObj["seller"] = ad.sellerUsername;
                    adsArray.append(adObj);
                }
                QJsonObject data;
                data["ads"] = adsArray;
                sendResponse(MessageType::ADS_LIST, data);
                break;
            }
            
        case MessageType::ADD_TOKEN:
            {
                if (User* user = Database::getUser(currentUser)) {
                    int amount = payload["amount"].toInt();
                    if (Database::updateBalance(currentUser, amount)) {
                        QJsonObject data;
                        data["balance"] = user->balance + amount;
                        sendResponse(MessageType::BALANCE_UPDATED, data);
                    }
                }
                break;
            }
            
        case MessageType::BUY_NOW:
            {
                int adId = payload["adId"].toInt();
                User* buyer = Database::getUser(currentUser);
                
                if (!buyer) {
                    sendResponse(MessageType::ERROR, {{"message", "User not found"}});
                    break;
                }
                
                // پیدا کردن آگهی
                QList<Ad> allAds = Database::getAds(Ad::APPROVED);
                Ad* adToBuy = nullptr;
                
                for (auto& ad : allAds) {
                    if (ad.id == adId) {
                        adToBuy = &ad;
                        break;
                    }
                }
                
                if (!adToBuy) {
                    sendResponse(MessageType::ERROR, {{"message", "Ad not available"}});
                    break;
                }
                
                // چک کردن موجودی
                if (buyer->balance < adToBuy->price) {
                    sendResponse(MessageType::ERROR, {{"message", "Insufficient balance"}});
                    break;
                }
                
                // پیدا کردن فروشنده
                User* seller = Database::getUserById(adToBuy->sellerId);
                if (!seller) {
                    sendResponse(MessageType::ERROR, {{"message", "Seller not found"}});
                    break;
                }
                
                // انجام تراکنش
                Database::updateBalance(currentUser, -adToBuy->price);
                Database::updateBalance(seller->username, adToBuy->price);
                Database::updateAdStatus(adId, Ad::SOLD);
                
                sendResponse(MessageType::SUCCESS, {{"message", "Purchase successful"}});
                break;
            }
            
        default:
            sendResponse(MessageType::ERROR, {{"error", "Unknown command"}});
            break;
        }
    }
}