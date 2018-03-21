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
    connect(socket, SIGNAL(readyRead()), this, SLOT(readBrowserCommand()));

    emit notifyBrowserConnected();
}

void ProxyServ::browserDisconnected()
{
    qDebug() << "Browser disconnected";
    //socket->close();

    //listenForConnections();
}

void ProxyServ::readBrowserCommand()
{
    QByteArray receivedData;

    qDebug() << "Received from FTP client:";
    receivedData = socket->readAll();
    qDebug() << receivedData;

    emit toProxyClientCommand(receivedData);
}

void ProxyServ::readProxyClientCommand(QByteArray data)
{

    //qDebug() << "FTP proxyServ forwarding to client:";
    //qDebug() << data;

    socket->write(data);
    socket->flush();

    if (data.contains("Transfer complete"))
    {
        qDebug() << "Closing data socket for the browser!";
        dataSocket->close();
        dataServer->close();
        //delete dataServer;
    }
}

void ProxyServ::readProxyClientData(QByteArray data)
{
    dataSocket->write(data);
    dataSocket->flush();
}

void ProxyServ::createDataServ(int portNo)
{
    if (dataServer != NULL)
    {
        delete dataServer;
    }

    dataServer = new QTcpServer(this);

    connect(dataServer, SIGNAL(newConnection()), this, SLOT(browserDataConnected()));

    qDebug() << "New data portNo : " << portNo;

    // Change to listen to specific ip address from message
    if (!dataServer->listen(QHostAddress::Any, portNo))
    {
        qDebug() << "DataServer coudln't start";
    }
    else
    {
        qDebug() << "DataServer started";
    }
}

void ProxyServ::browserDataConnected()
{
    qDebug() << "Data connection with browser established!";

    dataSocket = dataServer->nextPendingConnection();

    connect(dataSocket, SIGNAL(disconnected()), this, SLOT(browserDataDisconnected()));
    connect(dataSocket, SIGNAL(readyRead()), this, SLOT(readBrowserData()));
}

void ProxyServ::browserDataDisconnected()
{
    qDebug() << "Browser data line disconnected!";
}

void ProxyServ::readBrowserData()
{
    qDebug() << "Browser data line ready to be read!";
}

