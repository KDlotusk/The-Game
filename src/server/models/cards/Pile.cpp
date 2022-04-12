#include <string>

#include "Pile.hpp"

using namespace std;

namespace theGame {

    Pile::Pile(Stack __stack) : Stack(__stack) {};

    Pile::Pile(const bool& __isAscending) {
        if (__isAscending == true) {
            _cards.push_back(Card(1));
        } else {
            _cards.push_back(Card(100));
        }
    }

    Card Pile::getTopCard() const { return _cards[0]; }

    void Pile::playCard(const Card& __card) { _cards.insert(_cards.begin(), __card); }

    string Pile::asRequest() const {
        return to_string(getTopCard().getValue());
    }
}  // namespace theGame