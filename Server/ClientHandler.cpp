#include "ClientHandler.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

ClientHandler::ClientHandler(qintptr socketDescriptor, QObject *parent)
    : QThread(parent),
      m_socketDescriptor(socketDescriptor)
{
}

static QJsonDocument loadUsers()
{
    QFile file("users.json");
    if (!file.open(QIODevice::ReadOnly))
        return QJsonDocument();

    return QJsonDocument::fromJson(file.readAll());
}

static void saveUsers(const QJsonDocument& doc)
{
    QFile file("users.json");
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson());
}

void ClientHandler::run()
{
    QTcpSocket socket;
    socket.setSocketDescriptor(m_socketDescriptor);

    if (!socket.waitForReadyRead())
        return;

    QString request = socket.readAll().trimmed();
    QStringList parts = request.split("|");

    QJsonDocument doc = loadUsers();
    QJsonArray users = doc.object()["users"].toArray();

    /* ================= LOGIN ================= */
    if (parts[0] == "LOGIN" && parts.size() == 3) {
        QString username = parts[1];
        QString hash = parts[2];

        for (const QJsonValue& v : users) {
            QJsonObject u = v.toObject();
            if (u["username"].toString() == username &&
                u["password"].toString() == hash) {

                socket.write("LOGIN_OK");
                socket.waitForBytesWritten();
                return;
            }
        }

        socket.write("LOGIN_FAIL");
        socket.waitForBytesWritten();
        return;
    }

    /* ================= SIGNUP ================= */
    if (parts[0] == "SIGNUP" && parts.size() == 6) {
        QString name = parts[1];
        QString username = parts[2];
        QString phone = parts[3];
        QString email = parts[4];
        QString hash = parts[5];

        for (const QJsonValue& v : users) {
            if (v.toObject()["username"].toString() == username) {
                socket.write("SIGNUP_EXISTS");
                socket.waitForBytesWritten();
                return;
            }
        }

        QJsonObject newUser;
        newUser["name"] = name;
        newUser["username"] = username;
        newUser["phone"] = phone;
        newUser["email"] = email;
        newUser["password"] = hash;
        newUser["role"] = "user";
        newUser["wallet"] = 0;

        users.append(newUser);

        QJsonObject root;
        root["users"] = users;
        saveUsers(QJsonDocument(root));

        socket.write("SIGNUP_OK");
        socket.waitForBytesWritten();
        return;
    }
}
