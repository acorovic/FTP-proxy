#include <iostream>
#include <QtCore>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QObject>

#include "proxy.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    Proxy proxy;

    return a.exec();
}
