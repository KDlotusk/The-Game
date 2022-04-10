#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "requestEnum.h"
#include "Group.h"
#include "ReturnRequest.h"

using namespace std;

class RequestManager {
private:

    vector<Group*> groups;
    vector<VirtualClient*> clients;

    map<string, int> requests {
        {"CONEC", CONEC },
        {"ACKIT", ACKIT },
        {"ERROR", ERROR },
        {"VALID", VALID },
        {"ALIVE", ALIVE },
        {"SEEGM", SEEGM },
        {"JOING", JOING },
        {"LEAVE", LEAVE },
        {"MAKEG", MAKEG },
        {"SNDGM", SNDGM },
        {"GMINF", GMINF },
        {"ASYNC", ASYNC },
        {"START", START },
        {"ENDRW", ENDRW },
        {"UTURN", UTURN },
        {"PLAY_", PLAY_ },
        {"SNDHD", SNDHD },
        {"SNDSK", SNDSK },
        {"MESSG", MESSG },
        {"CNCAT", CNCAT },
        {"SRTGM", SRTGM },
        {"CTPLY", CTPLY},
        {"ENTRN", ENTRN}
    };

    void fillOptions(string opt, vector<string>& options) {
        

        string delimiter = " ";

        size_t pos = 0;
        string token;

        while ((pos = opt.find(delimiter)) != string::npos) {
            token = opt.substr(0, pos);
            options.push_back(token);
            opt.erase(0, pos + delimiter.length());
        }
        options.push_back(opt);
    }

public:

    ~RequestManager() {
        for(int k = 0; k < groups.size(); k++) {
            delete groups[k];
        }
        for(int k = 0; k < clients.size(); k++) {
            delete clients[k];
        }
    }

    Group* createGroup() {
        Group* group = new Group(groups.size());

        groups.push_back(group);

        return group;
    }
    void removeGroup(long id) {
        for(int k = 0; k < groups.size(); k++) {
            if(groups[k]->getId() == id) {
                delete groups[k];
                groups.erase(groups.begin() + k);
            }
        }
    }
    Group* findGroupById(long id) {
        for(int k = 0; k < groups.size(); k++) {
            if(groups[k]->getId() == id) {
                return groups[k];
            }
        }

        return nullptr;
    }

    Group* getGroupForRequest(long requestId) {
        for(int k = 0; k < groups.size(); k++) {
            if(groups[k]->isRequestFromThisGroup(requestId)) {
                return groups[k];
            }
        }

        return nullptr;
    }

    string seeGroups() {
        vector<pair<int, int>> idGroupsPossible;
        for(int k = 0; k < groups.size(); k++) {
            if(idGroupsPossible.size() < 10) {
                if(groups[k]->getStatus() == 1) {
                    idGroupsPossible.push_back(make_pair(groups[k]->getId(), groups[k]->getNbOfClient()));
                }
            }
            else {
                break;
            }
        }

        string str = to_string(idGroupsPossible.size());
        for(int k = 0; k < idGroupsPossible.size(); k++) {
            str+= " " + to_string(idGroupsPossible[k].first) + " " + to_string(idGroupsPossible[k].second);
        }

        return str;
    }

    VirtualClient* createClient(int fileDescriptor) {
        VirtualClient* client = new VirtualClient(clients.size(), fileDescriptor);
        clients.push_back(client);

        return client;
    }
    void removeClient(long id) {
        for(int k = 0; k < groups.size(); k++) {
            if(clients[k]->getId() == id) {
                delete clients[k];
                clients.erase(clients.begin() + k);

                //TODO : what to do with the groups that had the client ?
            }
        }
    }

    VirtualClient* getClientForRequest(long requestId) {
        for(int k = 0; k < clients.size(); k++) {
            if(clients[k]->isRequestFromThisPlayer(requestId)) {
                return clients[k];
            }
        }

        return nullptr;
    }

