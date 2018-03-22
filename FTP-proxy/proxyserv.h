#ifndef PROXYSERV_H
#define PROXYSERV_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QByteArray>

#define PROXY_PORT 21       //!< Port on which will browser be connected

/*!
 * \brief The ProxyServ class
 */
class ProxyServ : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief ProxyServ ctor
     * \param parent
     */
    explicit ProxyServ(QObject *parent = nullptr);

signals:
    /*!
     * \brief notifyBrowserConnected - then proxyClient connects to the FTP server
     */
    void notifyBrowserConnected();

    /*!
     * \brief toProxyClientCommand - forwarding commands from the browser
     */
    void toProxyClientCommand(QByteArray);

public slots:
    // Slots for the browser command socket
    /*!
     * \brief browserConnected - take the socket to the connected browser
     */
    void browserConnected();

    /*!
     * \brief browserDisconnected
     */
    void browserDisconnected();

    /*!
     * \brief readBrowserCommand - forwarding command to the proxyClient
     */
    void readBrowserCommand();

    // Slots for the browser data socket
    /*!
     * \brief createDataServ - called when FTP server enters passive mode
     * \param portNo - port on which the data from FTP server will be sent to browser
     */
    void createDataServ(int portNo);

    /*!
     * \brief browserDataConnected - take a socket to forward the data to the browser
     */
    void browserDataConnected();

    /*!
     * \brief browserDataDisconnected
     */
    void browserDataDisconnected();

    /*!
     * \brief readBrowserData - dummy
     * \warning Sending the data from the browser to the server is not supported!
     */
    void readBrowserData();

    // Slots for communication between proxyServ and proxyClient
    /*!
     * \brief readProxyClientCommand
     * \param command
     */
    void readProxyClientCommand(QByteArray command);

    /*!
     * \brief readProxyClientData
     * \param data
     */
    void readProxyClientData(QByteArray data);

private:
    QTcpServer* server{NULL};       //!< Server on which browser connects for commands
    QTcpSocket* socket{NULL};       //!< Socket on which proxy sends commands to browser
    QTcpServer* dataServer{NULL};   //!< Server on which browser connects for data
    QTcpSocket* dataSocket{NULL};   //!< Socket on which proxy sends data to browser

};

#endif // PROXYSERV_H
