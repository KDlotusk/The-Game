#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <utility>
#include <vector>

#include "RequestManager.cpp"
#include "communication.hpp"

#define SOCKET_BUFFER_SIZE 1024

namespace theGame {
    int serverFd;
    std::vector<int> clientsFds;

    RequestManager requestManager;

    void interactWithClient(int reader) {
        char msg[SOCKET_BUFFER_SIZE] = { 0 };
        int err;

        clientsFds.push_back(reader);

        do {

            err = read(reader, msg, SOCKET_BUFFER_SIZE);
            if (err <= 0) break;

            std::cout << " < [" + std::to_string(reader) + "] " + msg << std::endl;

            ReturnRequest* response = requestManager.request(std::string(msg), reader);


            while(response->hasNext()) {
            pair<std::string, int> message = response->readNext();
                err = write(message.second, message.first.c_str(), message.first.length());
                if (err <= 0) {
                    std::cout << "error" << std::endl;
                    break;
                }

                std::cout << " > [" + std::to_string(message.second) + "] " + message.first << std::endl;
            }  

            if (err <= 0) break;          

        } while (true);

        for (std::size_t i = 0; i < clientsFds.size(); ++i)
            if (clientsFds[i] == reader)
                clientsFds.erase(clientsFds.begin() + i);
    }
}