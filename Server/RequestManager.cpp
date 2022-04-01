#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "requestEnum.h"

using namespace std;

vector<string> options;
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



void fillOptions(string opt) {
    

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

string request(string str) {

    int requestName = DEFAULT;

    if(requests.find(str.substr(0, 5)) != requests.end())
        requestName = requests[str.substr(0, 5)];
    
    string opt = str.substr(5);
    fillOptions(opt);


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
            return "PLAY_";
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
