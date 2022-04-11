#include "Hand.hpp"

using namespace std;

namespace theGame {
    Hand::Hand(const vector<Card>& _cards) : Stack(_cards) {}

    Card Hand::getCard(const int& __i) const { return _cards[__i]; }
    void Hand::addCard(const Card& __card) { _cards.push_back(__card); }
    Card Hand::removeCard(const int& __i) {
        if (__i > 0 && __i < (int)_cards.size()) {
            Card tmp = _cards[__i];
            _cards.erase(_cards.begin() + __i);

            return tmp;
        }

        return Card(-1);
    }
}  // namespace theGame