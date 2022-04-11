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


//public:
    RequestManager::~RequestManager() {
        for(size_t k = 0; k < _groups.size(); k++) {
            delete _groups[k];
        }
        for(size_t k = 0; k < _clients.size(); k++) {
            delete _clients[k];
        }
    }

    Group* RequestManager::createGroup() { 
        Group* group = new Group(_groups.size());

        _groups.push_back(group);

        return group;
    }
    void RequestManager::removeGroup(const long& __id) {
        for (size_t k = 0; k < _groups.size(); k++) {
            if (_groups[k]->getId() == __id) {
                delete _groups[k];
                _groups.erase(_groups.begin() + k);
            }
        }
    }
    Group* RequestManager::findGroupById(const long& __id) const {
        for (size_t k = 0; k < _groups.size(); k++) {
            if (_groups[k]->getId() == __id) {
                return _groups[k];
            }
        }

        return nullptr;
    }
    Group* RequestManager::findGroupByRequest(const long& __requestId) const {
        for(size_t k = 0; k < _groups.size(); k++) {
            if(_groups[k]->isRequestFromThisGroup(__requestId)) {
                return _groups[k];
            }
        }
        return nullptr;
    }

    string RequestManager::seeGroups() const {
        vector<pair<int, int>> idGroupsPossible;
        for(size_t k = 0; k < _groups.size(); k++) {
            if(idGroupsPossible.size() < 10) {
                if(_groups[k]->getStatus() == 0) {
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

    VirtualClient* RequestManager::createClient(const int& __fileDescriptor) {
        VirtualClient* client = new VirtualClient(_clients.size(), __fileDescriptor);
        _clients.push_back(client);

        return client;
    }
    void RequestManager::removeClient(const long& __id) {
        for(size_t k = 0; k < _groups.size(); k++) {
            if(_clients[k]->getId() == __id) {
                delete _clients[k];
                _clients.erase(_clients.begin() + k);
            }
        }
    }
    VirtualClient* RequestManager::findClientByRequest(const long& __requestId) const {
        for(size_t k = 0; k < _clients.size(); k++) {
            if(_clients[k]->isRequestFromThisPlayer(__requestId)) {
                return _clients[k];
            }
        }
        return nullptr;
    }



    ReturnRequest* RequestManager::request(const std::string& __str, const int& __fileDescriptor) {
        
        int requestName = DEFAULT;

        if(_requests.find(__str.substr(0, 5)) != _requests.end())
            requestName = _requests[__str.substr(0, 5)];
        
        string opt = __str.substr(5);
        _fillOptions(opt);
        if(_options.size() == 0) {
            return new ReturnRequest("ERROR 411", __fileDescriptor); //aucune option n'a été reçu
        }


        int requestID;         
        try {
            requestID = stoi(_options[0]);
        }
        catch(const exception& e) {

            if(requestName != CONEC)
                return new ReturnRequest("ERROR 403", __fileDescriptor);// l'id de la requête n'est pas valable
        }

        ReturnRequest* request;


        VirtualClient* client = findClientByRequest(requestID);
        if(client != nullptr) {
            client->setLastRequestId(requestID);
        }
        else {
            if(requestName != CONEC) {
                return new ReturnRequest("ERROR 412", __fileDescriptor);// utilisateur non reconnu
            }
        }

        switch(requestName) {
            case CONEC: {
                    VirtualClient* client = createClient(__fileDescriptor);
                    return new ReturnRequest("VALID " + to_string(client->getId()) , __fileDescriptor);
                }
                break;
            case SEEGM: {
                return new ReturnRequest("SNDGM " + to_string(client->getLastRequestId()) + " " + seeGroups(), __fileDescriptor);
            }
                break;
            case JOING: {
                //add client to the desired group
                if(_options.size() < 2) {
                    return new ReturnRequest("ERROR 415", __fileDescriptor);// certaines valeurs d'_options sont manquante
                }
                int groupId;
                try{
                    groupId = stoi(_options[1]);
                }
                catch(const exception& e) {
                    return new ReturnRequest("ERROR 410", __fileDescriptor);// mauvaise valeur d'option envoyé
                }

                Group* group = findGroupById((long)groupId);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 417", __fileDescriptor);// le group n'existe pas
                }

                if(group->getStatus() != 0) {
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
                Group* groupTarget = findGroupByRequest(requestID);
                if(groupTarget == nullptr) {
                    return new ReturnRequest("ERROR 416", __fileDescriptor); // utilisateur n'est pas en jeux
                }

                request = new ReturnRequest();

                request->addNext("ASYNC " + to_string(groupTarget->getAsyncCode()), __fileDescriptor);


                groupTarget->removeClient(client->getId());
                vector<int> fds = groupTarget->getAllFileDescriptor();

                if(groupTarget->getStatus() == 0) {
                    
                    for(size_t k = 0; k < fds.size(); k++) {
                        request->addNext("GMINF " + to_string(groupTarget->getAsyncCode()) + " " + to_string(groupTarget->getNbOfClient()), fds[k]);
                    }

                    if(groupTarget->getNbOfClient() == 0) {
                        removeGroup(groupTarget->getId());
                    }   
                }


                if(groupTarget->getStatus() == 1) {
                    for(size_t k = 0; k < fds.size(); k++) {
                        request->addNext("ENDGM " + to_string(groupTarget->endOfGame()) + " 1", fds[k]);
                    } 
                    removeGroup(groupTarget->getId());
                }   

                return request;
            }
                break;
            case MAKEG: {
                Group* group = createGroup();
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
                Group* group = findGroupByRequest(requestID);
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


                    request->addNext("ASYNC " + to_string(group->getAsyncCode()), clients[k]->getFileDescriptor());
                    request->addNext("START " + to_string(clients[k]->getLastRequestId()) + " " + requestSNDSK + " " + requestSNDHD, clients[k]->getFileDescriptor());
                }

                return request;
            }
                break;
            case CTPLY: { // can't play -> end of the game 
                Group* group = findGroupByRequest(requestID);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 416", __fileDescriptor); // utilisateur n'est pas en jeux
                }

                int nbCardsNotPlayed = group->endOfGame();
                vector<VirtualClient*> clients = group->getClients();

                request = new ReturnRequest();
                for(size_t k = 0; k < clients.size(); k++) {
                    request->addNext("ENDGM " + to_string(nbCardsNotPlayed) + " 0", clients[k]->getFileDescriptor());
                } 

                removeGroup(group->getId());

                return request;
            }
                break;
            case PLAY_: {
                Group* groupTarget = findGroupByRequest(requestID);
                if(groupTarget == nullptr) {
                    return new ReturnRequest("ERROR 416", __fileDescriptor); // utilisateur n'est pas en jeux
                }

                if(_options.size() < 3) {
                    return new ReturnRequest("ERROR 415", __fileDescriptor); // certaine valeur d'option sont manquante
                }

                try{
                    int returnValue = groupTarget->play(requestID, stoi(_options[1]), stoi(_options[2]));

                    if(returnValue != 0) {
                        return new ReturnRequest("ERROR "+ to_string(returnValue), __fileDescriptor);
                    }
                }
                catch(const exception& e){
                    return new ReturnRequest("ERROR 410", __fileDescriptor); // mauvaise valeur d'option envoyé
                }


                string requestSNDHD = "SNDHD " + to_string(client->getLastRequestId()) + " " + groupTarget->sendHandCurrentPlayer();
                string requestSNDSK = "SNDSK " + to_string(client->getLastRequestId()) + " " + groupTarget->sendPiles();
                string requestCNCAT = "CNCAT " + requestSNDSK + " " + requestSNDHD;

                request = new ReturnRequest(requestCNCAT, groupTarget->getFileDescriptorCurrentPlayer());

                vector<VirtualClient*> clients = groupTarget->getClients();

                for(size_t k = 0; k < clients.size(); k++) {
                    if(clients[k]->getId() != client->getId())
                    requestSNDSK = "SNDSK " + to_string(clients[k]->getLastRequestId()) + " " + groupTarget->sendPiles();

                    request->addNext(requestSNDSK, clients[k]->getFileDescriptor());
                }

                return request;
            }
                break;
            case ENTRN: {
                Group* group = findGroupByRequest(requestID);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 416", __fileDescriptor); // utilisateur n'est pas en jeux
                }

                group->endOfTurn();
                request = new ReturnRequest("ACKIT " + to_string(client->getLastRequestId()), client->getFileDescriptor());
                request->addNext("UTURN " + to_string(group->getClients()[group->getCurrentClient()]->getLastRequestId()), group->getClients()[group->getCurrentClient()]->getFileDescriptor());
                

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
                Group* group = findGroupByRequest(requestID);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 416", __fileDescriptor); // utilisateur n'est pas en jeux
                }

                vector<VirtualClient*> clients = group->getClients();
                
                request = new ReturnRequest();
                for(size_t k = 0; k < clients.size(); k++) {
                    request->addNext(__str, clients[k]->getFileDescriptor()); // send the message directly to all the clients
                }

                return request;
            }
                break;
            default:
                return new ReturnRequest("ERROR 401", __fileDescriptor); // requête ne devrait pas être envoyer par le client
                break;
        }


        return new ReturnRequest();
    }
}  // namespace theGame
