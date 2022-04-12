#pragma once
#ifndef THEGAME_SERVER_PILE_HPP
#define THEGAME_SERVER_PILE_HPP

#include "Card.hpp"
#include "Stack.hpp"

namespace theGame {
    class Pile : public Stack {
    public:
        Pile(const bool& isAscending);

        const Card& getTopCard() const;

        void playCard(const Card& card);
    };
}  // namespace theGame

#endif  // THEGAME_SERVER_PILE_HPP