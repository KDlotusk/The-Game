#include <string>
#include <iostream>

#include "VirtualClient.hpp"

using namespace std;

namespace theGame {
    VirtualClient::VirtualClient(const long& __id, const int& __fileDescriptor) {
        _id = (1 + __id) * 100000;
        _fileDescriptor = __fileDescriptor;

        _hand = new Hand(std::vector<Card>());
        _isConnected = true;

        _timeLastRecord = time(NULL);
    }

    VirtualClient::~VirtualClient() { delete _hand; }

    long VirtualClient::getId() const { return _id; }
    Hand* VirtualClient::getHand() const { return _hand; }
    int VirtualClient::getCardsPlayed() const { return _cardsPlayed; }
    int VirtualClient::getFileDescriptor() const { return _fileDescriptor; }
    int VirtualClient::getLastRequestId() const { return _lastRequestId; }
    bool VirtualClient::isConnected() const { return _isConnected; }
    time_t VirtualClient::getTimeElapsedSinceLastRequest() { return difftime(time(NULL), _timeLastRecord); }

    void VirtualClient::resetFileDescriptor() {
        _fileDescriptor = -1;
    }
    void VirtualClient::setFileDescriptor(const int& __fileDescriptor) { _fileDescriptor = __fileDescriptor; }

    void VirtualClient::setTimerAtMinus9() { //in case other players are waiting on him
        _timeLastRecord = time(NULL) - 60 * 9;
    }

    void VirtualClient::setLastRequestId(const int& __lastRequestId) { 
        _lastRequestId = __lastRequestId; 
        _timeLastRecord = time(NULL);
    }
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

    void VirtualClient::disconnect() {
        resetFileDescriptor();
        _isConnected = false;

        cout << "client " + to_string(_id) + " is disconnected" << endl;
    }

    void VirtualClient::reconnect(const int& __fileDescriptor) {
        setFileDescriptor(__fileDescriptor);
        _isConnected = true;

        cout << "client " + to_string(_id) + " is connected" << endl;

    }

    void VirtualClient::incrementNbCardsPlayed() { _cardsPlayed += 1; }
    void VirtualClient::cardsPlayedTo0() { _cardsPlayed = 0; }

    string VirtualClient::asRequest() const {
        return _hand->asRequest();
    }
}  // namespace theGame