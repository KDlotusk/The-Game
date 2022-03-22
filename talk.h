#ifndef TALK_H
#define TALK_H

class Talk {
private:
    int readfd;
    int writefd;

public:
    int getReader() const { return readfd; }
    int getWriter() const { return writefd; }
    void setReader(int sockfd) {
        readfd = sockfd;
    }
    void setWriter(int sockfd) {
        writefd = sockfd;
    }
};

void talk(Talk& param);

#endif // TALK_H
