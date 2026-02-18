#include "ClientSocket.h"
#include <QDebug>

ClientSocket::ClientSocket(QObject* parent) : QObject(parent), m_buffer(""), m_reconnectTimer(new QTimer(this)) {
    m_socket = new QTcpSocket(this);
    connect(m_socket, &QTcpSocket::connected, this, &ClientSocket::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &ClientSocket::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &ClientSocket::onReadyRead);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), this, &ClientSocket::onError);
    connect(m_reconnectTimer, &QTimer::timeout, this, &ClientSocket::connectToServer);
    m_reconnectTimer->setInterval(3000);
}

ClientSocket::~ClientSocket() { m_socket->disconnectFromHost(); }

void ClientSocket::connectToServer() { m_socket->connectToHost("127.0.0.1", 5555); }

bool ClientSocket::isConnected() const { return m_socket->state() == QAbstractSocket::ConnectedState; }

void ClientSocket::sendMessage(const QJsonObject& data) {
    if (!isConnected()) { emit errorOccurred("Not connected"); return; }
    QJsonObject msg = Protocol::createMessage(MessageType::ADD_TOKEN, data);
    QByteArray jsonData = QJsonDocument(msg).toJson(QJsonDocument::Compact) + "\n";
    m_socket->write(jsonData);
}

void ClientSocket::onConnected() { emit connected(); m_reconnectTimer->stop(); }
void ClientSocket::onDisconnected() { emit disconnected(); m_reconnectTimer->start(); }
void ClientSocket::onError(QAbstractSocket::SocketError) { emit errorOccurred(m_socket->errorString()); }

void ClientSocket::onReadyRead() {
    m_buffer.append(m_socket->readAll());
    processBuffer();
}

void ClientSocket::processBuffer() {
    int pos;
    while ((pos = m_buffer.indexOf('\n')) != -1) {
        QByteArray line = m_buffer.left(pos);
        m_buffer.remove(0, pos + 1);
        QJsonDocument doc = QJsonDocument::fromJson(line);
        if (!doc.isNull()) emit messageReceived(doc);
    }
}
