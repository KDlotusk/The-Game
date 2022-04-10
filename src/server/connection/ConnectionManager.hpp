#pragma once

#ifndef THEGAME_SERVER_CONNECTIONMANAGER_HPP
#define THEGAME_SERVER_CONNECTIONMANAGER_HPP

#include <sys/time.h>

#include <iostream>
#include <thread>

#include "socket.hpp"

using namespace std;
using namespace theGame;

class ConnectionManager {
private:
    int port;

    ConnectionPoint* pConnectionPoint;

public:
    ConnectionManager(int __port = 3490) {
        port = __port;
        pConnectionPoint = new ConnectionPoint(port);

        int err = pConnectionPoint->init();
        if (err != 0) {
            cout << strerror(err) << endl;
            exit(err);
        }
    }

    ~ConnectionManager() { delete pConnectionPoint; }

    int getPort() { return port; }
    ConnectionPoint* getConnectionPoint() { return pConnectionPoint; }
};

#endif  // THEGAME_SERVER_CONNECTIONMANAGER_HPP