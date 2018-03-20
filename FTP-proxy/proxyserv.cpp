#include "proxyserv.h"

ProxyServ::ProxyServ(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(browserConnected()));

    listenForConnections();
}

void ProxyServ::listenForConnections()
{
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
    connect(socket, SIGNAL(readyRead()), this, SLOT(readBrowserData()));

    emit notifyBrowserConnected();
}

void ProxyServ::browserDisconnected()
{
    qDebug() << "Browser disconnected";
    //socket->close();

    //listenForConnections();
}

void ProxyServ::readBrowserData()
{
    QByteArray receivedData;

    qDebug() << "Received from FTP client:";
    receivedData = socket->readAll();
    qDebug() << receivedData;

    emit toProxyClient(receivedData);
}

void ProxyServ::readProxyClientData(QByteArray data)
{

    //qDebug() << "FTP proxyServ forwarding to client:";
    //qDebug() << data;

    socket->write(data);
    socket->flush();
}

