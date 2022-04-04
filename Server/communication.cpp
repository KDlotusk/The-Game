#include <iostream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <vector>

#include "RequestManager.cpp"
#include "communication.hpp"

#define SOCKET_BUFFER_SIZE 1024

namespace theGame {
    int serverFd;
    std::vector<int> clientsFds;

    void interactWithClient(int reader) {
        char msg[SOCKET_BUFFER_SIZE] = { 0 };
        int err;

        clientsFds.push_back(reader);

        do {
            err = read(reader, msg, SOCKET_BUFFER_SIZE);
            if (err <= 0) break;

            std::string res = requestManager(std::string(msg));
            err = write(reader, res.c_str(), res.length());
            if (err <= 0) break;

        } while (true);

        for (std::size_t i = 0; i < clientsFds.size(); ++i)
            if (clientsFds[i] == reader)
                clientsFds.erase(clientsFds.begin() + i);
    }
}