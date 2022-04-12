#pragma once
#ifndef THEGAME_SERVER_GROUP_HPP
#define THEGAME_SERVER_GROUP_HPP

#include <array>
#include <utility>
#include <vector>

#include "../cards/Pile.hpp"
#include "VirtualClient.hpp"

namespace theGame {
    class Group {
    private:
        std::pair<int, int> _nbMinMaxPlayer = std::make_pair(2, 5);
        int _nbCardsPerHand = 6;

        long _idGroup;
        std::vector<VirtualClient*> _clients;
        Stack _stack;

        std::array<Pile, 4> _piles = { Pile(true), Pile(true), Pile(false),
                                       Pile(false) };

        int _status = 0;  // 1: in game / 0: loading / 2: end of game

        int _currentClient = 0;

        int _asyncCode = -1;

    public:
        Group(long idGroup);

        bool isRequestFromThisGroup(const long& requestId) const;

        long getId() const;
        int getStatus() const;

        void addClient(VirtualClient* client);
        void removeClient(const long& id);

        std::vector<VirtualClient*> getClients() const;
        int getNbOfClient() const;
        int getCurrentClient() const;
        bool isStackEmpty() const;
        int getAsyncCode() const;
        int getFileDescriptorCurrentPlayer() const;

        void setAsyncCode(const int& asyncCode);

        std::string sendPiles() const;
        std::string sendHandCurrentPlayer() const;
        std::vector<int> getAllFileDescriptor() const;
        std::vector<int> getAllFileDescriptorButCurrentPlayer() const;
        int nbCardsNotPLayed() const;


        bool startGame();

        int play(const long& requestId, const int& pile, const int& cardId);

        void endOfTurn();
        int endOfGame();
    };
}  // namespace theGame

#endif  // THEGAME_SERVER_GROUP_HPP