#include <QCoreApplication>
#include <QTcpServer>
#include <QDebug>
#include "ServerHandler.h"
#include "Database.h"

class KalaNetServer : public QTcpServer {
    Q_OBJECT
public:
    KalaNetServer(QObject* parent = nullptr) : QTcpServer(parent) {}

protected:
    void incomingConnection(qintptr socketDescriptor) override {
        new ServerHandler(socketDescriptor, this);
    }
};

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    Database::init();
    
    KalaNetServer server;
    if (!server.listen(QHostAddress::Any, 5555)) {
        qDebug() << "Server failed to start!";
        return -1;
    }
    
    qDebug() << "🚀 KalaNet Server running on port 5555";
    qDebug() << "👤 Admin: admin/admin123";
    
    return app.exec();
}

#include "main.moc"
