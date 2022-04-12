TEMPLATE = app

TARGET = server
DESTDIR = dist/server
OBJECTS_DIR = build/server

LIBS += -pthread

CONFIG += \
        console c++11 \
        thread
CONFIG -= \
        app_bundle \
        qt

INCLUDEPATH += \
        src/server \
        src/server/connection \
        src/server/models \
        src/server/models/cards \
        src/server/models/clients

HEADERS += \ 
        src/server/server.hpp \
        src/server/connection/communication.hpp \
        src/server/connection/socket.hpp \
        src/server/models/request.hpp \
        src/server/models/cards/Card.hpp \
        src/server/models/cards/Hand.hpp \
        src/server/models/cards/Pile.hpp \
        src/server/models/cards/Stack.hpp \
        src/server/models/clients/Group.hpp \
        src/server/models/clients/VirtualClient.hpp

SOURCES += \
        src/server/server.cpp \
        src/server/connection/communication.cpp \
        src/server/connection/socket.cpp \
        src/server/models/request.cpp \
        src/server/models/cards/Card.cpp \
        src/server/models/cards/Hand.cpp \
        src/server/models/cards/Pile.cpp \
        src/server/models/cards/Stack.cpp \
        src/server/models/clients/Group.cpp \
        src/server/models/clients/VirtualClient.cpp
