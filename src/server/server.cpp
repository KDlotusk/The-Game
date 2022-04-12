#include "server.hpp"

#include <sys/time.h>

#include <iostream>
#include <thread>
#include <vector>

#include "ConnectionManager.hpp"
#include "communication.hpp"

using namespace std;

namespace theGame {
    int port;
    void handleClient(StreamSocket* __client) {
        interactWithClient(__client->getSockfd());
        cout << "Client " << __client->getSockfd() << " left" << endl;
        delete __client;
    }
}  // namespace theGame

using namespace theGame;

int main(int argc, char* argv[]) {
    if (argc != 2 || sscanf(argv[1], "%d", &port) != 1) {
        port = 3490;  // default port, if none provided
    }

    ConnectionPoint server(port);

    auto err = server.init();
    if (err != 0) {
        cout << strerror(err) << endl;
        exit(err);
    }

    cout << "Waiting clients on port " << server.getPort() << " ..." << endl;

    vector<StreamSocket*> clients;

    do {
        auto client = server.accept();
        cout << "Client " << client->getSockfd() << " accepted" << endl;

        thread(handleClient, client).detach();
    } while (true);

    return EXIT_SUCCESS;
}