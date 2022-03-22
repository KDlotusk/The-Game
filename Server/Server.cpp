#include <iostream>
#include <sys/time.h>
#include <thread>

#include "values.h"

#include "socket.h"
#include "talk.h"

#define NB_CLIENTS 3

using namespace std;
using namespace stdsock;

int main(int argc, char* argv[]) {
    Talk communications[NB_CLIENTS];
    thread threads[NB_CLIENTS];

    if (argc != 2 || sscanf(argv[1], "%d", &port) != 1) {
        printf("usage: %s port\n", argv[0]);
        // default port, if none provided
        port = 3490;
    }

    ConnectionPoint* server = new ConnectionPoint(port);
    int err = server->init();
    if (err != 0) {
        cout << strerror(err) << endl;
        exit(err);
    }

    cout << "Waiting clients on port " << port << " ..." << endl;

    // accepting connexion
    // and preparing communication points
    StreamSocket* clients[NB_CLIENTS];
    for (int i = 0; i < NB_CLIENTS; ++i) {
        clients[i] = server->accept();
        cout << "Client " << i << " accepted" << endl;

        communications[i].setReader(clients[i]->getSockfd());
        communications[i].setWriter(-1);

        threads[i] = thread(talk, ref(communications[i]));
    }

    for (int i = 0; i < NB_CLIENTS; ++i) {
        threads[i].join();
        cout << "Client " << i << " left." << endl;
        delete clients[i];
    }

    // closing connexion point
    delete server;
    cout << "stop" << endl;
    return 0;
}
