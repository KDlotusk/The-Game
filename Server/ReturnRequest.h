#include <iostream>
#include <vector>
#include <utility>
#include <string>

using namespace std;


class ReturnRequest {
private:
    vector<pair<string, int>> requests;


public:
    ReturnRequest() {};

    ReturnRequest(string request, int fileDescriptor) {
        addNext(request, fileDescriptor);
    }

    ReturnRequest( vector<pair<string, int>> _requests) {
        requests = _requests;
    }

    void addNext(string request, int fileDescriptor) {
        requests.push_back(make_pair(request, fileDescriptor));
    }

    bool isEmpty() { return requests.empty(); }

    bool hasNext() { return isEmpty(); }

    pair<string, int> readNext() {
        if(!isEmpty()) {
            pair<string, int> next = requests[0];
            requests.erase(requests.begin());

            return next;
        }
        
        return make_pair("", -1);
    }

    int getSize() { return requests.size(); }

};