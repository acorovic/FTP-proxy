#include "proxy.h"

Proxy::Proxy()
{
    QObject::connect(&proxyServ, SIGNAL(notifyBrowserConnected()), &proxyClient, SLOT(connectToFtpServer()));
    QObject::connect(&proxyClient, SIGNAL(toProxyServer(QByteArray)), &proxyServ, SLOT(readProxyClientData(QByteArray)));
    QObject::connect(&proxyServ, SIGNAL(toProxyClient(QByteArray)), &proxyClient, SLOT(readProxyServerData(QByteArray)));


    qDebug() << "Proxy created!";
}
