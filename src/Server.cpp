#include "../includes/Server.hpp"

namespace irc
{

Server::Server(): port(0), password("unknown") {}

Server::Server(int port, std::string password): port(port), password(password) {}

Server::~Server() {}

Server::Server(const Server &copy)
{
	*this = copy;
}

Server &Server::operator=(const Server &src)
{
	if (this != &src)
    {
        this->port = src.getPort();
        this->password = src.getPassword();
    }
    return *this;
}

int Server::getPort() const
{
    return this->port;
}

std::string Server::getPassword() const
{
    return this->password;
}

}
