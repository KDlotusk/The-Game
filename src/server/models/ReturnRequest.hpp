#pragma once
#ifndef THEGAME_SERVER_RETURNREQUEST_HPP
#define THEGAME_SERVER_RETURNREQUEST_HPP

#include <iostream>
#include <vector>
#include <utility>
#include <string>

namespace theGame {

    class ReturnRequest {
    private:
        std::vector<std::pair<std::string, int>> _requests;


    public:
        ReturnRequest();
        ReturnRequest(std::string request, int fileDescriptor);

        int getSize() const;
        bool isEmpty() const;

        bool hasNext() const;

        void addNext(std::string request, int fileDescriptor);

        std::pair<std::string, int> readNext();
    };
} // namespace theGame 

#endif  // THEGAME_SERVER_RETURNREQUEST_HPP