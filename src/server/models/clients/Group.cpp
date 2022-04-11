#include "Group.hpp"

using namespace std;

namespace theGame {
    Group::Group(long __idGroup) {
        _idGroup = (1 + __idGroup) * 100000 + 1;
    }

    const bool& Group::isRequestFromThisGroup(const long& __requestId) const {
        for (size_t k = 0; k < _clients.size(); k++) {
            if (_clients[k]->isRequestFromThisPlayer(__requestId)) return true;
        }
        return false;
    }

    VirtualClient* Group::getGameMaster() const { return _clients[0]; }
    const long& Group::getId() const { return _idGroup; }
    const int& Group::getStatus() const { return _status; }

    void Group::addClient(VirtualClient* __client) {
        _clients.push_back(__client);
    }
    void Group::removeClient(const long& __id) {
        for (size_t k = 0; k < _clients.size(); k++) {
            if (_clients[k]->getId() == __id) {
                _clients.erase(_clients.begin() + k);
            }
        }
    }

    vector<VirtualClient*> Group::getClients() const { return _clients; }
    const int& Group::getNbOfClient() const { return _clients.size(); }
    const int& Group::getCurrentClient() const { return _currentClient; }
    const bool& Group::isStackEmpty() const { return _stack.isEmpty(); }
    const int& Group::getAsyncCode() const { return _asyncCode; }
    const int& Group::getFileDescriptorCurrentPlayer() const { return _clients[_currentClient]->getFileDescriptor(); }
    void Group::setAsyncCode(const int& asyncCode) { _asyncCode = asyncCode; }

    const std::string& Group::sendPiles() const {
        string str = "4";

        for(size_t k = 0; k < 4; k++) {
            str += " " + _piles[k].asRequest();
        }

        return str;
    }
    const std::string& Group::sendHandCurrentPlayer() const {
        return _clients[_currentClient]->asRequest();
    }

    const int& Group::getFileDescriptorCurrentPLayer() const {
        return _clients[_currentClient]->getFileDescriptor();
    }
    const std::vector<int>& Group::getAllFileDescriptor() const {
        vector<int> fds;

        for(size_t k = 0; k < _clients.size() ; k++) {
            fds.push_back(_clients[k]->getFileDescriptor());
        }

        return fds;
    }
    const std::vector<int>& Group::getAllFileDescriptorButCurrentPlayer() const {
        vector<int> fds;

        for(size_t k = 0; k < _clients.size() ; k++) {
            if(k != _currentClient)
                fds.push_back(_clients[k]->getFileDescriptor());
        }

        return fds;
    }
    const int& Group::nbCardsNotPLayed() const {
        int count = 0;
        count += _stack.size();

        for(size_t k = 0; k < _clients.size(); k++) {
            count += _clients[k]->getHand()->size();
        }

        return count;
    }


    const bool& Group::startGame() {
        if (getNbOfClient() > _nbMinMaxPlayer.first &&
            getNbOfClient() < _nbMinMaxPlayer.second) {
            array<Pile, 4> piles = { Pile(true), Pile(true), Pile(false), Pile(false) };
            _stack.reset();

            // set the hand of the players
            if (_clients.size() == 2) {
                _nbCardsPerHand = 7;
            }
            if (_clients.size() > 2) {
                _nbCardsPerHand = 8;
            }

            for (size_t k = 0; k < _clients.size(); k++) {
                _clients[k]->addCards(_stack.draw(_nbCardsPerHand));
            }

            _status = 1;

            return true;
        }
        return false;
    }

    const int& Group::play(const long& __requestId, const int& __pile, const int& __cardId) {
        if(_status == 1) {
            if(_clients[_currentClient]->isRequestFromThisPlayer(__requestId)) {
                if(_clients[_currentClient]->getHand()->size() > __cardId && __cardId > 0) {
                    int cardNumber = _clients[_currentClient]->getHand()->getCard(__cardId).getValue();

                    _piles[__pile].playCard(Card(__cardId));

                    _clients[_currentClient]->incrementNbCardsPlayed();
                }
                else {
                    return 414; // cette carte n'existe pas
                }
            }
            else {
                return 303; // Ce n'est pas votre tour
            }
        }
        else {
            return 205; // la partie n'est pas en cours
        }
        

        return 0;
    }

    void Group::endOfTurn() {
        _clients[_currentClient]->addCards(_stack.draw(
            _clients[_currentClient]
                ->getCardsPlayed()));  // we draw the cards for the current player

        _clients[_currentClient]->cardsPlayedTo0();

        _currentClient = (_currentClient + 1) % _clients.size();
    }

    const int& Group::endOfGame() {
        _status = 2;
        return nbCardsNotPLayed();
    }
}  // namespace theGame