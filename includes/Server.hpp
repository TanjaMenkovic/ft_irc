#pragma once

#include <iostream>
#include <string>

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
};

}