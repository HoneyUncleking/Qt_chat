#include "Database.h"
#include <QCryptographicHash>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QDir>

QMap<QString, User> Database::users;
QList<Ad> Database::ads;
QMutex Database::mutex;
int Database::nextAdId = 1;
const QString Database::USERS_FILE = "users.json";
const QString Database::ADS_FILE = "ads.json";

void Database::init() {
    QMutexLocker lock(&mutex);
    
    // Load existing data
    loadFromFile();
    
    // Add default admin if not exists
    if (!users.contains("admin")) {
        User admin;
        admin.username = "admin";
        admin.passwordHash = QCryptographicHash::hash(QString("admin123").toUtf8(), QCryptographicHash::Sha256).toHex();
        admin.name = "Administrator";
        admin.phone = "0000000000";
        admin.email = "admin@kalanet.com";
        admin.balance = 1000000;
        admin.isAdmin = true;
        users["admin"] = admin;
        saveToFile();
    }
    
    // Set nextAdId based on existing ads
    for (const auto& ad : ads) {
        if (ad.id >= nextAdId) {
            nextAdId = ad.id + 1;
        }
    }
}

void Database::saveToFile() {
    // Save users
    QJsonArray usersArray;
    for (auto it = users.begin(); it != users.end(); ++it) {
        QJsonObject userObj;
        userObj["username"] = it.value().username;
        userObj["passwordHash"] = it.value().passwordHash;
        userObj["name"] = it.value().name;
        userObj["phone"] = it.value().phone;
        userObj["email"] = it.value().email;
        userObj["balance"] = it.value().balance;
        userObj["isAdmin"] = it.value().isAdmin;
        usersArray.append(userObj);
    }
    
    QJsonObject root;
    root["users"] = usersArray;
    
    QFile usersFile(USERS_FILE);
    if (usersFile.open(QIODevice::WriteOnly)) {
        usersFile.write(QJsonDocument(root).toJson());
        usersFile.close();
    }
    
    // Save ads
    QJsonArray adsArray;
    for (const auto& ad : ads) {
        QJsonObject adObj;
        adObj["id"] = ad.id;
        adObj["title"] = ad.title;
        adObj["description"] = ad.description;
        adObj["category"] = ad.category;
        adObj["imagePath"] = ad.imagePath;
        adObj["price"] = ad.price;
        adObj["sellerId"] = ad.sellerId;
        adObj["sellerUsername"] = ad.sellerUsername;
        adObj["status"] = static_cast<int>(ad.status);
        adsArray.append(adObj);
    }
    
    QJsonObject adsRoot;
    adsRoot["ads"] = adsArray;
    adsRoot["nextId"] = nextAdId;
    
    QFile adsFile(ADS_FILE);
    if (adsFile.open(QIODevice::WriteOnly)) {
        adsFile.write(QJsonDocument(adsRoot).toJson());
        adsFile.close();
    }
}

void Database::loadFromFile() {
    // Load users
    QFile usersFile(USERS_FILE);
    if (usersFile.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(usersFile.readAll());
        QJsonArray usersArray = doc.object()["users"].toArray();
        
        for (const auto& val : usersArray) {
            QJsonObject obj = val.toObject();
            User user;
            user.username = obj["username"].toString();
            user.passwordHash = obj["passwordHash"].toString();
            user.name = obj["name"].toString();
            user.phone = obj["phone"].toString();
            user.email = obj["email"].toString();
            user.balance = obj["balance"].toInt();
            user.isAdmin = obj["isAdmin"].toBool();
            users[user.username] = user;
        }
        usersFile.close();
    }
    
    // Load ads
    QFile adsFile(ADS_FILE);
    if (adsFile.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(adsFile.readAll());
        QJsonArray adsArray = doc.object()["ads"].toArray();
        nextAdId = doc.object()["nextId"].toInt(1);
        
        for (const auto& val : adsArray) {
            QJsonObject obj = val.toObject();
            Ad ad;
            ad.id = obj["id"].toInt();
            ad.title = obj["title"].toString();
            ad.description = obj["description"].toString();
            ad.category = obj["category"].toString();
            ad.imagePath = obj["imagePath"].toString();
            ad.price = obj["price"].toInt();
            ad.sellerId = obj["sellerId"].toInt();
            ad.sellerUsername = obj["sellerUsername"].toString();
            ad.status = static_cast<Ad::Status>(obj["status"].toInt());
            ads.append(ad);
        }
        adsFile.close();
    }
}

User* Database::getUser(const QString& username) {
    QMutexLocker lock(&mutex);
    auto it = users.find(username);
    if (it != users.end()) {
        return &it.value();
    }
    return nullptr;
}

bool Database::addUser(const User& user) {
    QMutexLocker lock(&mutex);
    if (users.contains(user.username)) return false;
    users[user.username] = user;
    saveToFile();
    return true;
}

bool Database::updateBalance(const QString& username, int delta) {
    QMutexLocker lock(&mutex);
    auto it = users.find(username);
    if (it != users.end()) {
        it.value().balance += delta;
        saveToFile();
        return true;
    }
    return false;
}

QList<Ad> Database::getAds(Ad::Status status) {
    QMutexLocker lock(&mutex);
    QList<Ad> result;
    for (const auto& ad : ads) {
        if (ad.status == status) result.append(ad);
    }
    return result;
}

bool Database::addAd(const Ad& ad) {
    QMutexLocker lock(&mutex);
    Ad newAd = ad;
    newAd.id = nextAdId++;
    ads.append(newAd);
    saveToFile();
    return true;
}

bool Database::updateAdStatus(int adId, Ad::Status status) {
    QMutexLocker lock(&mutex);
    for (auto& ad : ads) {
        if (ad.id == adId) {
            ad.status = status;
            saveToFile();
            return true;
        }
    }
    return false;
}

User* Database::getUserById(int id) {
    QMutexLocker lock(&mutex);
    for (auto& pair : users) {
        if (pair.balance == id) return &pair;
    }
    return nullptr;
}