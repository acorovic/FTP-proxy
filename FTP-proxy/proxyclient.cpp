#include "proxyclient.h"

static bool connected = false;

ProxyClient::ProxyClient(QObject *parent) : QObject(parent)
{

}

void ProxyClient::connectToFtpServer()
{
    socket = new QTcpSocket(this);
    qDebug() << "Connecting to FTP server...";

    connect(socket, SIGNAL(connected()), this, SLOT(connectedToFtpServer()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectedFtpServer()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readServerCommand()));
    QHostAddress ftp_serv_address;
    ftp_serv_address.setAddress(FTP_SERV_ADDRESS);

    socket->connectToHost(ftp_serv_address, FTP_SERV_PORT);
}

void ProxyClient::connectedToFtpServer()
{
    qDebug() << "Connected to FTP server!";
    connected = true;
}

void ProxyClient::disconnectedFtpServer()
{
    qDebug() << "Disconnected from FTP server!";
    connected = false;

    //delete socket;

    //connectToFtpServer();
}

void ProxyClient::readServerCommand()
{
    QByteArray receivedData;

    qDebug() << "Received from FTP server:";
    receivedData = socket->readAll();
    qDebug() << receivedData;

    if (receivedData.contains("227 Entering Passive Mode"))
    {
        qDebug() << "------Passive Mode------";

        QByteArray changeData(receivedData);
        QHostAddress dataServAddress;
        int portNo;

        dataSocket = new QTcpSocket(this);
        connect(dataSocket, SIGNAL(connected()), this, SLOT(connectedToDataServer()));
        connect(dataSocket, SIGNAL(disconnected()), this, SLOT(disconnectedFromDataServer()));
        connect(dataSocket, SIGNAL(readyRead()), this, SLOT(readServerData()));

        // Change to parsed address
        dataServAddress.setAddress(FTP_SERV_ADDRESS);
        portNo = getPassivePort(&receivedData);
        dataSocket->connectToHost(dataServAddress, portNo);

        char* out = changeData.data();
        out[changeData.indexOf(')') - 3] = '8';

        emit createDataServer(8*256);
        emit toProxyServerCommand(changeData);
    }
//    else if (receivedData.contains("Transfer complete"))
//    {
//        qDebug() << "Upao------------------------------";
//        dataSocket->close();
//        emit toProxyServerCommand(receivedData);
//    }
    else
    {
        emit toProxyServerCommand(receivedData);
    }
}

void ProxyClient::readProxyServerCommand(QByteArray data)
{
    // Sometimes socket closes so need to fake BYE message
    if (data.contains("QUIT") && !connected)
    {
        qDebug("!!!!!!!!!!!!!!!!!!!!!!!!!!! NE MOZE UPISATI");
        emit toProxyServerCommand("221 Bye\r\n");
    }
    else
    {
        socket->write(data);
        socket->flush();
    }
}

void ProxyClient::connectedToDataServer()
{
    qDebug() << "Connected to data server!";
}

void ProxyClient::disconnectedFromDataServer()
{
    qDebug() << "Disconnected from data server!";
}

void ProxyClient::readServerData()
{
    qDebug() << "Ready to read from data server!";
    QByteArray receivedData;

    receivedData = dataSocket->readAll();
    emit toProxyServerData(receivedData);
}


int ProxyClient::getPassivePort(QByteArray *message)
{
    QString portStr;
    QString portOffset;
    int commaNo = 0;

    for (auto it = message->indexOf('('); it < message->indexOf(')'); it++)
    {

        if (message->at(it) == ',')
        {
            ++commaNo;
        }
        if (commaNo == 4)
        {
            if (message->at(it) != ',')
            {
                portStr.append(message->at(it));
            }
        }
        if (commaNo == 5)
        {
            if (message->at(it) != ',')
            {
                portOffset.append(message->at(it));
            }
        }
    }

    qDebug() << "Port No: " << portStr;
    qDebug() << "Port offset: " << portOffset;

    return portStr.toInt() * 256 + portOffset.toInt();
}

