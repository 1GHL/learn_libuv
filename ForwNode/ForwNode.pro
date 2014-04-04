TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    forwserver.cpp

HEADERS += \
    forwserver.h

LIBS +=  -lpthread -levent

