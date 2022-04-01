#include <iostream>
#include <sys/time.h>
#include <thread>
#include <vector>

#include "socket.h"
#include "talk.h"
#include "values.h"

using namespace std;
using namespace stdsock;

void handleClient(
    vector<StreamSocket*>& clients,
    vector<Talk>& communications,
    vector<thread>& threads,
    int i
) {
    talk(communications[i]);
    threads.erase(threads.begin() + i);
    delete clients[i];
}

int main(int argc, char* argv[]) {
    if (argc != 2 || sscanf(argv[1], "%d", &port) != 1) {
        printf("usage: %s port\n", argv[0]);
        // default port, if none provided
        port = 3490;
    }

    ConnectionPoint server(port);

    auto err = server.init();
    if (err != 0) {
        cout << strerror(err) << endl;
        exit(err);
    }

    cout << "Waiting clients on port " << port << " ..." << endl;

    vector<StreamSocket*> clients;
    vector<Talk> communications;
    vector<thread> threads;

    do {
        clients.push_back(server.accept());
        auto i = clients.size() - 1;
        cout << "Client " << i << " accepted" << endl;

        Talk com;
        com.setReader(clients[i]->getSockfd());
        com.setWriter(-1);
        communications.push_back(com);

        threads.push_back(
            thread(handleClient, ref(clients), ref(communications), ref(threads), i)
        );
    } while (true);

    return EXIT_SUCCESS;
}
