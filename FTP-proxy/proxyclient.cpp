#include "proxyclient.h"

ProxyClient::ProxyClient(QObject *parent) : QObject(parent)
{

}

void ProxyClient::connectToFtpServer()
{
    socket = new QTcpSocket(this);
    qDebug() << "Connecting to FTP server...";

    connect(socket, SIGNAL(readyRead()), this, SLOT(readServerCommand()));
    connect(socket, SIGNAL(connected()), this, SLOT(connectedToFtpServer()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnectedFtpServer()));

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
        connect(dataSocket, SIGNAL(readyRead()), this, SLOT(readServerData()));
        connect(dataSocket, SIGNAL(connected()), this, SLOT(connectedToDataServer()));
        connect(dataSocket, SIGNAL(disconnected()), this, SLOT(disconnectedFromDataServer()));

        // Connect to original data socket
        dataServAddress.setAddress(FTP_SERV_ADDRESS);
        portNo = getPassivePort(&receivedData);
        dataSocket->connectToHost(dataServAddress, portNo);

        // Change the PASSV message (port for data server in proxyServ)
        changeData = setDataProxyPort(&changeData);
        qDebug() << "PASSV message to be forwarded to the browser:";
        qDebug() << changeData;

        emit createDataServer(DATA_SERV_PORT);

        // Forward changed message to proxyServ
        emit toProxyServerCommand(changeData);
    }
    else if (receivedData.contains("Transfer complete"))
    {
        // Wait for the data to be received
        // Transfer complete message will be sent when all data is received (disconnectedFromDataServer())
        dataSocket->waitForReadyRead();
    }
    else
    {
        // Just forward the command
        emit toProxyServerCommand(receivedData);
    }
}

void ProxyClient::readProxyServerCommand(QByteArray command)
{
    // Sometimes socket closes earlier so need we need to mannualy send Bye message
    if (command.contains("QUIT") && !connected)
    {
        emit toProxyServerCommand("221 Bye\r\n");
    }
    // Just forward the command to the FTP server
    else
    {
        socket->write(command);
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

    QByteArray message("226 Transfer complete\r\n");
    emit toProxyServerCommand(message);
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

QByteArray ProxyClient::setDataProxyPort(QByteArray *message)
{
    QByteArray firstPart = *message;
    int firstPartEnd = 0;
    int commaNo = 0;

    for (auto it = message->indexOf('('); it < message->indexOf(')'); it++)
    {
        if (message->at(it) == ',')
        {
            if (++commaNo == 4)
            {
                firstPartEnd = it;
            }
        }
    }

    firstPart.truncate(firstPartEnd + 1);

    int portNo = DATA_SERV_PORT/256;
    int portOffset = DATA_SERV_PORT - portNo*256;
    QByteArray byteArray;

    byteArray.setNum(portNo);
    firstPart.append(byteArray);
    firstPart.append(',');
    byteArray.setNum(portOffset);
    firstPart.append(byteArray);
    firstPart.append(")\r\n");

    return firstPart;
}

