#include <iostream>
#include <sys/time.h>
#include <thread>

#include "socket.h"
#include "talk.h"

using namespace std;
using namespace stdsock;

int main(int argc, char* argv[]) {
    Talk input, output;
    std::thread threads[2];
    int port;

    if (argc != 3) {
        printf("usage: %s server_address port\n", argv[0]);
        return 0;
    }

    if (sscanf(argv[2], "%d", &port) != 1) {
        printf("usage: %s server_address port\n", argv[0]);
        return 1;
    }

    //
    cout << "Attemping to reach " << argv[1] << ":" << port << endl;
    StreamSocket* sock = new StreamSocket(std::string(argv[1]), port);

    int err = sock->connect();
    if (err != 0) {
        delete sock;
        perror("[-]Error in connection: ");
        return(err);
    }
    cout << "[+]Connected to Server.\n";

    input.setReader(0);
    input.setWriter(sock->getSockfd());
    threads[0] = std::thread(talk, std::ref(input));

    output.setReader(sock->getSockfd());
    output.setWriter(1);
    threads[1] = std::thread(talk, std::ref(output));

    //
    threads[0].join();
    threads[1].join();

    delete sock;
    cout << "stop\n";

    return 0;
}
