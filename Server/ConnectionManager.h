#pragma once

#include <iostream>
#include <sys/time.h>
#include <thread>

#include "socket.h"
#include "talk.h"


using namespace std;

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