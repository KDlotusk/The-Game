#pragma once
#ifndef THEGAME_SERVER_VIRTUALCLIENT_HPP
#define THEGAME_SERVER_VIRTUALCLIENT_HPP

#include <vector>

#include "Hand.hpp"

namespace theGame {
    class VirtualClient {
    private:
        long _id;  // 1000 000 par exemple
        long _lastRequestId;
        Hand* _hand = new Hand(std::vector<Card>());
        int _cardsPlayed = 0;

    public:
        VirtualClient(const long& id);
        ~VirtualClient();

        const long& getId() const;
        const Hand* getHand() const;
        const int& getCardsPlayed() const;

        void addCards(const std::vector<Card>& __cards);

        void incrementRequest();

        const bool& isRequestFromThisPlayer(const long& __requestId) const;

        void incrementNbCardsPlayed();

        void cardsPlayedTo0();
    };
}  // namespace theGame

#endif  // THEGAME_SERVER_VIRTUALCLIENT_HPP