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

        const int& getSize() const;
        const bool& isEmpty() const;

        const bool& hasNext() const;

        void addNext(std::string request, int fileDescriptor);

        std::pair<std::string, int> readNext();
    };
}