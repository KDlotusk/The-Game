#include <iostream>

#include "request.hpp"


using namespace std;

namespace theGame {

//private:
    void RequestManager::_fillOptions(string __opt) {
        _options.clear();

        string delimiter = " ";

        size_t pos = 0;
        string token;

        while ((pos = __opt.find(delimiter)) != string::npos) {
            token = __opt.substr(0, pos);
            _options.push_back(token);
            __opt.erase(0, pos + delimiter.length());
        }
        _options.push_back(__opt);

        // remove the first element of the vector, tests shows that the first element is a blank element left by the "while" part of the function
        _options.erase(_options.begin()); 
    }
    Group* RequestManager::_createGroup() { 
        Group* group = new Group(_groups.size());

        _groups.push_back(group);

        return group;
    }
    void RequestManager::_removeGroup(const long& __id) {
        for (size_t k = 0; k < _groups.size(); k++) {
            if (_groups[k]->getId() == __id) {
                delete _groups[k];
                _groups.erase(_groups.begin() + k);
            }
        }
    }
    Group* RequestManager::_findGroupById(const long& __id) const {
        for (size_t k = 0; k < _groups.size(); k++) {
            if (_groups[k]->getId() == __id) {
                return _groups[k];
            }
        }

        return nullptr;
    }
    Group* RequestManager::_findGroupByRequest(const long& __requestId) const {
        for(size_t k = 0; k < _groups.size(); k++) {
            if(_groups[k]->isRequestFromThisGroup(__requestId)) {
                return _groups[k];
            }
        }
        return nullptr;
    }
    Group* RequestManager::_findGroupByFd(const long& __fileDescriptor) const {
        for(size_t k = 0; k < _groups.size(); k++) {
            vector<VirtualClient*> clients = _groups[k]->getClients();
            for(size_t j = 0; j < clients.size(); j++) {
                if(clients[j]->getFileDescriptor() == __fileDescriptor)
                    return _groups[k];
            }
        }
        return nullptr;
    }

    string RequestManager::_seeGroups() const {
        vector<pair<int, int>> idGroupsPossible;
        for(size_t k = 0; k < _groups.size(); k++) {
            if(idGroupsPossible.size() < 10) {
                if(_groups[k]->getStatus() == 0) {
                    if(_groups[k]->getNbOfClient() < 5)
                        idGroupsPossible.push_back(make_pair(_groups[k]->getId(), _groups[k]->getNbOfClient()));
                }
            }
            else {
                break;
            }
        }

        string str = to_string(idGroupsPossible.size());
        for(size_t k = 0; k < idGroupsPossible.size(); k++) {
            str+= " " + to_string(idGroupsPossible[k].first) + " " + to_string(idGroupsPossible[k].second);
        }

        return str;
    }

    VirtualClient* RequestManager::_createClient(const int& __fileDescriptor) {
        VirtualClient* client = new VirtualClient(_usableValuesForIdClient, __fileDescriptor);
        _clients.push_back(client);

        _usableValuesForIdClient++;

        return client;
    }
    void RequestManager::_removeClient(const long& __id) {
        for(size_t k = 0; k < _clients.size(); k++) {
            if(_clients[k]->getId() == __id) {
                delete _clients[k];
                _clients.erase(_clients.begin() + k);
            }
        }
    }
    VirtualClient* RequestManager::_findClientByRequest(const long& __requestId) const {
        for(size_t k = 0; k < _clients.size(); k++) {
            if(_clients[k]->isRequestFromThisPlayer(__requestId)) {
                return _clients[k];
            }
        }
        return nullptr;
    }
    VirtualClient* RequestManager::_findClientByFd(const long& __fileDescriptor) const {
        for(size_t k = 0; k < _clients.size(); k++) {
            if(_clients[k]->getFileDescriptor() == __fileDescriptor) {
                return _clients[k];
            }
        }
        return nullptr;
    }

//public:
    RequestManager::~RequestManager() {
        for(size_t k = 0; k < _groups.size(); k++) {
            delete _groups[k];
        }
        for(size_t k = 0; k < _clients.size(); k++) {
            delete _clients[k];
        }
    }

