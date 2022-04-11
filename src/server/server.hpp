#pragma once
#ifndef THEGAME_SERVER_HPP
#define THEGAME_SERVER_HPP

#include "connection/socket.hpp"

namespace theGame {
    extern int port;
    void handleClient(StreamSocket* client);
}  // namespace theGame

int main(int argc, char* argv[]);

#endif  // THEGAME_SERVER_HPP