#ifndef PROXYCLIENT_H
#define PROXYCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

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
    void toProxyServer(QByteArray);

public slots:
    void connectToFtpServer();
    void connectedToFtpServer();
    void disconnectedFtpServer();
    void readServerData();
    void readProxyServerData(QByteArray data);

private:
    QTcpSocket* socket;
};

#endif // PROXYCLIENT_H
