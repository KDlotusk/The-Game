#include "socket.hpp"

#include <iostream>

using namespace std;

namespace theGame {
    ConnectionPoint::ConnectionPoint(int port, int backlog)
        : Socket(port), backlog(backlog) {}

    StreamSocket* ConnectionPoint::accept() {
        struct sockaddr_in from;
        socklen_t l = sizeof(from);
        int clientfd = ::accept(sockfd, (struct sockaddr*)&from, &l);

        return new StreamSocket(clientfd, from);
    }

    int ConnectionPoint::bind() {
        int optval = 1;
        if (::setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) != 0) {
            perror("[-] Error in setting options: ");
            close();
            return (errno);
        }

        if (::bind(sockfd, (struct sockaddr*)&address, sizeof(struct sockaddr)) != 0) {
            close();
            return errno;
        }

        return 0;
    }

    int ConnectionPoint::listen() {
        if (::listen(sockfd, backlog) != 0) {
            close();
            return errno;
        }

        return 0;
    }

    int ConnectionPoint::init() {
        // sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if (sockfd == -1) {
            return errno;
        }

        int err;
        if ((err = bind()) != 0) return err;

        if ((err = listen()) != 0) return err;

        return 0;
    }

    Socket::Socket(const std::string& IP, int port) {
        int optval = 1;
        sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            perror("[-] Error in Connexion: ");
            exit(errno);
        }
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

        // Configuration de l'adresse de transport
        address.sin_family = AF_INET;    // socket type
        address.sin_port = htons(port);  // port, converted in net style
        //    address.sin_addr.s_addr = INADDR_ANY; // any valid address in the domain
        // bzero(&(address.sin_zero), 8);          // zeroing the remaining of the
        // structure
        memset(&(address.sin_zero), 0, 8);  // zeroing the remaining of the structure

        // 127.0.0.1 Convert IPv4 and IPv6 addresses from text to binary form
        //    if(inet_aton(IP.c_str(), &address.sin_addr)<=0) //ipv4 only
        if (inet_pton(address.sin_family, IP.c_str(), &address.sin_addr) <=
            0)  // ipv4 or ipv6
        {
            perror("\nInvalid address/ Address not supported \n");
            exit(errno);
        }
    }

    Socket::Socket(int port) {
        int optval = 1;
        sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            perror("[-] Error in Connexion: ");
            exit(errno);
        }
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

        // Transport address Configuration
        address.sin_family = AF_INET;          // socket type
        address.sin_port = htons(port);        // port, converted in net style
        address.sin_addr.s_addr = INADDR_ANY;  // any valid address in the domain
        //    bzero(&(address.sin_zero), 8);        // zeroing the remaining of the
        //    structure
        memset(&(address.sin_zero), 0, 8);  // zeroing the remaining of the structure
    }

    Socket::Socket(int sockfd, const struct sockaddr_in& addr)
        : sockfd(sockfd), address(addr) {}

    StreamSocket::StreamSocket(int port) : Socket(port) {}

    StreamSocket::StreamSocket(const std::string& IP, int port) : Socket(IP, port) {}

    StreamSocket::StreamSocket(int sockfd, const struct sockaddr_in& addr)
        : Socket(sockfd, addr) {}

    Socket::~Socket() { close(); }

    int StreamSocket::connect() {
        ::connect(sockfd, (struct sockaddr*)&address, sizeof(address));
        return errno;
    }

    void Socket::close() {
        if (sockfd == -1) {
            return;
        }
        // call std::basic_fstream<CharT,Traits>::close
        ::close(sockfd);
    }

    void StreamSocket::setBlocking() {
        int opts = fcntl(sockfd, F_GETFL);
        opts = opts & (~O_NONBLOCK);
        fcntl(sockfd, F_SETFL, opts);
    }

    void StreamSocket::setNonBlocking() { fcntl(sockfd, F_SETFL, O_NONBLOCK); }

    int StreamSocket::read(std::string& msg) {
        int bytes_total = 0;
        char buffer[DEFAULT_SOCKET_BUFFER];

        int bytes_read = recv(sockfd, buffer, DEFAULT_SOCKET_BUFFER, 0);

        if (bytes_read <= 0) {
            return bytes_read;
        }

        msg.append(std::string(buffer, 0, bytes_read));
        bytes_total += bytes_read;

        // set non-blocking.
        setNonBlocking();

        while (bytes_read > 0) {
            memset(buffer, 0, DEFAULT_SOCKET_BUFFER);
            bytes_read = recv(sockfd, buffer, DEFAULT_SOCKET_BUFFER, 0);

            if (bytes_read < 0) {
                break;
            }

            msg.append(std::string(buffer, 0, bytes_read));
            bytes_total += bytes_read;
        }

        // set back to blocking
        setBlocking();

        return bytes_total;
    }

    int StreamSocket::read(char* buf, int len) { return ::recv(sockfd, buf, len, 0); }

    int StreamSocket::send(const std::string& msg) {
        return send(msg.c_str(), msg.length(), 0);
    }

    int StreamSocket::send(const char* buf, int len, int flags) {
        return ::send(sockfd, buf, len, flags);
    }
}  // namespace theGame
