#include "../includes/Server.hpp"

namespace irc 
{
    // receiving privatemessage:
    // >> :ohertzbe_!~ohertzbe@194.136.126.51 PRIVMSG ohertzbe :hello idiot
    // sending privmessages:
    // << PRIVMSG #chan21 :testing haahhahaahahahah
    // << PRIVMSG ohertzbe :hello idiot
    // error no such nick/channel
    //>> :zirconium.libera.chat 401 ohertzbe asdsad :No such nick/channel


    //# define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + "~" + username + "@ft_irc PRIVMSG " + target + " " + message + "\r\n")

    void    Server::send_privmsg(int client_fd, const std::string& receiver, const std::string& privmessage) {
        std::string message;
        message = RPL_PRIVMSG(users[client_fd].getNickname(), users[client_fd].getUsername(), receiver, privmessage);
        // if we are sending to a channel, we need to send the message to everyone in the channel
        if (receiver[0] == '#') {
            send_to_channel_not_fd(client_fd, receiver, message);
        } else { // if it is a message to one user only we use send_to_user
            for (auto &[fd, user]: users) {
                if (user.getNickname() == receiver) {
                    send_to_user(user.getFd(), message);
                    return ;
                }
            }
        }
    }


    void    Server::privmsg(int client_fd, std::vector<std::string> tokens){
        std::string message = "";
        // need to check that either #channel or user exists
        if (tokens[0].at(0) == '#') {
            if (!users[client_fd].isInChannel(tokens[0])) {
                message = ERR_NOSUCHNICK(users[client_fd].getNickname(), tokens[0]);
                send_to_user(client_fd, message);
                return ;
            }
        } else {
            // check if user is found on server
            bool found = false;
            for (auto &[fd, user]: users) {
                if (user.getNickname() == tokens[0]) {
                    found = true;
                    break ;
                }
            }
            if (!found) {
                message = ERR_NOSUCHNICK(users[client_fd].getNickname(), tokens[0]);
                send_to_user(client_fd, message);
                return ;
            }
        }
        // add tokens to one uniform string
        std::vector<std::string>::iterator it = tokens.begin() + 1;
        while (it != tokens.end()) {
            message += *it + " ";
            it++;
        }
        // no errors, so we can send message
        send_privmsg(client_fd, tokens[0], message);
    }
}