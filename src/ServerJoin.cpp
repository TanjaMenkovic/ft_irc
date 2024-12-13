#include "../includes/Server.hpp"

namespace irc 
{


/*
- check if channel exists in the map and if not, create a new channel in map and put user to be operator 
- in users set add name of the channel

when user join the channel he is gettting:
>> :tmenkovi!~tmenkovi@194.136.126.52 JOIN #ahah123
>> :copper.libera.chat 353 tmenkovi @ #ahah123 :@tmenkovi
>> :copper.libera.chat 366 tmenkovi #ahah123 :End of /NAMES list.

when user is joining the channel that exists and has a topic:
>> :tmenkovi!~tmenkovi@freenode-l2g.s3h.4nuk5f.IP JOIN :#bla
>> :*.freenode.net 332 tmenkovi #bla :shiiiiit  <- if topic exists it add this line also
>> :*.freenode.net 333 tmenkovi #bla tanjaaa!~tmenkovi@freenode-l2g.s3h.4nuk5f.IP :1733757095 <- try without this line
>> :*.freenode.net 353 tmenkovi = #bla :tmenkovi @tanjaaa CHECKED
>> :*.freenode.net 366 tmenkovi #bla :End of /NAMES list. CHECKED

# define RPL_JOIN(nickname, username, channel_name) (":" + nickname + "!~" + username + "@ft_irc JOIN #" + channel_name)
// use getChannelNicks to get list_of_nicks
# define RPL_NAMREPLY(nickname, channel_name, list_of_nicks) (":ft_irc 353 " + nickname + " = #" + channel_name + " :" + list_of_nicks + "\r\n")
# define RPL_ENDOFNAMES(nickname, channel_name) (":ft_irc 366 " + nickname + " #" + channel_name + " :End of /NAMES list.\r\n")

everyone else is getting only:
>> :tmenkovi!~tmenkovi@194.136.126.52 JOIN #ahah123
*/

/*
getting all nicknames from users in channel and putting them into one string separated with spaces,
where there are first written regular users and then operators with '@' before their names
*/
std::string Server::getChannelNicks(std::string channel_name)
{
    std::string operators = "";
    std::string regUsers = "";
    std::string channelUsers = "";
    for (auto &[fd, user]: this->users) {
        if (user.isInChannel(channel_name)) {
            user.isOperator(channel_name) ? operators += "@" + user.getNickname() + " " : regUsers += user.getNickname() + " ";
        }
    }
    channelUsers += regUsers + operators;
    return (channelUsers);
}

void Server::addUser(int client_fd, std::string channel_name)
{
    std::map<std::string, irc::Channel>::iterator found_channel = channels.find(channel_name);
    std::string message;

    if (found_channel == channels.end()) { // channel doesn't exist
        irc::Channel new_channel = irc::Channel(channel_name);
        channels[channel_name] = new_channel;
        users[client_fd].joinChannel(channel_name, true);
        message = RPL_JOIN(users[client_fd].getNickname(), users[client_fd].getUsername(), channel_name);
        send_to_channel(channel_name, message);
        message = RPL_NAMREPLY(users[client_fd].getNickname(), channel_name, getChannelNicks(channel_name));
        message += RPL_ENDOFNAMES(users[client_fd].getNickname(), channel_name);
        send_to_user(client_fd, message);
    } else { // channel exists
        if (users[client_fd].isInChannel(channel_name) == false) { // and user is not in the channel
            if (channels[channel_name].getIsUserInvited(client_fd)) { // has user been invited
                channels[channel_name].removeInvitedUser(client_fd); // remove from invite list when user joins
            }
            users[client_fd].joinChannel(channel_name, false);
            message = RPL_JOIN(users[client_fd].getNickname(), users[client_fd].getUsername(), channel_name);
            send_to_channel(channel_name, message);
            message = "";
            if (channels[channel_name].getTopic() != "") {
                message = RPL_TOPIC(users[client_fd].getNickname(), channel_name, channels[channel_name].getTopic());
            }
            message += RPL_NAMREPLY(users[client_fd].getNickname(), channel_name, getChannelNicks(channel_name));
            message += RPL_ENDOFNAMES(users[client_fd].getNickname(), channel_name);
            send_to_user(client_fd, message);
        }
    }
            // if user is not in the channel add him
    
}

void Server::join(int client_fd, std::string channel_name, std::string channel_pass) {
    std::map<std::string, irc::Channel>::iterator found_channel = channels.find(channel_name);
    std::string message;

    if (found_channel != channels.end() && users[client_fd].isInChannel(channel_name) == false)
    {
        if (channels[channel_name].getPassword() != channel_pass) { // password is incorrect
            message = ERR_BADCHANNELKEY(users[client_fd].getNickname(), channel_name);
            send_to_user(client_fd, message);
            return ;
        }
        if (channels[channel_name].getInviteOnly() && !channels[channel_name].getIsUserInvited(client_fd)) { // channel is invite only
            message = ERR_INVITEONLYCHAN(users[client_fd].getNickname(), channel_name);
            send_to_user(client_fd, message);
            return ;
        }
        if (IsLimitReached(channel_name)) { // channel is full
            message = ERR_CHANNELISFULL(users[client_fd].getNickname(), channel_name);
            send_to_user(client_fd, message);
            return ;
        } 
    }
    if (channel_name[0] != '#') {
        channel_name = "#" + channel_name;
    }
    addUser(client_fd, channel_name);
}

}

/*
- when we join a channel it is not written which user joined it in betwwen []

- u shouldn't send User &user, send his fd and then find a user based on that, since all users are inside of Server class

- implament this from irc_replies,hpp: (and if needed some more)

// JOIN
# define RPL_JOIN(user_id, channel) (user_id + " JOIN :#" +  channel + "\r\n")
# define ERR_BANNEDFROMCHAN(client, channel) ("474 " + client + " #" + channel + " :Cannot join channel (+b)\r\n")
# define ERR_BADCHANNELKEY(client, channel) ("475 " + client + " #" + channel + " :Cannot join channel (+k)\r\n")

// PRIVMSG
# define ERR_NOSUCHNICK(client, target) ("401 " + client + " " + target + " :No such nick/channel\r\n")
# define ERR_NORECIPIENT(client) ("411 " + client + " :No recipient given PRIVMSG\r\n")
# define ERR_NOTEXTTOSEND(client) ("412 " + client + " :No text to send\r\n")
# define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + username + "@localhost PRIVMSG " + target + " " + message + "\r\n")

// TOPIC
# define RPL_TOPIC(client, channel, topic) (":localhost 332 " + client + " #" + channel + " " + topic + "\r\n")
# define RPL_NOTOPIC(client, channel) (":localhost 331 " + client + " #" + channel + " :No topic is set\r\n")

*/