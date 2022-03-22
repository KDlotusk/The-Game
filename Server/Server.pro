TEMPLATE = app

CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += thread
LIBS += -pthread

INCLUDEPATH += ../

HEADERS += values.h

SOURCES += \
        server.cpp \
        ../socket.cpp \
        ../talk.cpp
