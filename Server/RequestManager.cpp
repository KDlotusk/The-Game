#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "requestEnum.h"
#include "Group.h"

using namespace std;

class RequestManager {
private:

    vector<Group*> groups;

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
        {"MESSG", MESSG }
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
    }

    void createGroup() {
        groups.push_back(new Group(groups.size()));
    }
    void removeGroup(long id) {
        for(int k = 0; k < groups.size(); k++) {
            if(groups[k]->getId() == id) {
                delete groups[k];
                groups.erase(groups.begin() + k);
            }
        }
    }

    Group* getGroupForRequest(long requestId) {
        for(int k = 0; k < groups.size(); k++) {
            if(groups[k]->isRequestFromThisGroup(requestId)) {
                return groups[k];
            }
        }

        return nullptr;
    }

    string request(string str) {

        int requestName = DEFAULT;

        if(requests.find(str.substr(0, 5)) != requests.end())
            requestName = requests[str.substr(0, 5)];
        
        string opt = str.substr(5);
        vector<string> options;
        fillOptions(opt, options);
        if(options.size() == 0) {
            return "ERROR 411"; //aucune option n'a été reçu
        }


        int requestID; 
        
        try {
            requestID = stoi(options[0]);
        }
        catch(exception e) {
            return "ERROR 403"; // l'id de la requête n'est pas valable
        }
        

        Group* groupTarget = getGroupForRequest(requestID);

        if(groupTarget == nullptr) {
            return "ERROR 412"; // utilisateur non reconnu
        }


        switch(requestName) {
            case CONEC: 
                return "CONEC";
                break;
            case ACKIT: 
                return "ACKIT";
                break;
            case ERROR: 
                return "ERROR";
                break;
            case VALID: 
                return "VALID";
                break;
            case ALIVE: 
                return "ALIVE";
                break;
            case SEEGM: 
                return "SEEGM";
                break;
            case JOING: 
                return "JOING";
                break;
            case LEAVE: 
                return "LEAVE";
                break;
            case MAKEG: 
                return "MAKEG";
                break;
            case SNDGM: 
                return "SNDGM";
                break;
            case GMINF: 
                return "GMINF";
                break;
            case ASYNC: 
                return "ASYNC";
                break;
            case START: 
                return "START";
                break;
            case ENDRW: 
                return "ENDRW";
                break;
            case UTURN: 
                return "UTURN";
                break;
            case PLAY_: 
            // play(long requestId, int pile, int cardId)

                if(options.size() < 3) {
                    return "ERROR 415"; // certaine valeur d'option sont manquante
                }

                try{
                    groupTarget->play(requestID, stoi(options[1]), stoi(options[2]));
                }
                catch(exception e){
                    return "ERROR 410"; // mauvaise valeur d'option envoyé
                }

                break;
            case SNDHD: 
                return "SNDHD";
                break;
            case SNDSK: 
                return "SNDSK";
                break;
            case MESSG: 
                return "MESSG";
                break;
            default:
                return "ERROR 400" ;
                break;
        }
    }
};
