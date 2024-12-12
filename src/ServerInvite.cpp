#include "../includes/Server.hpp"

namespace irc 
{
    /* User invited and then joins
        >> :maslina_!~oliverher@ft_irc INVITE maslina :#chanchan2
        << JOIN #chanchan2
        >> :maslina!~oliverher@ft_irc JOIN #chanchan2 * :Oliver Hertzberg
    */
   /* maslina__ not operator, maslina_ changes mode
        >> :maslina_!~oliverher@ft_irc MODE #chanchan2 +i 
        << INVITE maslina #chanchan2
        >> :ft_irc 482 maslina__ #chanchan2 :You're not a channel operator

    */
   /* Operator inviting maslina
    << INVITE maslina #chanchan2
    >> :ft_irc 341 maslina_ maslina #chanchan2
    >> :maslina!~oliverher@ft_irc JOIN #chanchan2 * :Oliver Hertzberg
   */
  // tokens =  <nickname invited> <channel_name> (and maybe some extra shit we ignore)

    void Server::invite_user(const std::string &invited_user, const std::string& channel_name, int client_fd) {
        std::string message;
        int         invited_fd;

        for (auto &[fd, user]: users) {
            if (user.getNickname() == invited_user) {
                invited_fd = user.getFd();
                break ;
            }
        }
        message = RPL_INVITING(users[client_fd].getNickname(), invited_user, channel_name);
        send_to_user(client_fd, message);
        message = RPL_INVITE(users[client_fd].getNickname(), users[client_fd].getUsername(), invited_user, channel_name);
        send_to_user(invited_fd, message);
        if (channels[channel_name].getInviteOnly()) { // channel invite only so we add invited user to channel invite list
            for (auto &[fd, user]: users) { // find invited user fd from users
                if (user.getNickname() == invited_user) { // add invited users fd to invited users in channel
                    channels[channel_name].addInvitedUser(fd);
                }
            }
        }
    }
    void Server::invite(int client_fd, std::vector<std::string> tokens) {
        std::string invited_user = tokens[0];
        std::string channel_name = tokens[1];
        std::string error_message = "";

        bool found = false; // check that channel exists
        for (auto &[name, channel]: channels) {
            if (name == channel_name) {
                found = true;
                break;
            }
        }
        if (!found) { // channel doesn't exist
            error_message = ERR_NOSUCHCHANNEL(users[client_fd].getNickname(), channel_name);
            send_to_user(client_fd, error_message);
            return ;
        }
        if (!users[client_fd].isInChannel(channel_name)) { // user is not in channel
            error_message = ERR_NOTONCHANNEL(users[client_fd].getNickname(), channel_name);
            send_to_user(client_fd, error_message);
            return ;
        }
        found = false;
        for (auto &[fd, user]: users) { // check that invited user exists on server and that invited user is not already in channel
            if (user.getNickname() == invited_user) {
                found = true;
                if (user.isInChannel(channel_name)) { // user already in channel
                    error_message = ERR_USERONCHANNEL(user.getUsername(), user.getNickname(), channel_name);
                    send_to_user(client_fd, error_message);
                    return ;
                }
            }
        }
        if (!found) { // we didn't find invited user in server users
            error_message = ERR_NOSUCHNICK(users[client_fd].getNickname(), invited_user);
            send_to_user(client_fd, error_message);
            return ;
        }
        if (!users[client_fd].isOperator(channel_name)) { // check user is operator
            error_message = ERR_CHANOPRIVSNEEDED(users[client_fd].getNickname(), channel_name);
            send_to_user(client_fd, error_message);
            return ;
        }
        invite_user(invited_user, channel_name, client_fd);
    }
}