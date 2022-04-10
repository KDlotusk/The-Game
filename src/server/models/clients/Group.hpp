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

        int _asyncCode = -1;

    public:
        Group(long idGroup);

        const bool& isRequestFromThisGroup(const long& requestId) const;

        const VirtualClient& getGameMaster() const;
        const long& getId() const;
        const int& getStatus() const;

        void addClient(const VirtualClient& client);
        void removeClient(const long& id);

        vector<VirtualClient> getClients();
        const int& getNbOfClient() const;
        const int& getCurrentClient() const;
        const bool& isStackEmpty() const;
        const int& getAsyncCode() const;
        const int& getFileDescriptorCurrentPlayer() const;

        void setAsyncCoe(const int& asyncCode);

        const std::string& sendPiles() const;
        const std::string& sendHandCurrentPlayer() const;
        const std::vector<int>& getAllFileDescriptor() const;
        const std::vector<int>& getAllFileDescriptorButCurrentPlayer() const;
        const int& nbCardsNotPLayed() const;


        const bool& startGame();

        void play(const int& pile, const int& cardNumber, const long& requestId);

        void endOfTurn();
        int endOfGame();

        
    };
}  // namespace theGame

#endif  // THEGAME_SERVER_GROUP_HPP