#pragma once

#include <string>
#include <vector>

using namespace std;

class RequestManager {

    vector<string> options;
 
    static void fillOptions(string opt) {
        
    }

public:
    static string request(string str) {
        string requestName = str.substr(0, 5);
        string opt = str.substr(5);
        fillOptions(opt);
    }
};