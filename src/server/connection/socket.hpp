/**
 * @file socket.hpp
 * @author FJa
 * @version 1.0
 * @date 2020-02-03
 * @details freely inspired from @link https://github.com/davidsteinsland/cpp-sockets
 * @endlink see also forked project @link
 * https://github.com/Victini378/Cross-Platform_CppSockets @endlink for a updated
 * portable version.
 */
#pragma once

#ifndef THEGAME_SERVER_SOCKET_HPP
#define THEGAME_SERVER_SOCKET_HPP

#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include <string>

#define DEFAULT_SOCKET_BUFFER 128

/**
 * @namespace theGame
 * used to define a generic Socket API
 * in POSIX style
 */
namespace theGame {
    /**
     * @brief Generic Socket class
     * that is specialized for Connection and Transport
     */
    class Socket {
    protected:
        int sockfd;                  //!< file descriptor for socket
        struct sockaddr_in address;  //!< POSIX address struct
    public:
        /**
         * @brief Socket, construct a socket on
         * the specified port. Any address is valid
         * Used mainly for localhost communication.
         * @param port
         */
        Socket(int port);
        /**
         * @brief Socket, construct a socket on
         * the specified port and address.
         * @param dotAddr, in dot format (ie. 127.0.0.1), or name (localhost)
         * @param port
         */
        Socket(const std::string& dotAddr, int port);
        /**
         * @brief Socket construct a socket based on an existing
         * socket file descriptor and sockaddr_in structure.
         * Used by server when accepting a connexion
         * @param sockfd, socket file descriptor
         * @param addr, address structure
         */
        Socket(int sockfd, const struct sockaddr_in& addr);
        /**
         * @brief ~Socket, destructor
         * Close the socket file descriptor
         */
        virtual ~Socket();

        /**
         * @brief getSockfd
         * @return sockfd, the file descriptor
         */
        virtual int getSockfd() const { return sockfd; }
        /**
         * @brief getAdress
         * @return the POSIX sockaddr_in struct
         */
        virtual const struct sockaddr_in& getAdress() const { return address; }
        /**
         * @brief getPort, extract port from sockaddr_in struct
         * @return port
         */
        virtual int getPort() const { return ntohs(address.sin_port); }
        /**
         * @brief getPort, extract IP from sockaddr_in struct
         * @return dotted address (IP)
         */
        virtual std::string getIP() const {
            //        return inet_ntoa(address.sin_addr); // ipv4 only
            char ip[INET_ADDRSTRLEN];
            inet_ntop(address.sin_family, &(address.sin_addr), ip,
                      INET_ADDRSTRLEN);  // ipv4 or ipv6
            return std::string(ip);
        }
        /**
         * @brief valid, checks if the socked is valid
         * ie. with a valid file descriptor
         * @return true if valid, false otherwise
         */
        virtual bool valid() const { return sockfd != -1; }
        /**
         * @brief close the socket, by closing the file descriptor if opened
         */
        virtual void close();
    };

    /**
     * Specialized Socket class
     * used for transporting information
     * both on Server and Client Side,
     * after a connexion has been established
     */
    class StreamSocket : public Socket {
    private:
    public:
        /**
         * @brief Socket, construct a socket on
         * the specified port. Any address is valid
         * Used mainly for localhost communication.
         * @param port
         */
        StreamSocket(int port);
        /**
         * @brief Socket, construct a socket on
         * the specified port and address.
         * @param dotAddr, in dot format (ie. 127.0.0.1), or name (localhost)
         * @param port
         */
        StreamSocket(const std::string& dotAddr, int port);
        /**
         * @brief Socket construct a socket based on an existing
         * socket file descriptor and sockaddr_in structure.
         * Used by server when accepting a connexion
         * @param sockfd, socket file descriptor
         * @param addr, address structure
         */
        StreamSocket(int sockfd, const struct sockaddr_in& addr);

        /**
         * @brief connect, Open a connection on the socket file descriptor
         * to peer at address.
         * @return 0 on success, -1 for errors
         * @remark as only used on client side, maybe woth to separate in a specialized
         * class
         */
        int connect();

        /**
         * @brief setBlocking, set the socket in blocking mode
         */
        void setBlocking();
        /**
         * @brief setNonBlocking, set the socket in non blocking mode
         */
        void setNonBlocking();

        /**
         * @brief read all data. Will block until there is data to read.
         * For long data (>DEFAULT_SOCKET_BUFFER), will read the rest of it and append
         * it to msg
         * @return total msg size, or -1
         */
        int read(std::string& msg);
        /**
         * @brief read a specified amount of data in a char pointer
         * @param buf, specified buffer
         * @param len, length of buffer (typically DEFAULT_SOCKET_BUFFER)
         * @return amount of bytes actually read, or -1
         */
        int read(char* buf, int len);
        /**
         * @brief send a msg in a char string
         * @param msg, the string to be sent
         * @return amount of bytes actually sent, or -1
         */
        int send(const std::string& msg);
        /**
         * @brief send a specified amount of data in a char pointer
         * @param buf, specified buffer
         * @param len, length of buffer
         * @param flags, 0 or see Linux man page for send (2)
         * @return amount of bytes actually sent, or -1
         */
        int send(const char* buf, int len, int flags = 0);
    };

    /**
     * Specialized Socket class
     * used for connexions on Server Side
     */
    class ConnectionPoint : public Socket {
    private:
        int backlog;

    public:
        /**
         * @brief ServerSocket, constructs a listening socket on the specified port.
         * The default address is 0.0.0.0 (INADDR_ANY)
         * @param port, default is 3490 (for personnal historical reasons ;o)
         * @param backlog, default 10
         */
        ConnectionPoint(int port = 3490, int backlog = 10);
        /**
         * @brief accept a new incomming client (blocking call).
         * @return a client socket instance
         */
        StreamSocket* accept();
        /**
         * @brief init, binds the file descriptor to the current port and address
         * and prepare to listen to client connexions
         * @remark calls bind()+listen()
         * @return error code if there was a problem
         */
        int init();
        /**
         * @brief bind, binds the file descriptor to the current port and address
         * @return error code if there was a problem
         */
        int bind();
        /**
         * @brief listen, prepare to listen to client connexions
         * @return error code if there was a problem
         */
        int listen();
    };
}  // namespace theGame
#endif  // THEGAME_SERVER_SOCKET_HPP
