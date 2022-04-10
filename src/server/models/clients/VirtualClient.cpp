#include "VirtualClient.hpp"

using namespace std;

namespace theGame {
    VirtualClient::VirtualClient(const long& __id)
        : _id(__id), _lastRequestId(__id + 1) {}
    VirtualClient::~VirtualClient() { delete _hand; }

    const long& VirtualClient::getId() const { return _id; }
    const Hand* VirtualClient::getHand() const { return _hand; }
    const int& VirtualClient::getCardsPlayed() const { return _cardsPlayed; }

    void VirtualClient::addCards(const vector<Card>& __cards) {
        for (size_t k = 0; k < __cards.size(); k++) {
            _hand->addCard(__cards[k]);
        }
    }

    void VirtualClient::incrementRequest() {
        _lastRequestId = (_lastRequestId % 1000 + 1) % 1000 + _id;
    }

    const bool& VirtualClient::isRequestFromThisPlayer(const long& __requestId) const {
        return (_id % 1000 == __requestId % 1000);
    }

    void VirtualClient::incrementNbCardsPlayed() { _cardsPlayed += 1; }

    void VirtualClient::cardsPlayedTo0() { _cardsPlayed = 0; }
}  // namespace theGame