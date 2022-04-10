#include "Stack.hpp"

#include <algorithm>
#include <iostream>
#include <random>

using namespace std;

namespace theGame {
    Stack::Stack() { reset(); }

    Stack::Stack(const vector<Card>& __cards) { _cards = __cards; }

    void Stack::reset() {
        _cards.clear();
        for (size_t k = 1; k < 100; k++) {
            _cards.push_back(k);
        }

        auto rng = default_random_engine{};
        shuffle(begin(_cards), end(_cards), rng);
    }

    const vector<Card>& Stack::draw(const int& i /* = 1 */) {
        vector<Card> cardDrawn;
        for (size_t k = 0; k < i; k++) {
            auto tmp = _cards[0];
            _cards.erase(_cards.begin());
            cardDrawn.push_back(tmp);
        }
        return cardDrawn;
    }

    const bool& Stack::isEmpty() const { return _cards.empty(); }
}  // namespace theGame