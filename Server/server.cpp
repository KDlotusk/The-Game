#include "ConnectionManager.h"

#include "socket.h"
#include "talk.h"

#define NB_CLIENTS 3

using namespace std;
using namespace stdsock;

int main() {
    // Talk com1, com2;
    Talk communications[NB_CLIENTS];
    thread threads[NB_CLIENTS];
    ConnectionManager* connector = new ConnectionManager();
  // ConnectionPoint* connector = new ConnectionPoint();

    cout << "Waiting clients on port " << connector->getPort() << " ..." << endl;

    // accepting connexion
    // and preparing communication points
    StreamSocket* clients[NB_CLIENTS];
    for (int i = 0; i < NB_CLIENTS; ++i) {
        clients[i] = connector->getConnectionPoint()->accept();
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
    delete connector;
    cout << "stop" << endl;
    return 0;
}
