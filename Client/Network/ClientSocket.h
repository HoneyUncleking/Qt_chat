#pragma once
#include <QTcpSocket>
#include <QTimer>
#include <QJsonDocument>
#include "../../Common/Protocol.h"

class ClientSocket : public QObject {
    Q_OBJECT

public:
    explicit ClientSocket(QObject* parent = nullptr);
    ~ClientSocket();
    void connectToServer();
    void sendMessage(const QJsonObject& data);
    bool isConnected() const;

signals:
    void connected();
    void disconnected();
    void messageReceived(const QJsonDocument& doc);
    void errorOccurred(const QString& error);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError(QAbstractSocket::SocketError error);

private:
    QTcpSocket* m_socket;
    QByteArray m_buffer;
    QTimer* m_reconnectTimer;
    void processBuffer();
};
