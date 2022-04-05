#pragma once

#include "Stack.h"

class Hand: public Stack{
public:

    Hand(vector<Card> _cards) : Stack(_cards) {
    }

    Card getCard(int i) { return cards[i]; }
    void addCard(Card card) {cards.push_back(card); }
    void removeCard(int i) {
        if(i> 0 && i < cards.size()) {
            Card tmp = cards[i];
            cards.erase(cards.begin());
        }
    }
    };