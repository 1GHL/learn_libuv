TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    forwserver.cpp

HEADERS += \
    forwserver.h


LIBS +=  -lpthread
LIBS += -L$$PWD/../libs/ -luv
INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../libs
PRE_TARGETDEPS += $$PWD/../libs/libuv.a
