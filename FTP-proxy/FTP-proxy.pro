TEMPLATE = app
QT += network core
CONFIG += console c++11
CONFIG += core

SOURCES += main.cpp \
    proxyserv.cpp \
    proxyclient.cpp \
    proxy.cpp

HEADERS += \
    proxyserv.h \
    proxyclient.h \
    proxy.h
