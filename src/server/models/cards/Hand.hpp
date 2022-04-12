#pragma once
#ifndef THEGAME_SERVER_HAND_HPP
#define THEGAME_SERVER_HAND_HPP

#include "Card.hpp"
#include "Stack.hpp"

namespace theGame {
    class Hand : public Stack {
    public:
        Hand(const std::vector<Card>& cards);
        ~Hand() override {};

        void resetHand();
        Card getCard(const int& i) const;
        void addCard(const Card& card);
        Card removeCard(const int& i);
    };
}  // namespace theGame

#endif  // THEGAME_SERVER_HAND_HPP