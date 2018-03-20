#ifndef TEST_H
#define TEST_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

class Test : public QObject
{
    Q_OBJECT
public:
    explicit Test(QObject *parent = nullptr);

signals:

public slots:
    void newConnection();

private:
    QTcpServer* server;
};

#endif // TEST_H
