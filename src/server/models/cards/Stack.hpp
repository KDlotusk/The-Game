#pragma once
#ifndef THEGAME_SERVER_STACK_HPP
#define THEGAME_SERVER_STACK_HPP

#include <vector>

#include "Card.hpp"

namespace theGame {
    class Stack {
    protected:
        std::vector<Card> _cards;

    public:
        Stack();

        Stack(const std::vector<Card>& cards);

        void reset();

        const std::vector<Card>& draw(const int& i = 1);

        const bool& isEmpty() const;

        const int& size() const;

        virtual const std::string& asRequest() const;
    };
}  // namespace theGame

#endif  // THEGAME_SERVER_STACK_HPP