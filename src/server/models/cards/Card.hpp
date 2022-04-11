#pragma once
#ifndef THEGAME_SERVER_CARD_HPP
#define THEGAME_SERVER_CARD_HPP

namespace theGame {
    class Card {
        int _value;

    public:
        Card(const int& value);

        int getValue() const;

        bool is10diff(const int& value) const;
    };
}  // namespace theGame

#endif  // THEGAME_SERVER_CARD_HPP