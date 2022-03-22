#include <clocale>
#include <ctime>
#include <iostream>
#include <thread>

#include "socket.h"

using namespace std;
using namespace stdsock;

void worker(StreamSocket* client) {
    std::cout << "Got a client!" << std::endl;

    std::cout << client->getIP() << ":" << client->getPort() << std::endl;

    /// @link https://cplusplus.com/reference/ctime/strftime/ @endlink
    time_t ttime = time(0);
    setlocale(LC_ALL, "fr_FR.UTF-8");
    char dateString[80]; // Chaine de caractere contenant la date
    strftime(dateString, 80, "%c", localtime(&ttime)); // https://cplusplus.com/reference/ctime/strftime/

    client->send(dateString, strlen(dateString), 0); // send a char*
    cout << "Server: \t" << dateString << endl;

    sleep(5);
    string veryLongString = "dskhgjkdshgsdlgsdhjghsdglshkgsdlhgsdhjkgsdlgdhhdshjgsdhjdsssssshghjdgjdhgjdghdjghdjghjdghdjghdjghdjghjdhgjdghdjghdjghdjhgjdhgjdghjhdjghdjhgjdghdjghdjghdjghdjgdhjgdhgjdhgjdhgdjghjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj";
    client->send(veryLongString); // send a std::string
    cout << "Server: \t" << veryLongString << endl;

    //    while (true) {
    //        std::string msg;
    //        if (client->read(msg) <= 0) {
    //            break;
    //        }

    //        std::cout << msg;
    //    }

    delete client;
    std::cout << "Client disconnected" << std::endl;
}


int main() {
    ConnectionPoint* server = new ConnectionPoint(3490);

    int err = server->init();
    std::cout << server->getIP() << ":" << server->getPort() << std::endl;
    if (err != 0) {
        std::cout << strerror(err) << std::endl;
        exit(err);
    }

    while (true) {
        StreamSocket* client = server->accept();

        if (!client->valid()) {
            delete client;
            continue;
        }

        std::thread(worker, client).detach();
    }



    delete server;
    return 0;
}
