#include <iostream>
#include <sys/time.h>
#include <thread>

#include "socket.h"

using namespace std;
using namespace stdsock;

int main(int argc, char* argv[]) {
    int port;

    if (argc != 3)
    {
        printf("usage: %s server_address port\n", argv[0]);
        return 0;
    }

    if (sscanf(argv[2], "%d", &port) != 1)
    {
        printf("usage: %s server_address port\n", argv[0]);
        return 1;
    }

    //
    StreamSocket* sock = new StreamSocket(argv[1], port);

    int err = sock->connect();
    if (err != 0) {
        delete sock;
        perror("[-]Error in connection: ");
        return(err);
    }
    cout << "[+]Connected to Server " << sock->getIP() << ":" << sock->getPort() << endl;

    string msg;
    int nb = sock->read(msg);
    if (nb < 0) {
        printf("[-]Error in receiving data.\n");
    }
    else {
        cout << "Server: \t" << msg << endl;
    }
    msg.clear();
    nb = sock->read(msg);
    if (nb < 0) {
        printf("[-]Error in receiving data.\n");
    }
    else {
        cout << "Server: \t" << msg << endl;
    }

    //    delete sock;
    return 0;
}
