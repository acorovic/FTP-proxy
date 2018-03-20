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

signals:
    void notifyBrowserConnected();

public slots:
    void browserConnected();
    void browserDisconnected();
    void browserWritten(qint64);

private:
    QTcpServer* server{NULL};
    QTcpSocket* socket{NULL};

    bool isServerStarted{false};
};

#endif // PROXYSERV_H
