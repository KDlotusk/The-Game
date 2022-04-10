#pragma once

#include<array>
#include <utility>
#include "VirtualCLient.h"
#include "Pile.h"

using namespace std;


class Group {
private:
    pair<int, int> nbMinMaxPlayer = make_pair(2, 5);
    int nbCardsPerHand = 6;

    long idGroup;
    vector<VirtualClient*> clients;
    Stack stack;

    array<Pile, 4> piles = { Pile(true), Pile(true), Pile(false), Pile(false) };

    int status = 0; // 1: in game / 0: loading / 2: end of game

    int currentPlayer = 0;

    int asyncCode = -1;

public:
    Group(long _idGroup) {
        idGroup = _idGroup;
    }

    bool isRequestFromThisGroup(long requestId) {
        for(int k = 0; k < clients.size(); k++) {
            if(clients[k]->isRequestFromThisPlayer(requestId))
                return true;
        }
        return false;
    }

    VirtualClient* getGameMaster() { return clients[0]; }
    long getId() {return idGroup;} 
    int getStatus() {return status;}

    void addClient(VirtualClient* client) {
        clients.push_back(client);
    }
    void removeClient(long id) {
        for(int k = 0; k < clients.size(); k++) {
            if(clients[k]->getId() == id ) {
                clients.erase(clients.begin() + k);
            }
        }
    }

    vector<VirtualClient*> getClients() {
        return clients;
    }

    int getNbOfClient() {
        return clients.size();
    }

    int getCurrentClient() {
        return currentPlayer;
    }

    int isStackEmpty() {
        return stack.isEmpty();
    }

    int getAsyncCode() {
        return asyncCode;
    }

    void setAsyncCode(int _asyncCode) {
        asyncCode = _asyncCode;
    }

    bool startGame() {
        if(getNbOfClient() > nbMinMaxPlayer.first && getNbOfClient() < nbMinMaxPlayer.second) {

            array<Pile, 4> piles = { Pile(true), Pile(true), Pile(false), Pile(false) };
            stack.reset();
            
            // set the hand of the players
            if(clients.size() == 2) {
                nbCardsPerHand = 7;
            }
            if(clients.size() > 2) {
                nbCardsPerHand = 8;
            }

            for(int k = 0;  k < clients.size(); k++) {
                clients[k]->addCards(stack.draw(nbCardsPerHand));
            }

            status = 1;


            return true;
        }
        return false;
    }

    int play(long requestId, int pile, int cardId) { // TODO correct accordingly

        if(status == 1) {
            if(clients[currentPlayer]->isRequestFromThisPlayer(requestId)) {
                if(clients[currentPlayer]->getHand()->size() > cardId && cardId > 0) {
                    int cardNumber = clients[currentPlayer]->getHand()->getCard(cardId).getValue();

                    piles[pile].playCard(Card(cardNumber));

                    clients[currentPlayer]->incrementNbCardsPlayed();
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

    void endOfTurn() {
        clients[currentPlayer]->addCards(stack.draw(clients[currentPlayer]->getCardsPlayed())); // we draw the cards for the current player

        currentPlayer = (currentPlayer+1)%clients.size();
    }

    string sendPiles() {
        string str = "4";

        for(int k = 0; k < 4; k++) {
            str += " " + piles[k].asRequest();
        }

        return str;
    }

    string sendHandCurrentPlayer() {
        return clients[currentPlayer]->asRequest();
    }

    int getFileDescriptorCurrentPLayer() {
        return clients[currentPlayer]->getFileDescriptor();
    }

    vector<int> getAllFileDescriptor() {
        vector<int> fds;

        for(int k = 0; k < clients.size() ; k++) {
            fds.push_back(clients[k]->getFileDescriptor());
        }

        return fds;
    }

    vector<int> getAllFileDescriptorButCurrentPLayer() {
        vector<int> fds;

        for(int k = 0; k < clients.size() ; k++) {
            if(k != currentPlayer)
                fds.push_back(clients[k]->getFileDescriptor());
        }

        return fds;
    }

    int nbCardsNotPLayed() {
        return 0; // TODO
    }

    int endOfGame() {
        status = 2;
        return nbCardsNotPLayed();
    }
};