    theGame::ReturnRequest* RequestManager::disconnect(const int& __fileDescriptor) {
        Group* group = _findGroupByFd(__fileDescriptor);
        VirtualClient* client = _findClientByFd(__fileDescriptor);

        client->disconnect();

        ReturnRequest* request = new ReturnRequest();

        if(group != nullptr) {

            vector<VirtualClient*> clients = group->getClients();

            if(group->getStatus() == 2) {
                group->removeClient(client->getId());
            }
            else if (group->getStatus() == 0 ) {
                group->removeClient(client->getId());

                for(size_t k = 0; k < clients.size(); k++) {
                    request->addNext("GMINF " + to_string(group->getAsyncCode()) + " " + to_string(group->getNbOfClient()), clients[k]->getFileDescriptor());
                }

                if(group->getNbOfClient() == 0) {
                    _removeGroup(group->getId());
                }   
            }
            else {

                for(size_t k = 0; k < clients.size(); k++) {
                    if(clients[k]->isConnected())
                        request->addNext("DISCO " + to_string(clients[k]->getLastRequestId()) + " " + to_string(client->getId()), clients[k]->getFileDescriptor());
                }

                return request;
            }
        }

        return request;
    }

    ReturnRequest* RequestManager::updateTimer() {
        

        ReturnRequest* request = new ReturnRequest();

        for(size_t k = 0; k < _clients.size(); k++) {
            if(_clients[k]->getTimeElapsedSinceLastRequest() /60 >= 9) { // si ça fait 9 minutes ou plus que le client n'a rien envoyé
                request->addNext("ALIVE " + to_string(_clients[k]->getLastRequestId()), _clients[k]->getFileDescriptor());
                cout << "client " + to_string(_clients[k]->getFileDescriptor()) + " has not talked in a while, please answer" << endl;
            }
            if(_clients[k]->getTimeElapsedSinceLastRequest() /60 >= 10) {
                cout << "client " + to_string(_clients[k]->getFileDescriptor()) + " is considered disconnected" << endl;


                Group* group = _findGroupByRequest(_clients[k]->getLastRequestId());
                
                if(group != nullptr) {
                    group->removeClient(_clients[k]->getId());

                    for(size_t k = 0; k < group->getClients().size(); k++) {
                        request->addNext("ENDGM " + to_string(group->endOfGame()) + " 2", group->getClients()[k]->getFileDescriptor());
                    } 
                    _removeGroup(group->getId());
                }

                _removeClient(_clients[k]->getId());
            }
        }

        return request;
    }

