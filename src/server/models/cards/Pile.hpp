#pragma once
#ifndef THEGAME_SERVER_PILE_HPP
#define THEGAME_SERVER_PILE_HPP

#include "Card.hpp"
#include "Stack.hpp"

namespace theGame {
    class Pile : public Stack {
    public:

        Pile(Stack stack);

        Pile(const bool& isAscending);

        Card getTopCard() const;

        void playCard(const Card& card);

        std::string asRequest()const override ;
    };
}  // namespace theGame

#endif  // THEGAME_SERVER_PILE_HPP