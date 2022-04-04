#pragma once

#include <iostream>
#include <vector>
#include "Card.h"

#include <algorithm>
#include <random>

using namespace std;


class Stack{
protected:
    vector<Card> cards;

public:

    Stack() {
        reset();
    }

    Stack(vector<Card> _cards) {
        cards = _cards;
    }

    void reset() {
        cards.clear();
        for(int k = 1; k < 100; k++) { cards.push_back(k); }

        auto rng = std::default_random_engine {};
        shuffle(begin(cards), end(cards), rng);
    }

    vector<Card> draw(int i = 1) {

        vector<Card> cardDrawn;
        for(int k = 0; k < i; k++) {
            Card tmp = cards[0];
            cards.erase(cards.begin());
            cardDrawn.push_back(tmp);
        }
        return cardDrawn;
    }

    bool isEmpty() {
        return cards.empty();
    }

};