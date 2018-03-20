#include "proxyserv.h"

ProxyServ::ProxyServ(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(browserConnected()));

    if (!server->listen(QHostAddress::Any, 21))
    {
        qDebug() << "Server coudln't start";
        isServerStarted = false;
    }
    else
    {
        qDebug() << "Server started";
        isServerStarted = true;
    }
}

void ProxyServ::browserConnected()
{
    // Take a socket to a browser
    qDebug() << "Browser connected!";
    socket = server->nextPendingConnection();

    connect(socket, SIGNAL(disconnected()), this, SLOT(browserDisconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(browserWritten(qint64)));

    emit notifyBrowserConnected();
}

void ProxyServ::browserDisconnected()
{
    qDebug() << "Browser disconnected";
    socket->close();

    delete socket;
}

void ProxyServ::browserWritten(qint64)
{
    qDebug() << socket->readAll();
}
