TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    tinyweb.cpp \
    echo-server.cpp

HEADERS += \
    tinyweb.h \
#    task.h \
    echo-server.h

LIBS +=  -lpthread
LIBS += -L$$PWD/../libs/ -luv
INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../libs
PRE_TARGETDEPS += $$PWD/../libs/libuv.a
