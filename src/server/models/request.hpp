#pragma once
#ifndef THEGAME_SERVER_REQUEST_HPP
#define THEGAME_SERVER_REQUEST_HPP

#include <map>
#include <string>
#include <vector>

#include "clients/Group.hpp"
#include "ReturnRequest.hpp"

namespace theGame {
    enum request {
        ACKIT, ALIVE, ASYNC, CNCAT, 
        CONEC, CTPLY, ENDGM, ENDRW, 
        ERROR, ENTRN, GMINF, JOING, 
        LEAVE, MAKEG, MESSG, PLAY_, 
        SEEGM, SNDGM, SNDHD, SNDSK, 
        SRTGM, START, UTURN, VALID, 

        DEFAULT
    };

    class RequestManager {
    private:
        vector<string> _options;
        std::vector<theGame::Group*> _groups;
        vector<VirtualClient*> _clients;

        std::map<std::string, int> _requests{
            { "ACKIT", ACKIT }, { "ALIVE", ALIVE }, 
            { "ASYNC", ASYNC }, { "CNCAT", CNCAT },
            { "CONEC", CONEC }, { "CTPLY", CTPLY }, 
            { "ENDGM", ENDGM }, { "ENDRW", ENDRW },
            { "ERROR", ERROR }, { "ENTRN", ENTRN }, 
            { "GMINF", GMINF }, { "JOING", JOING },
            { "LEAVE", LEAVE }, { "MAKEG", MAKEG }, 
            { "MESSG", MESSG }, { "PLAY_", PLAY_ },
            { "SEEGM", SEEGM }, { "SNDGM", SNDGM }, 
            { "SNDHD", SNDHD }, { "SNDSK", SNDSK },
            { "SRTGM", SRTGM }, { "START", START }, 
            { "UTURN", UTURN }, { "VALID", VALID },
        };

        void _fillOptions(std::string opt);

    public:
        ~RequestManager();

        theGame::Group* createGroup();
        void removeGroup(const long& id);
        theGame::Group* findGroupById(const long& id) const;
        theGame::Group* findGroupByRequest(const long& requestId) const;

        const std::string& seeGroups() const;

        theGame::VirtualClient* createClient(const int& fileDescriptor);
        void removeClient(const long& id);
        theGame::VirtualClient* findClientByRequest(const long& requestId) const;


        theGame::ReturnRequest* request(const std::string& str, const int& fileDescriptor);
    };
}  // namespace theGame

#endif  // THEGAME_SERVER_REQUEST_HPP