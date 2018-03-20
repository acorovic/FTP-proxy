TEMPLATE = app
QT += network core
CONFIG += console c++11
CONFIG += core

SOURCES += main.cpp \
    test.cpp \
    proxyserv.cpp \
    proxyclient.cpp \
    proxy.cpp

HEADERS += \
    test.h \
    proxyserv.h \
    proxyclient.h \
    proxy.h
