#pragma once

class Card {

    int cardValue;

public:

    Card(int value) {
        cardValue = value;
    }

    int getValue() {
        return cardValue;
    }

    bool is10diff(int value) {
        if(cardValue + 10 == value || cardValue -10 == value)
            return true;
        return false;
    }
};