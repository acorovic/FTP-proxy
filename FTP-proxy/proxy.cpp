#include "proxy.h"

Proxy::Proxy()
{
    QObject::connect(&proxyServ, SIGNAL(notifyBrowserConnected()), &proxyClient, SLOT(connectToFtpServer()));
    QObject::connect(&proxyServ, SIGNAL(toProxyClientCommand(QByteArray)), &proxyClient, SLOT(readProxyServerCommand(QByteArray)));
    QObject::connect(&proxyClient, SIGNAL(toProxyServerCommand(QByteArray)), &proxyServ, SLOT(readProxyClientCommand(QByteArray)));
    QObject::connect(&proxyClient, SIGNAL(createDataServer(int)), &proxyServ, SLOT(createDataServ(int)));
    QObject::connect(&proxyClient, SIGNAL(toProxyServerData(QByteArray)), &proxyServ, SLOT(readProxyClientData(QByteArray)));

    qDebug() << "Proxy created!";
}
