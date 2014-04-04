TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS +=  -lpthread
LIBS += -L$$PWD/../libs/ -luv
INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../libs
PRE_TARGETDEPS += $$PWD/../libs/libuv.a

