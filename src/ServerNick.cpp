#include "../includes/Server.hpp"

namespace irc 
{

/*
Syntax: NICK <nickname>
- non-empty strings 
- MUST NOT contain any of the following characters: ' ', ',', '*', '?', '!', '@', '.'
- MUST NOT start with any of the following characters: '$', ':', '#'
*/

static bool	is_forbidden(char c)
{
	if (c == ' ' || c == ',' || c == '*' || c == '?' || c == '!'
		|| c == '@' || c == '.')
		return (true);
	else
		return (false);
}

static bool	invalid_characters(std::string nickname)
{
	if (nickname[0] == '$' || nickname[0] == ':' || nickname[0] == '#')
		return (true);
	
	for (size_t i = 0; i < nickname.size(); i++) {
			if (is_forbidden(nickname[i]) == true)
				return (true);
	}
	return (false);			
}

void Server::change_nick(int client_fd, std::string new_nickname) {
    std::string message;
    
    if (new_nickname == "") {
        message = ERR_NONICKNAMEGIVEN(users[client_fd].getNickname());
        send_to_user(client_fd, message);
    } else if (invalid_characters(new_nickname)) {
        message = ERR_ERRONEUSNICKNAME(users[client_fd].getNickname(), new_nickname);
        send_to_user(client_fd, message);
    } else if (isNicknameTaken(new_nickname)) {
        message = ERR_NICKNAMEINUSE(users[client_fd].getNickname(), new_nickname);
        send_to_user(client_fd, message);
    } else {
        message = RPL_NICK(users[client_fd].getNickname(), users[client_fd].getUsername(), new_nickname);
        send_to_joined_channels_not_fd(client_fd, message);
        send_to_user(client_fd, message);
        users[client_fd].setNickname(new_nickname);
    }
}

}