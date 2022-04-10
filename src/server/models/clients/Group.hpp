#pragma once
#ifndef THEGAME_SERVER_GROUP_HPP
#define THEGAME_SERVER_GROUP_HPP

#include <array>
#include <utility>
#include <vector>

#include "Pile.hpp"
#include "VirtualClient.hpp"

namespace theGame {
    class Group {
    private:
        std::pair<int, int> _nbMinMaxPlayer = std::make_pair(2, 5);
        int _nbCardsPerHand = 6;

        long _idGroup;
        std::vector<VirtualClient> _clients;
        Stack _stack;

        std::array<Pile, 4> _piles = { Pile(true), Pile(true), Pile(false),
                                       Pile(false) };

        int _status = 0;  // 1: in game / 0: loading / 2: end of game

        int _currentPlayer = 0;

    public:
        Group(long idGroup);

        const bool& isRequestFromThisGroup(const long& requestId) const;

        const VirtualClient& getGameMaster() const;
        const long& getId() const;

        void addClient(const VirtualClient& client);
        void removeClient(const long& id);

        const int& getNbOfClient() const;

        const bool& startGame();

        void play(const int& pile, const int& cardNumber, const long& requestId);

        void endOfTurn();
    };
}  // namespace theGame

#endif  // THEGAME_SERVER_GROUP_HPP