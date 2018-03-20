#include "proxyclient.h"


ProxyClient::ProxyClient(QObject *parent) : QObject(parent)
{

}

void ProxyClient::connectToFtpServer()
{
    socket = new QTcpSocket(this);
    qDebug() << "Connecting to FTP server...";

    connect(socket, SIGNAL(connected()), this, SLOT(connectedToFtpServer()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectedFtpServer()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readServerData()));
    QHostAddress ftp_serv_address;
    ftp_serv_address.setAddress(FTP_SERV_ADDRESS);

    socket->connectToHost(ftp_serv_address, FTP_SERV_PORT);
}

void ProxyClient::connectedToFtpServer()
{
    qDebug() << "Connected to FTP server!";
}

void ProxyClient::disconnectedFtpServer()
{
    qDebug() << "Disconnected from FTP server!";

    //delete socket;

    //connectToFtpServer();
}

void ProxyClient::readServerData()
{
    QByteArray receivedData;

    qDebug() << "Received from FTP server:";
    receivedData = socket->readAll();
    qDebug() << receivedData;

    emit toProxyServer(receivedData);
}

void ProxyClient::readProxyServerData(QByteArray data)
{
    socket->write(data);
    socket->flush();
}
