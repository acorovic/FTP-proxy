#include "proxy.h"

Proxy::Proxy()
{
    QObject::connect(&proxyServ, SIGNAL(notifyBrowserConnected()), &proxyClient, SLOT(connectToFtpServer()));

    qDebug() << "Proxy created!";
}
