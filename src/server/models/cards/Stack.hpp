#pragma once
#ifndef THEGAME_SERVER_STACK_HPP
#define THEGAME_SERVER_STACK_HPP

#include <vector>
#include <string>

#include "Card.hpp"

namespace theGame {
    class Stack {
    protected:
        std::vector<Card> _cards;

    public:
        Stack();

        Stack(const std::vector<Card>& cards);

        virtual ~Stack() {};

        void reset();

        std::vector<Card> draw(const int& i = 1);

        bool isEmpty() const;

        int size() const;

        virtual std::string asRequest() const;
    };
}  // namespace theGame

#endif  // THEGAME_SERVER_STACK_HPP