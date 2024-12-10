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



    void Server::kick_user() {

    }

    void Server::kick(int client_fd, std::vector<std::string> tokens) {
        
        std::string message;
        size_t tokens_amount = tokens.size();
        auto it = users.begin();
        for (it; it != users.end(); it++) { // loop through users to find user we want to kick from nickname string
            if (it->second.getNickname() == kick_nickname) {
                if (!it->second.isInChannel(channel_name)) {
                    it = users.end();
                }
            }
        }
        if (it == users.end()) { // if it == users.end, means we didn't find user or user is not in channel
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
        kick_user();
}