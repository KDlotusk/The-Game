TEMPLATE = subdirs
SUBDIRS = Server Client ServerAsync ClientAsync
#Server.depends = .
#Client.depends = .

HEADERS += \
    socket.h\
    talk.h
