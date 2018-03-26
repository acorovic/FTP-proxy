#ifndef PROXYCLIENT_H
#define PROXYCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTcpServer>

#define FTP_SERV_ADDRESS "127.0.0.1"    //!< IP of the FTP server
#define FTP_SERV_PORT 27015             //!< Port of the FTP server
#define DATA_SERV_PORT 27016            //!< Port on which proxyServ will create dataServer for the browser

class ProxyClient : public QObject
{
    Q_OBJECT
public:
    /*!
     * \brief ProxyClient ctor
     * \param parent
     */
    explicit ProxyClient(QObject *parent = nullptr);

signals:
    /*!
     * \brief toProxyServerCommand - signal to forward command to proxyServer
     */
    void toProxyServerCommand(QByteArray);

    /*!
     * \brief toProxyServerData - signal to forward data to proxyServer
     */
    void toProxyServerData(QByteArray);

    /*!
     * \brief createDataServer - singal for proxyServer to create data Server on specific port
     * \param port
     */
    void createDataServer(int port);

public slots:
    // Slots to connect to FTP server to read commands
    /*!
     * \brief connectToFtpServer - after browser connects to proxyServ
     */
    void connectToFtpServer();

    /*!
     * \brief connectedToFtpServer
     */
    void connectedToFtpServer();

    /*!
     * \brief disconnectedFtpServer
     */
    void disconnectedFtpServer();

    /*!
     * \brief readServerCommand - read the command from the FTP server
     * \details functions reads and parses message from the FTP server, if message contains
     *          "Entering Passive Mode" then it changes the message and forwards it to proxyServ
     *          "Transfer complete" then it waits for the data to be received
     *          else it just forwards the message to the proxyServ
     */
    void readServerCommand();

    // Slots to connect to FTP server to read data
    /*!
     * \brief connectedToDataServer
     */
    void connectedToDataServer();

    /*!
     * \brief disconnectedFromDataServer
     */
    void disconnectedFromDataServer();

    /*!
     * \brief readServerData - forward the data to proxyServ
     */
    void readServerData();

    // Slots for communication with the proxyServ

    /*!
     * \brief readProxyServerCommand
     * \param command
     */
    void readProxyServerCommand(QByteArray command);


private:
    QTcpSocket* socket{NULL};       //!< Socket to connect to the FTP server for commands
    QTcpSocket* dataSocket{NULL};   //!< Socket to connect to the FTP server for data

    bool connected{false};          //!< Helper variable

    /*!
     * \brief getPassivePort - function parses message and returns the port
     * \param message - to be parsed
     * \return  port
     */
    int getPassivePort(QByteArray* message);

    /*!
     * \brief setDataProxyPort - function which creates new message
     * \param message - to be parsed and created new mesasge from this
     * \return new message with the changed port to DATA_SERV_PORT
     */
    QByteArray setDataProxyPort(QByteArray* message);
};

#endif // PROXYCLIENT_H
