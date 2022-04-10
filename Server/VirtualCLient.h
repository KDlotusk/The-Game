#pragma once

#include "Hand.h"

class VirtualClient {
private:
    long id; // 1000 000 par exemple
    long lastRequestId;
    Hand* hand = new Hand(vector<Card>());
    int cardsPlayed = 0;

    int fileDescriptor;


public:
    VirtualClient(long _id, int _fileDescriptor) {
        id = _id;
        lastRequestId = id +1;
        
        fileDescriptor = _fileDescriptor;
    }
    ~VirtualClient() {
        delete hand;
    }


    long getId() {
        return id;
    }
    Hand* getHand() {
        return hand;
    }
    int getCardsPlayed() {
        return cardsPlayed;
    }
    int getFileDescriptor() {
        return fileDescriptor;
    }
    int getLastRequestId() {
        return lastRequestId;
    }

    void setLastRequestId(int _lastRequestId) {
        lastRequestId = _lastRequestId;
    }

    void addCards(vector<Card> cards) {
        for(int k = 0; k < cards.size(); k++) {
            hand->addCard(cards[k]);
        }
    }

    void incrementRequest() {
        lastRequestId = (lastRequestId%1000 +1)%1000 + id;
    }

    bool isRequestFromThisPlayer(long requestId) {
        if(id%1000 == requestId%1000)
            return true;
        return false;
    }

    void incrementNbCardsPlayed() {
        cardsPlayed += 1;
    }

    void cardsPlayedTo0() {
        cardsPlayed = 0;
    }

    string asRequest() {
        return hand->asRequest();
    }


};