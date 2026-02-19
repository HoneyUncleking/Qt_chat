#pragma once
#include <QTcpSocket>
#include <QObject>
#include "Database.h"
#include "Protocol.h"

class ServerHandler : public QObject {
    Q_OBJECT
public:
    ServerHandler(qintptr socketDescriptor, QObject* parent = nullptr);
    ~ServerHandler();

private slots:
    void handleMessage();

signals:
    void disconnected();

private:
    QTcpSocket* socket;
    QString currentUser;
    
    void sendResponse(MessageType type, const QJsonObject& data = {});
    User* getCurrentUser();
};
