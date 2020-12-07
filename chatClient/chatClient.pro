#-------------------------------------------------
#
# Project created by QtCreator 2016-09-03T20:13:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chatClient
TEMPLATE = app

LIBS += -lpthread
SOURCES += main.cpp\
    file.cpp \
        interface.cpp \
    login.cpp \
    connectserver.cpp \
    onlinelistupdate.cpp \
    recvfilethread.cpp \
    register.cpp \
    recvmsgthread.cpp

HEADERS  += interface.h \
    file.h \
    login.h \
    connectserver.h \
    onlinelistupdate.h \
    recvfilethread.h \
    register.h \
    msg.h \
    recvmsgthread.h

FORMS    += interface.ui \
    file.ui \
    login.ui \
    register.ui
