#-------------------------------------------------
#
# Project created by QtCreator 2015-12-06T15:35:57
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Market-Server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    buyer.cpp \
    electronics.cpp \
    food.cpp \
    goods.cpp \
    livinggoods.cpp \
    seller.cpp \
    user.cpp

HEADERS  += mainwindow.h \
    buyer.h \
    electronics.h \
    food.h \
    goods.h \
    livinggoods.h \
    seller.h \
    user.h

FORMS    += mainwindow.ui
