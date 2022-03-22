TEMPLATE = subdirs
SUBDIRS = Server ServerAsync
#Server.depends = .
#Client.depends = .

HEADERS += \
    socket.h\
    talk.h
