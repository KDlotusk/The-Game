#pragma once

#ifndef THEGAME_SERVER_COMMUNICATION_HPP
#define THEGAME_SERVER_COMMUNICATION_HPP

#include <vector>

#include "ReturnRequest.hpp"

namespace theGame {
    extern std::vector<int> clientsFileDescriptors;

    void updateTimer();
    void sendRequest(ReturnRequest* __request);
    void interactWithClient(const int& readerFileDescriptor);
}  // namespace theGame
#endif  // THEGAME_SERVER_COMMUNICATION_HPP
