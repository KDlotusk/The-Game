#include <iostream>
#include <sys/time.h>
#include <thread>
#include <vector>

#include "ConnectionManager.h"

#include "communication.hpp"
#include "socket.h"
#include "values.h"

using namespace std;
using namespace theGame;

namespace theGame {
    void handleClient(stdsock::StreamSocket* client) {
        interactWithClient(client->getSockfd());
        cout << "Client " << client->getSockfd() << " left" << endl;
        delete client;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2 || sscanf(argv[1], "%d", &port) != 1) {
        printf("usage: %s port\n", argv[0]);
        // default port, if none provided
        port = 3490;
    }

    stdsock::ConnectionPoint server(port);

    auto err = server.init();
    if (err != 0) {
        cout << strerror(err) << endl;
        exit(err);
    }

    serverFd = server.getSockfd();
    cout << "Waiting clients on port " << server.getPort() << " ..." << endl;

    vector<stdsock::StreamSocket*> clients;

    do {
        auto client = server.accept();
        cout << "Client " << client->getSockfd() << " accepted" << endl;

        thread(handleClient, client).detach();
    } while (true);

    return EXIT_SUCCESS;
}