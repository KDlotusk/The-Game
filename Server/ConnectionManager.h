#pragma once

#include <iostream>
#include <sys/time.h>
#include <thread>

#include "socket.h"


using namespace std;
using namespace stdsock;
class ConnectionManager {
    int port;

    ConnectionPoint* connectionPoint;


public:
    ConnectionManager(int _port = 3490) {
        port = _port;
        connectionPoint = new ConnectionPoint(port);

        int err = connectionPoint->init();
        if (err != 0) {
            cout << strerror(err) << endl;
            exit(err);
        }
    }

    ~ConnectionManager() {
        delete connectionPoint;
    }

    int getPort() { return port; }
    ConnectionPoint* getConnectionPoint() { return connectionPoint; }
};