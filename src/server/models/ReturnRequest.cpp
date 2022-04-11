#include "ReturnRequest.hpp"

using namespace std;

namespace theGame {
    ReturnRequest::ReturnRequest() {};
    ReturnRequest::ReturnRequest(std::string request, int fileDescriptor) {
        request += "\n";
        addNext(request, fileDescriptor);
    }

    int ReturnRequest::getSize() const { return _requests.size(); }
    bool ReturnRequest::isEmpty() const { return _requests.empty(); }

    bool ReturnRequest::hasNext() const { return !isEmpty(); }

    void ReturnRequest::addNext(std::string __request, int __fileDescriptor) {
        __request += "\n";
        _requests.push_back(std::make_pair(__request, __fileDescriptor));
    }

    std::pair<std::string, int> ReturnRequest::readNext() {
        if(!isEmpty()) {
            std::pair<std::string, int> next = _requests[0];
            _requests.erase(_requests.begin());

            return next;
        }
        
        return std::make_pair("", -1);
    }
}