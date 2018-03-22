#ifndef PROXY_H
#define PROXY_H

#include <QObject>
#include <QDebug>

#include "proxyserv.h"
#include "proxyclient.h"


class Proxy
{
public:
    Proxy();
private:
    ProxyServ proxyServ;
    ProxyClient proxyClient;
};

#endif // PROXY_H
