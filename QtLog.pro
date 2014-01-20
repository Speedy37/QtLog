#-------------------------------------------------
#
# Project created by QtCreator 2014-01-20T18:17:26
#
#-------------------------------------------------

QT       -= gui

TARGET = QtLog
TEMPLATE = lib

DEFINES += QTLOG_LIBRARY

SOURCES += qtlog.cpp

HEADERS += qtlog.h\
        qtlog_global.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