    ReturnRequest* RequestManager::request(const std::string& __str, const int& __fileDescriptor) {
        
        int requestName = DEFAULT;

        if(_requests.find(__str.substr(0, 5)) != _requests.end())
            requestName = _requests[__str.substr(0, 5)];
        else
            return new ReturnRequest("ERROR 400", __fileDescriptor); // requête non reconnus

        
        string opt = __str.substr(5);
        _fillOptions(opt);
        if(_options.size() == 0 && requestName != CONEC) {
            return new ReturnRequest("ERROR 411", __fileDescriptor); //aucune option n'a été reçu
        }


        int requestID = 0;         
        try {
            requestID = stoi(_options[0]);
        }
        catch(const exception& e) {

            if(requestName != CONEC)
                return new ReturnRequest("ERROR 403", __fileDescriptor);// l'id de la requête n'est pas valable
        }

        ReturnRequest* request = nullptr;


        VirtualClient* client = _findClientByRequest(requestID);
        if(client != nullptr) {
            client->setLastRequestId(requestID);
            if(!client->isConnected()) {
                client->reconnect(__fileDescriptor);
            }
        }
        else {
            if(requestName != CONEC) {
                return new ReturnRequest("ERROR 412", __fileDescriptor);// utilisateur non reconnu
            }
        }


        //Dealing with the requests
        switch(requestName) {
            case CONEC: {
                    VirtualClient* client = _createClient(__fileDescriptor);
                    return new ReturnRequest("VALID " + to_string(client->getId()) , __fileDescriptor);
                }
                break;
            case SEEGM: {
                return new ReturnRequest("SNDGM " + to_string(client->getLastRequestId()) + " " + _seeGroups(), __fileDescriptor);
            }
                break;
            case JOING: {

                Group* group;

                group = _findGroupByRequest(client->getLastRequestId());
                if(group != nullptr) {
                    return new ReturnRequest("ERROR 304", __fileDescriptor); // Vous êtes déjà dans un groupe, vous ne pouvez pas en rejoindre un autre
                }

                //add client to the desired group
                if(_options.size() < 2) {
                    return new ReturnRequest("ERROR 415", __fileDescriptor);// certaines valeurs d'options sont manquante
                }
                int groupId;
                try{
                    groupId = stoi(_options[1]);
                }
                catch(const exception& e) {
                    return new ReturnRequest("ERROR 410", __fileDescriptor);// mauvaise valeur d'option envoyé
                }

                group = _findGroupById((long)groupId);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 417", __fileDescriptor);// le group n'existe pas
                }

                if(group->getStatus() != 0 || group->getNbOfClient() >= 5) {
                    return new ReturnRequest("ERROR 201", __fileDescriptor);// la partie n'est pas disponible
                }
                group->addClient(client);

                if(group->getAsyncCode() == -1) {
                    group->setAsyncCode(group->getId() + client->getLastRequestId());
                }

                request = new ReturnRequest("ASYNC " + to_string(client->getLastRequestId()) + " " + to_string(group->getAsyncCode()), __fileDescriptor);               

                //notify the others with GMINF
                vector<int> fds = group->getAllFileDescriptor();

                for(size_t k = 0; k < fds.size(); k++) {
                    request->addNext("GMINF " + to_string(group->getAsyncCode()) + " " + to_string(group->getNbOfClient()), fds[k]);
                }

                return request;
            }
                break;
            case LEAVE: {
                Group* group = _findGroupByRequest(requestID);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 416", __fileDescriptor); // utilisateur n'est pas en jeux
                }

                request = new ReturnRequest();
                vector<int> fds = group->getAllFileDescriptor();

                if(group->getStatus() == 0) {

                    group->removeClient(client->getId());


                    request->addNext("ASYNC " + to_string(group->getAsyncCode()), __fileDescriptor);

                    
                    for(size_t k = 0; k < fds.size(); k++) {
                        request->addNext("GMINF " + to_string(group->getAsyncCode()) + " " + to_string(group->getNbOfClient()), fds[k]);
                    }

                    if(group->getNbOfClient() == 0) {
                        _removeGroup(group->getId());
                    }   
                }


                if(group->getStatus() == 1) {
                    for(size_t k = 0; k < fds.size(); k++) {
                        request->addNext("ENDGM " + to_string(group->endOfGame()) + " 1", fds[k]);
                    } 
                    group->removeClient(client->getId());
                    _removeGroup(group->getId());
                }   

                return request;
            }
                break;
            case MAKEG: {
                Group* group = _createGroup();
                group->addClient(client);

                if(group->getAsyncCode() == -1) {
                    group->setAsyncCode(group->getId() + client->getLastRequestId());
                }

                request= new ReturnRequest("VALID " + to_string(client->getLastRequestId()) + " " + to_string(group->getId()), __fileDescriptor);
                request->addNext ("ASYNC " + to_string(client->getLastRequestId()) + " " + to_string(group->getAsyncCode()), __fileDescriptor);               


                return request;                    
            }
                break;
            case SRTGM: {
                Group* group = _findGroupByRequest(requestID);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 416", __fileDescriptor); // utilisateur n'est pas en jeux
                }

                if(!group->startGame()) {
                    return new ReturnRequest("ERROR 204", __fileDescriptor); // Il n y a pas assez de joueurs en jeux pour commencer
                }

                vector<VirtualClient*> clients = group->getClients();

                request = new ReturnRequest();
                for(size_t k = 0; k < clients.size(); k++) {
                    string requestSNDSK = "SNDSK " + to_string(clients[k]->getLastRequestId()) + " " + group->sendPiles();
                    string requestSNDHD = "SNDHD " + to_string(clients[k]->getLastRequestId()) + " " + clients[k]->asRequest();

                    request->addNext("START " + to_string(clients[k]->getLastRequestId()) + " " + requestSNDSK + " " + requestSNDHD, clients[k]->getFileDescriptor());
                    request->addNext("ASYNC " + to_string(group->getAsyncCode()), clients[k]->getFileDescriptor());
                }

                request->addNext("UTURN " + to_string(group->getClients()[group->getCurrentClient()]->getLastRequestId()), group->getClients()[group->getCurrentClient()]->getFileDescriptor());
                
