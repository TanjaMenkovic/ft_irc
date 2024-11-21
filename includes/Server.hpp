#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <signal.h>
#include <vector>
#include <poll.h>

namespace irc
{

class Server
{
    private:
        int port;
        std::string password;

    public:
        ~Server();
        Server();
        Server(int port, std::string password);
        Server(const Server &copy);
        Server &operator=(const Server &src);

        int getPort() const;
        std::string getPassword() const;

        int setup_server();
        bool validateClientPassword(const std::string &clientPassword) const;
};

}