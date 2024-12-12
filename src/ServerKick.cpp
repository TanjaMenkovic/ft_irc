#include "../includes/Server.hpp"

namespace irc 
{
    void Server::kick_user(const std::string& channel_name, const std::string& reason, const std::string& user_to_kick) {
        std::string message =  RPL_KICK(channel_name, user_to_kick, reason);
        send_to_channel(channel_name, message);
         // remove channel from the kicked users joined_channels
        for (auto &[id, user]: users) {
            if (user.getNickname() == user_to_kick) {
                user.leaveChannel(channel_name);
                break ;
            }
        }
    }

    void Server::kick(int client_fd, std::vector<std::string> tokens) {
        
        std::string message;
        std::string kick_nickname;
        std::string channel_name;
        std::string reason = " ";

        channel_name = tokens[0];
        kick_nickname = tokens[1];
        if (tokens.size() > 3) { // if we have more tokens than user we want to kick and channel, we add those to reason
            std::vector<std::string>::iterator it = tokens.begin() + 2; // it points to first reason string
            while (it != tokens.end()) { // add all reason strings to reason separated by space
                reason += *it + " ";
                it++;
            }
        }
        bool found = false;
        for (auto it = users.begin(); it != users.end(); it++) { // loop through users to find user we want to kick from nickname string
            if (it->second.getNickname() == kick_nickname) {
                if (it->second.isInChannel(channel_name)) {
                    if (reason == " ") {
                        reason = kick_nickname;
                    }
                    found = true;
                    break ;
                }
            }
        }
        if (!found) { // if it == users.end, means we didn't find user or user is not in channel
            message = ERR_NOSUCHNICK(users[client_fd].getNickname(), kick_nickname);
            send_to_user(client_fd, message);
            return ;
        }
        // check that kicker is operator
        if (!users[client_fd].isOperator(channel_name)) {
            message = ERR_CHANOPRIVSNEEDED(users[client_fd].getNickname() ,channel_name);
            send_to_user(client_fd, message);
            return ;
        }
        // No errors, proceed to kick user
        kick_user(channel_name, reason, kick_nickname);
    }
}