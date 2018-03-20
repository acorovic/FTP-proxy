#ifndef PROXYSERV_H
#define PROXYSERV_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QByteArray>

class ProxyServ : public QObject
{
    Q_OBJECT
public:
    explicit ProxyServ(QObject *parent = nullptr);

    void listenForConnections();

signals:
    void notifyBrowserConnected();
    void toProxyClient(QByteArray);

public slots:
    void browserConnected();
    void browserDisconnected();
    void readBrowserData();
    void readProxyClientData(QByteArray data);

private:
    QTcpServer* server{NULL};
    QTcpSocket* socket{NULL};

    bool isServerStarted{false};
};

#endif // PROXYSERV_H
