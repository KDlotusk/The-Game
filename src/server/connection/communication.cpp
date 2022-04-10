#include "communication.hpp"

#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <vector>

#include "request.hpp"

#define SOCKET_BUFFER_SIZE 1024

using namespace std;

namespace theGame {
    vector<int> clientsFileDescriptors;

    void interactWithClient(const int& __readerFileDescriptor) {
        char message[SOCKET_BUFFER_SIZE] = { 0 };

        clientsFileDescriptors.push_back(__readerFileDescriptor);

        RequestManager requestManager;

        do {
            if (read(__readerFileDescriptor, message, SOCKET_BUFFER_SIZE) <= 0) break;

            string serverAnswer = requestManager.request(string(message));
            if (write(
                    __readerFileDescriptor, serverAnswer.c_str(),
                    serverAnswer.length()) <= 0)
                break;
        } while (true);

        for (size_t i = 0; i < clientsFileDescriptors.size(); ++i)
            if (clientsFileDescriptors[i] == __readerFileDescriptor)
                clientsFileDescriptors.erase(clientsFileDescriptors.begin() + i);
    }
}  // namespace theGame