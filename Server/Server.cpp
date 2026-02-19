#include "Server.h"
#include "ClientHandler.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    ClientHandler* handler = new ClientHandler(socketDescriptor, this);
    handler->start();
}
