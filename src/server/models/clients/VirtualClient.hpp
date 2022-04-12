#pragma once
#ifndef THEGAME_SERVER_VIRTUALCLIENT_HPP
#define THEGAME_SERVER_VIRTUALCLIENT_HPP

#include <vector>
#include <string>

#include "Hand.hpp"

namespace theGame {
    class VirtualClient {
    private:
        long _id;  // 100 000 for example
        int _fileDescriptor;

        long _lastRequestId;
        time_t _timeLastRecord;
        Hand* _hand;
        int _cardsPlayed = 0;

        bool _isConnected;

    public:
        VirtualClient(const long& id, const int& fileDescriptor);
        ~VirtualClient();

        long getId() const;
        Hand* getHand() const;
        int getCardsPlayed() const;
        int getFileDescriptor() const;
        int getLastRequestId() const;
        bool isConnected() const;
        time_t getTimeElapsedSinceLastRequest();

        void resetFileDescriptor();
        void setFileDescriptor(const int& fileDescriptor);

        void setTimerAtMinus9();

        void setLastRequestId(const int& lastRequestId);
        void incrementRequest();

        bool isRequestFromThisPlayer(const long& __requestId) const;

        void disconnect();
        void reconnect(const int& __fileDescriptor);

        void addCards(const std::vector<Card>& __cards);

        void incrementNbCardsPlayed();
        void cardsPlayedTo0();

        std::string asRequest() const;
    };
}  // namespace theGame

#endif  // THEGAME_SERVER_VIRTUALCLIENT_HPP