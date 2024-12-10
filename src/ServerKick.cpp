#include "../includes/Server.hpp"

namespace irc 
{
    /*
    Kicked user
    << KICK #hah123 ohertzbe :
    >> :tantalum.libera.chat 482 ohertzbe_ #hah123 :You're not a channel operator
    # define ERR_CHANOPRIVSNEEDED(nickname, channel_name) (":ft_irc 482 " + nickname + " #" +  channel_name + " :You're not channel operator\r\n")
    >> :ohertzbe!~ohertzbe@194.136.126.53 KICK #hah123 ohertzbe_ :ohertzbe_

    Kicker
    << KICK #hah123 ohertzbe_ :
    >> :ohertzbe!~ohertzbe@194.136.126.53 KICK #hah123 ohertzbe_ :ohertzbe_
    */

   // no suck nick/ channel
   // << KICK #xd12 haha :
// >> :iridium.libera.chat 401 ohertzbe haha :No such nick/channel
// << PING iridium.libera.chat
// >> :iridium.libera.chat PONG iridium.libera.chat :iridium.libera.chat
// --> lag pong
// << KICK #xd12 ad :
// >> :iridium.libera.chat 401 ohertzbe ad :No such nick/channel
// # define ERR_NOSUCHNICK(nickname, target) (":ft_irc 401 " + nickname + " " + target + " :No such nick/channel\r\n")



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
        std::cout << "kicknick = " << kick_nickname << std::endl;
        std::cout << "channelname = " << channel_name << std::endl;

        if (tokens.size() > 2) { // if we have more tokens than user we want to kick and channel, we add those to reason
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
                    found = true;
                    break ;
                }
            }
        }
        std::cout << "found = " << found << std::endl;
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