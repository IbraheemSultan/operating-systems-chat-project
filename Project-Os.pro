QT += core gui widgets network
CONFIG += c++11
TEMPLATE = app

SOURCES += main.cpp \
           clientwindow.cpp \
           server.cpp

HEADERS += server.h \
    clientwindow.h
