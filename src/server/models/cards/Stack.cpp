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
            Card tmp = _cards[0];
            _cards.erase(_cards.begin());
            cardDrawn.push_back(tmp);
        }
        return cardDrawn;
    }

    const bool& Stack::isEmpty() const { return _cards.empty(); }

    const int& Stack::size() const { return _cards.size(); }

    const string& Stack::asRequest() const {
        string str = to_string(_cards.size());

        for(size_t k = 0; k < _cards.size(); k++) {
            str+= " " + _cards[k].getValue();
        }

        return str;
    }


}  // namespace theGame