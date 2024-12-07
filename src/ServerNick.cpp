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
    
    if (new_nickname.empty()) {
        message = ERR_NONICKNAMEGIVEN(users[client_fd].getNickname());
    } else if (invalid_characters(new_nickname)) {
        message = ERR_ERRONEUSNICKNAME(users[client_fd].getNickname(), new_nickname);
    } else if (isNicknameTaken(new_nickname)) {
        message = ERR_NICKNAMEINUSE(users[client_fd].getNickname(), new_nickname);
    } else {
        message = RPL_NICK(users[client_fd].getNickname(), users[client_fd].getUsername(), new_nickname);
        users[client_fd].setNickname(new_nickname);
    }
    send_to_joined_channels(client_fd, message);
}

}