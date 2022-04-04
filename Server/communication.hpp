#pragma once
#ifndef TALK_H
#define TALK_H

namespace theGame {
    extern int serverFd;
    extern std::vector<int> clientsFds;

    void interactWithClient(int reader);
}

#endif // TALK_H