    ReturnRequest* request(string str, int fileDescriptor) {    

        int requestName = DEFAULT;

        if(requests.find(str.substr(0, 5)) != requests.end())
            requestName = requests[str.substr(0, 5)];
        
        string opt = str.substr(5);
        vector<string> options;
        fillOptions(opt, options);
        if(options.size() == 0) {
            return new ReturnRequest("ERROR 411", fileDescriptor); //aucune option n'a été reçu
        }


        int requestID; 
        
        try {
            requestID = stoi(options[0]);
        }
        catch(exception e) {
            return new ReturnRequest("ERROR 403", fileDescriptor);// l'id de la requête n'est pas valable
        }

        ReturnRequest* request;


        VirtualClient* client = getClientForRequest(requestID);
        if(client != nullptr) {
            client->setLastRequestId(requestID);
        }
        else {
            if(requestName != CONEC) {
                return new ReturnRequest("ERROR 412", fileDescriptor);// utilisateur non reconnu
            }
        }

        switch(requestName) {
            case CONEC: {
                    VirtualClient* client = createClient(fileDescriptor);
                    return new ReturnRequest("VALID " + client->getId(), fileDescriptor);
                }
                break;
            case SEEGM: {

                return new ReturnRequest("SNDGM " + requestID + seeGroups(), fileDescriptor);
            }
                break;
            case JOING: {
                //add client to the desired group
                if(options.size() < 2) {
                    return new ReturnRequest("ERROR 415", fileDescriptor);// certaines valeurs d'options sont manquante
                }
                int groupId;
                try{
                    groupId = stoi(options[1]);
                }
                catch(exception e) {
                    return new ReturnRequest("ERROR 410", fileDescriptor);// mauvaise valeur d'option envoyé
                }

                Group* group = findGroupById((long)groupId);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 417", fileDescriptor);// le group n'existe pas
                }

                if(group->getStatus() != 0) {
                    return new ReturnRequest("ERROR 201", fileDescriptor);// la partie n'est pas disponible
                }
                group->addClient(client);

                if(group->getAsyncCode() == -1) {
                    group->setAsyncCode(group->getId() + client->getLastRequestId());
                }

                request = new ReturnRequest("ASYNC " + to_string(client->getLastRequestId()) + " " + to_string(group->getAsyncCode()), fileDescriptor);               

                //notify the others with GMINF
                vector<int> fds = group->getAllFileDescriptor();

                for(int k = 0; k < fds.size(); k++) {
                    request->addNext("GMINF " + to_string(group->getAsyncCode()) + " " + to_string(group->getNbOfClient()), fds[k]);
                }

                return request;
            }
                break;
            case LEAVE: {
                Group* groupTarget = getGroupForRequest(requestID);
                if(groupTarget == nullptr) {
                    return new ReturnRequest("ERROR 416", fileDescriptor); // utilisateur n'est pas en jeux
                }

                request = new ReturnRequest();
                vector<int> fds = groupTarget->getAllFileDescriptorButCurrentPLayer();

                if(groupTarget->getStatus() == 0) {
                    request->addNext("ASYNC " + groupTarget->getAsyncCode(), fileDescriptor);

                    
                    for(int k = 0; k < fds.size(); k++) {
                        request->addNext("GMINF " + to_string(groupTarget->getAsyncCode()) + " " + to_string(groupTarget->getNbOfClient()-1), fds[k]);
                    }

                    if(groupTarget->getNbOfClient() == 0) {
                        removeGroup(groupTarget->getId());
                    }   
                }

                groupTarget->removeClient(client->getId());

                if(groupTarget->getStatus() == 1) {
                    for(int k = 0; k < fds.size(); k++) {
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

                return new ReturnRequest("VALID " + to_string(client->getLastRequestId()) + " " + to_string(group->getId()), fileDescriptor);
                    }
                break;
            case SRTGM: {
                Group* group = getGroupForRequest(requestID);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 416", fileDescriptor); // utilisateur n'est pas en jeux
                }

                if(!group->startGame()) {
                    return new ReturnRequest("ERROR 204", fileDescriptor); // Il n y a pas assez de joueurs en jeux pour commencer
                }

                vector<VirtualClient*> clients = group->getClients();

                request = new ReturnRequest();
                for(int k = 0; k < clients.size(); k++) {
                    string requestSNDSK = "SNDSK " + to_string(clients[k]->getLastRequestId()) + " " + group->sendPiles();
                    string requestSNDHD = "SNDHD " + to_string(clients[k]->getLastRequestId()) + " " + clients[k]->asRequest();


                    request->addNext("ASYNC " + group->getAsyncCode(), clients[k]->getFileDescriptor());
                    request->addNext("START " + to_string(clients[k]->getLastRequestId()) + " " + requestSNDSK + " " + requestSNDHD, clients[k]->getFileDescriptor());
                }

                return request;
            }
                break;
            case CTPLY: { // can't play -> end of the game 
                Group* group = getGroupForRequest(requestID);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 416", fileDescriptor); // utilisateur n'est pas en jeux
                }

                int nbCardsNotPlayed = group->endOfGame();
                vector<VirtualClient*> clients = group->getClients();

                request = new ReturnRequest();
                for(int k = 0; k < clients.size(); k++) {
                    request->addNext("ENDGM " + to_string(nbCardsNotPlayed) + " 0", clients[k]->getFileDescriptor());
                } 

                removeGroup(group->getId());

                return request;
            }
                break;
            case PLAY_: {
                Group* groupTarget = getGroupForRequest(requestID);
                if(groupTarget == nullptr) {
                    return new ReturnRequest("ERROR 416", fileDescriptor); // utilisateur n'est pas en jeux
                }

                if(options.size() < 3) {
                    return new ReturnRequest("ERROR 415", fileDescriptor); // certaine valeur d'option sont manquante
                }

                try{
                    int returnValue = groupTarget->play(requestID, stoi(options[1]), stoi(options[2]));

                    if(returnValue != 0) {
                        return new ReturnRequest("ERROR "+ to_string(returnValue), fileDescriptor);
                    }
                }
                catch(exception e){
                    return new ReturnRequest("ERROR 410", fileDescriptor); // mauvaise valeur d'option envoyé
                }


                string requestSNDHD = "SNDHD " + to_string(client->getLastRequestId()) + " " + groupTarget->sendHandCurrentPlayer();
                string requestSNDSK = "SNDSK " + to_string(client->getLastRequestId()) + " " + groupTarget->sendPiles();
                string requestCNCAT = "CNCAT " + requestSNDSK + " " + requestSNDHD;

                request = new ReturnRequest(requestCNCAT, groupTarget->getFileDescriptorCurrentPLayer());

                vector<VirtualClient*> clients = groupTarget->getClients();

                for(int k = 0; k < clients.size(); k++) {
                    if(clients[k]->getId() != client->getId())
                    requestSNDSK = "SNDSK " + to_string(clients[k]->getLastRequestId()) + " " + groupTarget->sendPiles();

                    request->addNext(requestSNDSK, clients[k]->getFileDescriptor());
                }

                return request;
            }
                break;
            case ENTRN: {
                Group* group = getGroupForRequest(requestID);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 416", fileDescriptor); // utilisateur n'est pas en jeux
                }

                group->endOfTurn();
                request = new ReturnRequest("ACKIT " + client->getLastRequestId(), client->getFileDescriptor());
                request->addNext("UTURN " + group->getClients()[group->getCurrentClient()]->getLastRequestId(), group->getClients()[group->getCurrentClient()]->getFileDescriptor());
                

                //send ENDRW if needed
                if(group->isStackEmpty()) {
                    vector<VirtualClient*> clients = group->getClients();

                    for(int k = 0; k < clients.size(); k++) {
                        request->addNext("ENDRW", clients[k]->getFileDescriptor());
                    }
                }

                return request;
            }
                break;
            case MESSG: {
                Group* group = getGroupForRequest(requestID);
                if(group == nullptr) {
                    return new ReturnRequest("ERROR 416", fileDescriptor); // utilisateur n'est pas en jeux
                }

                vector<VirtualClient*> clients = group->getClients();
                
                request = new ReturnRequest();
                for(int k = 0; k < clients.size(); k++) {
                    request->addNext(str, clients[k]->getFileDescriptor()); // send the message directly to all the clients
                }

                return request;
            }
                break;
            default:
                return new ReturnRequest("ERROR 401", fileDescriptor); // requête ne devrait pas être envoyer par le client
                break;
        }


        return new ReturnRequest();
    }
};
