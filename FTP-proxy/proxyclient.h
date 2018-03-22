#ifndef PROXYCLIENT_H
#define PROXYCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QTcpServer>

#define FTP_SERV_ADDRESS "127.0.0.1"
#define FTP_SERV_PORT 27015

enum ClientState_t {IDLE, READY, USER_SENT, PASS_REQ, PASS_AUTH,
                    QUIT, BYE, AUTHORIZED, FORWARD};

class ProxyClient : public QObject
{
    Q_OBJECT
public:
    explicit ProxyClient(QObject *parent = nullptr);

signals:
    void toProxyServerCommand(QByteArray);
    void toProxyServerData(QByteArray);

    void createDataServer(int port);

public slots:
    void connectToFtpServer();

    void connectedToFtpServer();
    void disconnectedFtpServer();
    void readServerCommand();

    void connectedToDataServer();
    void disconnectedFromDataServer();
    void readServerData();

    void readProxyServerCommand(QByteArray data);


private:
    QTcpSocket* socket{NULL};
    QTcpSocket* dataSocket{NULL};

    bool connected{false};
    bool dataRead{false};

    int getPassivePort(QByteArray* message);
};

#endif // PROXYCLIENT_H
