#pragma once

#include "Stack.h"

class Pile: public Stack{
public:

    Pile(bool isAscending) {
        if(isAscending == true) {
            cards.push_back(Card(1));
        } 
        else {
            cards.push_back(Card(100));
        }
    }

    Card getTopCard() {
        return cards[0];
    }

    void playCard(Card card) {
        cards.insert(cards.begin(), card);
    }

};