#pragma once
#ifndef THEGAME_SERVER_REQUEST_HPP
#define THEGAME_SERVER_REQUEST_HPP

#include <map>
#include <string>
#include <vector>

#include "clients/Group.hpp"

namespace theGame {
    enum request {
        CONEC,
        ACKIT,
        ERROR,
        VALID,
        ALIVE,
        SEEGM,
        JOING,
        LEAVE,
        MAKEG,
        SNDGM,
        GMINF,
        ASYNC,
        START,
        ENDGM,
        ENDRW,
        UTURN,
        PLAY_,
        SNDHD,
        SNDSK,
        MESSG,

        DEFAULT
    };

    class RequestManager {
    private:
        std::vector<theGame::Group*> _groups;

        std::map<std::string, int> _requests{
            { "CONEC", CONEC }, { "ACKIT", ACKIT }, { "ERROR", ERROR },
            { "VALID", VALID }, { "ALIVE", ALIVE }, { "SEEGM", SEEGM },
            { "JOING", JOING }, { "LEAVE", LEAVE }, { "MAKEG", MAKEG },
            { "SNDGM", SNDGM }, { "GMINF", GMINF }, { "ASYNC", ASYNC },
            { "START", START }, { "ENDRW", ENDRW }, { "UTURN", UTURN },
            { "PLAY_", PLAY_ }, { "SNDHD", SNDHD }, { "SNDSK", SNDSK },
            { "MESSG", MESSG }
        };

        void _fillOptions(std::string opt, std::vector<std::string>& options) const;

    public:
        ~RequestManager();

        void createGroup();
        void removeGroup(const long& id);

        const theGame::Group* getGroupForRequest(const long& requestId) const;
        const std::string& request(const std::string& str);
    };
}  // namespace theGame

#endif  // THEGAME_SERVER_REQUEST_HPP