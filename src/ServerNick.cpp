#include "../includes/Server.hpp"

namespace irc 
{

/**
 * 	Syntax: NICK <nickname>
 * 
 * 	Nicknames are non-empty strings with the following restrictions:
 * 	
 * 	They MUST NOT contain any of the following characters: 
 * 	- space (' '), 
 * 	- comma (','), 
 * 	- asterisk ('*'), 
 * 	- question mark ('?'), 
 * 	- exclamation mark ('!'), 
 * 	- at sign ('@'),
 * 	- dot ('.').
 * 	
 * 	They MUST NOT start with any of the following characters: 
 * 	dollar ('$'), colon (':'), diese (#).
 * 	
 * 	Numeric Replies:
 * // // NICK
// # define ERR_NONICKNAMEGIVEN(client) (":localhost 431 " + client + " :There is no nickname.\r\n")
// # define ERR_ERRONEUSNICKNAME(client, nickname) (":localhost 432 " + client + " " + nickname + " :Erroneus nickname\r\n")
// # define ERR_NICKNAMEINUSE(client, nickname) (":localhost 433 " + client + " " + nickname + " :Nickname is already in use.\r\n")
// # define RPL_NICK(oclient, uclient, client) (":" + oclient + "!" + uclient + "@localhost NICK " +  client + "\r\n")
 * 
 * 	ERR_NONICKNAMEGIVEN (431)
 * 	ERR_ERRONEUSNICKNAME (432)
 * 	ERR_NICKNAMEINUSE (433)
 * 	ERR_NICKCOLLISION (436)
 * 
 * 	Example:
 * 	[CLIENT] /Nick mike
 * 
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