                group->getClients()[group->getCurrentClient()]->setTimerAtMinus9();

                group->setAsyncCode(-1);

                return request;
            }
                break;
            case CTPLY: { // can't play -> end of the game 
                Group* group = _findGroupByRequest(requestID);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 416", __fileDescriptor); // utilisateur n'est pas en jeux
                }

                int nbCardsNotPlayed = group->endOfGame();
                vector<VirtualClient*> clients = group->getClients();

                request = new ReturnRequest();
                for(size_t k = 0; k < clients.size(); k++) {
                    request->addNext("ENDGM " + to_string(nbCardsNotPlayed) + " 0", clients[k]->getFileDescriptor());
                } 

                _removeGroup(group->getId());

                return request;
            }
                break;
            case PLAY_: {
                Group* group = _findGroupByRequest(requestID);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 416", __fileDescriptor); // utilisateur n'est pas en jeux
                }

                if(_options.size() < 3) {
                    return new ReturnRequest("ERROR 415", __fileDescriptor); // certaine valeur d'option sont manquante
                }

                try{
                    int returnValue = group->play(requestID, stoi(_options[1]), stoi(_options[2]));

                    if(returnValue != 0) {
                        return new ReturnRequest("ERROR "+ to_string(returnValue), __fileDescriptor);
                    }
                }
                catch(const exception& e){
                    return new ReturnRequest("ERROR 410", __fileDescriptor); // mauvaise valeur d'option envoyé
                }

                client->setTimerAtMinus9();


                string requestSNDHD = "SNDHD " + to_string(client->getLastRequestId()) + " " + group->sendHandCurrentPlayer();
                string requestSNDSK = "SNDSK " + to_string(client->getLastRequestId()) + " " + group->sendPiles();
                string requestCNCAT = "CNCAT " + requestSNDSK + " " + requestSNDHD;

                request = new ReturnRequest(requestCNCAT, group->getFileDescriptorCurrentPlayer());

                vector<VirtualClient*> clients = group->getClients();

                for(size_t k = 0; k < clients.size(); k++) {
                    if(clients[k]->getId() != client->getId()) {
                        requestSNDSK = "SNDSK " + to_string(clients[k]->getLastRequestId()) + " " + group->sendPiles();

                        request->addNext(requestSNDSK, clients[k]->getFileDescriptor());
                    }
                }

                return request;
            }
                break;
            case ENTRN: {
                Group* group = _findGroupByRequest(requestID);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 416", __fileDescriptor); // utilisateur n'est pas en jeux
                }

                group->endOfTurn();
                request = new ReturnRequest("ACKIT " + to_string(client->getLastRequestId()), client->getFileDescriptor());
                request->addNext("UTURN " + to_string(group->getClients()[group->getCurrentClient()]->getLastRequestId()), group->getClients()[group->getCurrentClient()]->getFileDescriptor());
                
                group->getClients()[group->getCurrentClient()]->setTimerAtMinus9();

                //send ENDRW if needed
                if(group->isStackEmpty()) {
                    vector<VirtualClient*> clients = group->getClients();

                    for(size_t k = 0; k < clients.size(); k++) {
                        request->addNext("ENDRW", clients[k]->getFileDescriptor());
                    }
                }

                return request;
            }
                break;
            case MESSG: {
                Group* group = _findGroupByRequest(requestID);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 416", __fileDescriptor); // utilisateur n'est pas en jeux
                }

                if(group->getStatus() == 1) {

                    vector<VirtualClient*> clients = group->getClients();
                    
                    request = new ReturnRequest();
                    for(size_t k = 0; k < clients.size(); k++) {
                        request->addNext(__str, clients[k]->getFileDescriptor()); // send the message directly to all the clients
                    }

                    return request;
                }

            }
                break;
            case RONEC: {
                if(client == nullptr) {
                    return new ReturnRequest("ERROR 412", __fileDescriptor);
                }

                return new ReturnRequest("VALID " + to_string(client->getLastRequestId()), __fileDescriptor);
            }
                break;
            default:
                if(requestName != ACKIT)
                    return new ReturnRequest("ERROR 401", __fileDescriptor); // requête ne devrait pas être envoyer par le client
                break;
        }


        return new ReturnRequest();
    }
}  // namespace theGame