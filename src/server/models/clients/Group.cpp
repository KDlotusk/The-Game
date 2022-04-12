#include "Group.hpp"

using namespace std;

namespace theGame {
    Group::Group(long __idGroup) : _idGroup(__idGroup) {}

    const bool& Group::isRequestFromThisGroup(const long& __requestId) const {
        for (size_t k = 0; k < _clients.size(); k++) {
            if (_clients[k].isRequestFromThisPlayer(__requestId)) return true;
        }
        return false;
    }

    const VirtualClient& Group::getGameMaster() const { return _clients[0]; }
    const long& Group::getId() const { return _idGroup; }

    void Group::addClient(const VirtualClient& __client) {
        _clients.push_back(__client);
    }
    void Group::removeClient(const long& __id) {
        for (size_t k = 0; k < _clients.size(); k++) {
            if (_clients[k].getId() == __id) {
                _clients.erase(_clients.begin() + k);
            }
        }
    }

    const int& Group::getNbOfClient() const { return _clients.size(); }

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
                _clients[k].addCards(_stack.draw(_nbCardsPerHand));
            }

            _status = 1;

            return true;
        }
        return false;
    }

    void Group::play(
        const int& __pile, const int& __cardNumber, const long& __requestId) {
        _piles[__pile].playCard(Card(__cardNumber));

        for (size_t k = 0; k < _clients.size(); k++) {
            if (_clients[k].isRequestFromThisPlayer(
                    __requestId)) {  // TODO check, is this better like that or should
                                     // I use _currentPlayer ?
                _clients[k].incrementNbCardsPlayed();
            }
        }
    }

    void Group::endOfTurn() {
        _clients[_currentPlayer].addCards(_stack.draw(
            _clients[_currentPlayer]
                .getCardsPlayed()));  // we draw the cards for the current player

        _currentPlayer = (_currentPlayer + 1) % _clients.size();
    }
}  // namespace theGame