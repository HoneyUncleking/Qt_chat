#pragma once
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>

enum class MessageType {
    // Authentication (1-10)
    LOGIN = 1,
    SIGNUP,
    LOGIN_RESPONSE,
    SIGNUP_RESPONSE,
    
    // Profile (11-20)
    GET_PROFILE = 11,
    UPDATE_PROFILE,
    PROFILE_DATA,
    
    // Wallet (21-30)
    GET_BALANCE = 21,
    ADD_TOKEN,
    BALANCE_UPDATED,
    
    // Ads (31-50)
    CREATE_AD = 31,
    GET_ADS,
    GET_MY_ADS,
    GET_AD_DETAILS,
    ADS_LIST,
    AD_CREATED,
    UPDATE_AD_STATUS,
    
    // Cart (51-60)
    ADD_TO_CART = 51,
    REMOVE_FROM_CART,
    GET_CART,
    CLEAR_CART,
    CART_DATA,
    
    // Purchase (61-70)
    BUY_NOW = 61,
    CHECKOUT,
    PURCHASE_COMPLETE,
    
    // History (71-80)
    GET_HISTORY = 71,
    HISTORY_DATA,
    
    // Admin (81-90)
    ADMIN_GET_PENDING_ADS = 81,
    ADMIN_APPROVE_AD,
    ADMIN_REJECT_AD,
    ADMIN_GET_STATS,
    ADMIN_STATS,
    
    // General (91-100)
    SUCCESS = 91, 
    ERROR,
    PING,
    PONG,
    DISCONNECT
};

class Protocol {
public:
    static QJsonObject createMessage(MessageType type, const QJsonObject& data = {}) {
        QJsonObject msg;
        msg["type"] = static_cast<int>(type);
        msg["data"] = data;
        msg["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        return msg;
    }
    
    static MessageType getType(const QJsonDocument& doc) {
        return static_cast<MessageType>(doc.object()["type"].toInt());
    }
    
    static QJsonObject getData(const QJsonDocument& doc) {
        return doc.object()["data"].toObject();
    }
    
    static QJsonObject getData(const QJsonObject& obj) {
        return obj["data"].toObject();
    }
};
