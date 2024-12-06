#include "../includes/Server.hpp"

namespace irc 
{

void Server::change_nick(int client_fd, std::string new_nickname) {
    if (isNicknameTaken(new_nickname) == false)
        users[client_fd].setNickname(new_nickname);
    else
        std::cout << "Nickname already exists" << std::endl;
}

}