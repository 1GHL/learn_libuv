TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
#    test/test-tcp-read-stop.c \
#    test/test-tcp-open.c \
#    test/test-tcp-flags.c \
#    test/test-tcp-connect-timeout.c \
#    test/test-tcp-close-while-connecting.c \
#    test/test-tcp-close.c \
#    test/test-poll-close.c \
#    test/test-poll.c \
#    test/test-ping-pong.c
    test.cpp \
    testmam.cpp \
    test_udp.cpp

LIBS +=  -lpthread -levent -lev
LIBS += -L$$PWD/../libs/ -luv
INCLUDEPATH += $$PWD/../include
DEPENDPATH += $$PWD/../libs
PRE_TARGETDEPS += $$PWD/../libs/libuv.a

