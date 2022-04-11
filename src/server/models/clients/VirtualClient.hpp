#pragma once
#ifndef THEGAME_SERVER_VIRTUALCLIENT_HPP
#define THEGAME_SERVER_VIRTUALCLIENT_HPP

#include <vector>

#include "Hand.hpp"

namespace theGame {
    class VirtualClient {
    private:
        long _id;  // 100 000 for example
        long _lastRequestId;
        Hand* _hand;
        int _cardsPlayed = 0;

        int _fileDescriptor;

    public:
        VirtualClient(const long& id, const int& fileDescriptor);
        ~VirtualClient();

        const long& getId() const;
        const Hand* getHand() const;
        const int& getCardsPlayed() const;
        const int& getFileDescriptor() const;
        const int& getLastRequestId() const;

        void setLastRequestId(const int& lastRequestId);
        void incrementRequest();

        const bool& isRequestFromThisPlayer(const long& __requestId) const;

        void addCards(const std::vector<Card>& __cards);

        void incrementNbCardsPlayed();
        void cardsPlayedTo0();

        const string& asRequest() const;
    };
}  // namespace theGame

#endif  // THEGAME_SERVER_VIRTUALCLIENT_HPP