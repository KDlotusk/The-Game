#include <stdlib.h>
#include <unistd.h>

#include <mutex>

#include <iostream>
#include <string>
#include <vector>

#include "request.hpp"
#include "communication.hpp"


#define SOCKET_BUFFER_SIZE 1024

using namespace std;

namespace theGame {
    vector<int> clientsFileDescriptors;
    RequestManager requestManager;
    mutex mtx;

    void interactWithClient(const int& __readerFileDescriptor) {
        char message[SOCKET_BUFFER_SIZE] = { 0 };

        clientsFileDescriptors.push_back(__readerFileDescriptor);

        do {
            mtx.lock();

            if (read(__readerFileDescriptor, message, SOCKET_BUFFER_SIZE) <= 0) { mtx.unlock(); break; }

            cout << " < [" + to_string(__readerFileDescriptor) + "] " + message << endl;

            ReturnRequest* response = requestManager.request(string(message), __readerFileDescriptor);
            mtx.unlock();

            while(response->hasNext()) {
                pair<string, int> requestToSend = response->readNext();
                if (write(
                    requestToSend.second, requestToSend.first.c_str(),
                    requestToSend.first.length()) <= 0)
                        break;
                
                cout << " > [" + to_string(requestToSend.second) + "] " + requestToSend.first << endl;
            }
            
        } while (true);

        for (size_t i = 0; i < clientsFileDescriptors.size(); ++i)
            if (clientsFileDescriptors[i] == __readerFileDescriptor)
                clientsFileDescriptors.erase(clientsFileDescriptors.begin() + i);
    }
}  // namespace theGame