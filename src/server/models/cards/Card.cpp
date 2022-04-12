#include "Card.hpp"

namespace theGame {
    Card::Card(const int& __value) : _value(__value) {}

    const int& Card::getValue() const { return _value; }

    const bool& Card::is10diff(const int& __value) const {
        return (_value + 10 == __value || _value - 10 == __value);
    }
}  // namespace theGame