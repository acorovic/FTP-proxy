#include "test.h"

Test::Test(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if (!server->listen(QHostAddress::Any, 21))
    {
        qDebug() << "Server couldn't start";
    }
    else
    {
        qDebug() << "Server started";
    }
}

void Test::newConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    qDebug() << "Client connected";
}
