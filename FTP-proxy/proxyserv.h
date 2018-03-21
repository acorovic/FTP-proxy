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
    void toProxyClientCommand(QByteArray);

public slots:
    void browserConnected();
    void browserDisconnected();
    void readBrowserCommand();

    void createDataServ(int portNo);
    void browserDataConnected();
    void browserDataDisconnected();
    void readBrowserData();

    void readProxyClientCommand(QByteArray data);
    void readProxyClientData(QByteArray data);

private:
    QTcpServer* server{NULL};
    QTcpSocket* socket{NULL};
    QTcpServer* dataServer{NULL};
    QTcpSocket* dataSocket{NULL};

    bool isServerStarted{false};
};

#endif // PROXYSERV_H
