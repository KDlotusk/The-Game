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
        request += "\n";
        addNext(request, fileDescriptor);
    }

    void addNext(string request, int fileDescriptor) {
        request += "\n";
        requests.push_back(make_pair(request, fileDescriptor));
    }

    bool isEmpty() { return requests.empty(); }

    bool hasNext() { return !isEmpty(); }

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