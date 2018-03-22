#include "proxyserv.h"

ProxyServ::ProxyServ(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    connect(server, SIGNAL(newConnection()), this, SLOT(browserConnected()));

    if (!server->listen(QHostAddress::Any, PROXY_PORT))
    {
        qDebug() << "Server coudln't start!";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void ProxyServ::browserConnected()
{ 
    qDebug() << "Browser connected!";
    if (socket != NULL)
    {
        delete socket;
    }
    // Take a socket to forward commands to the browser
    socket = server->nextPendingConnection();

    // Connect a signal to read commands from the browser
    connect(socket, SIGNAL(readyRead()), this, SLOT(readBrowserCommand()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(browserDisconnected()));

    emit notifyBrowserConnected();
}

void ProxyServ::browserDisconnected()
{
    qDebug() << "Browser disconnected";
}

void ProxyServ::readBrowserCommand()
{
    QByteArray receivedCommand;

    receivedCommand = socket->readAll();
    qDebug() << "Received from FTP client:";
    qDebug() << receivedCommand;

    emit toProxyClientCommand(receivedCommand);
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

//    if (dataSocket != NULL)
//    {
//        delete dataSocket;
//    }
    // Take a socket to forward data to the browser
    dataSocket = dataServer->nextPendingConnection();

    connect(dataSocket, SIGNAL(readyRead()), this, SLOT(readBrowserData()));
    connect(dataSocket, SIGNAL(disconnected()), this, SLOT(browserDataDisconnected()));
}

void ProxyServ::browserDataDisconnected()
{
    qDebug() << "Browser data line disconnected!";
}

void ProxyServ::readBrowserData()
{
    qDebug() << "Browser data line ready to be read!";
}

void ProxyServ::readProxyClientData(QByteArray data)
{
    // Forward the data to the browser
    dataSocket->write(data);
    dataSocket->flush();
}

void ProxyServ::readProxyClientCommand(QByteArray command)
{
    // Forward the command to the browser
    socket->write(command);
    socket->flush();

    if (command.contains("Transfer complete"))
    {
        qDebug() << "Closing data socket for the browser!";

        dataSocket->close();
        dataServer->close();
    }
}
