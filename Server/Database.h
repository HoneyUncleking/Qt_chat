#pragma once
#include <QString>
#include <QMap>
#include <QMutex>
#include <QList>
#include <QJsonObject>

struct User {
    QString username;
    QString passwordHash;
    QString phone;
    QString email;
    QString name;
    int balance = 0;
    bool isAdmin = false;
};

struct Ad {
    int id = 0;
    QString title;
    QString description;
    QString category;
    QString imagePath;
    int price = 0;
    int sellerId = 0;
    QString sellerUsername;
    
    enum Status { PENDING, APPROVED, REJECTED, SOLD };
    Status status = PENDING;
};

class Database {
public:
    static void init();
    static User* getUser(const QString& username);
    static bool addUser(const User& user);
    static bool updateBalance(const QString& username, int delta);
    static QList<Ad> getAds(Ad::Status status = Ad::APPROVED);
    static bool addAd(const Ad& ad);
    static bool updateAdStatus(int adId, Ad::Status status);
    static User* getUserById(int id);
    static void saveToFile();
    static void loadFromFile();
    
private:
    static QMap<QString, User> users;
    static QList<Ad> ads;
    static QMutex mutex;
    static int nextAdId;
    static const QString USERS_FILE;
    static const QString ADS_FILE;
};