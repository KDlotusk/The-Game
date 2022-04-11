#include <string>

#include "VirtualClient.hpp"

using namespace std;

namespace theGame {
    VirtualClient::VirtualClient(const long& __id, const int& __fileDescriptor) {
        _id = (1 + __id) * 100000;
        _fileDescriptor = __fileDescriptor;

        _hand = new Hand(std::vector<Card>());
    }

    VirtualClient::~VirtualClient() { delete _hand; }

    long VirtualClient::getId() const { return _id; }
    Hand* VirtualClient::getHand() const { return _hand; }
    int VirtualClient::getCardsPlayed() const { return _cardsPlayed; }
    int VirtualClient::getFileDescriptor() const { return _fileDescriptor; }
    int VirtualClient::getLastRequestId() const { return _lastRequestId; }

    void VirtualClient::setLastRequestId(const int& __lastRequestId) { _lastRequestId = __lastRequestId; }
    void VirtualClient::incrementRequest() {
        _lastRequestId = (_lastRequestId%1000 +1)%1000 + _id;
    }

    void VirtualClient::addCards(const vector<Card>& __cards) {
        for (size_t k = 0; k < __cards.size(); k++) {
            _hand->addCard(__cards[k]);
        }
    }

    bool VirtualClient::isRequestFromThisPlayer(const long& __requestId) const {
        return (_id / 1000 == __requestId / 1000);
    }

    void VirtualClient::incrementNbCardsPlayed() { _cardsPlayed += 1; }
    void VirtualClient::cardsPlayedTo0() { _cardsPlayed = 0; }

    string VirtualClient::asRequest() const {
        return _hand->asRequest();
    }
}  // namespace theGame