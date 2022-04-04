#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include "talk.h"
#include <vector>

std::vector<int> file_descriptors;

void talk(Talk& param) {
    char msg;
    int err;
    bool has_err = false;

    file_descriptors.push_back(param.getReader());

    std::cout << param.getReader() << " - " << param.getWriter() << std::endl;

    do {
        err = read(param.getReader(), &msg, 1);
        if (err <= 0) break;

        if (param.getWriter() == -1) {
            for (auto writefd : file_descriptors)
                if (writefd != param.getReader()) {
                    err = write(writefd, &msg, 1);
                    if (err <= 0) {
                        has_err = true;
                        break;
                    }
                }
        }
        else {
            err = write(param.getWriter(), &msg, 1);
            if (err <= 0) {
                has_err = true;
            }
        }

    } while (true);

    for (std::size_t i = 0; i < file_descriptors.size(); ++i)
        if (file_descriptors[i] == param.getReader())
            file_descriptors.erase(file_descriptors.begin() + i);

    return;
}
