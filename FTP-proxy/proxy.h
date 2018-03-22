#ifndef PROXY_H
#define PROXY_H

#include <QObject>
#include <QDebug>

#include "proxyserv.h"
#include "proxyclient.h"

/*!
 * \brief The Proxy class
 */
class Proxy
{
public:
    /*!
     * \brief Proxy ctor
     */
    Proxy();

    /*!
     * \brief Proxy dctor
     */
    ~Proxy() = default;
private:
    /*!
     * \brief proxyServ - browser connects to it
     */
    ProxyServ proxyServ;

    /*!
     * \brief proxyClient - it connects to real FTP server
     */
    ProxyClient proxyClient;
};

#endif // PROXY_H
