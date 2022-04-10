#include "request.hpp"

#include <iostream>

using namespace std;

namespace theGame {
    void RequestManager::_fillOptions(string __opt, vector<string>& __options) const {
        string delimiter = " ";

        size_t pos = 0;
        string token;

        while ((pos = __opt.find(delimiter)) != string::npos) {
            token = __opt.substr(0, pos);
            __options.push_back(token);
            __opt.erase(0, pos + delimiter.length());
        }
        __options.push_back(__opt);
    }

    RequestManager::~RequestManager() {
        for (size_t k = 0; k < _groups.size(); k++) {
            delete _groups[k];
        }
    }

    void RequestManager::createGroup() { _groups.push_back(new Group(_groups.size())); }
    void RequestManager::removeGroup(const long& __id) {
        for (size_t k = 0; k < _groups.size(); k++) {
            if (_groups[k]->getId() == __id) {
                delete _groups[k];
                _groups.erase(_groups.begin() + k);
            }
        }
    }

    const Group* RequestManager::getGroupForRequest(const long& __requestId) const {
        for (size_t k = 0; k < _groups.size(); k++) {
            if (_groups[k]->isRequestFromThisGroup(__requestId)) {
                return _groups[k];
            }
        }

        return nullptr;
    }

    const string& RequestManager::request(const string& __str) {
        int requestName = DEFAULT;

        if (_requests.find(__str.substr(0, 5)) != _requests.end())
            requestName = _requests[__str.substr(0, 5)];

        auto opt = __str.substr(5);
        vector<string> options;
        _fillOptions(opt, options);
        if (options.size() == 0) {
            return "ERROR 411";  // aucune requête n'a été reçus
        }

        switch (requestName) {
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
                return "ERROR 400";
                break;
        }
    }
}  // namespace theGame
