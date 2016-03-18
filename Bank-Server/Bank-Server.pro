#-------------------------------------------------
#
# Project created by QtCreator 2015-12-06T17:06:35
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Bank-Server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    user.cpp

HEADERS  += mainwindow.h \
    user.h

FORMS    += mainwindow.ui
