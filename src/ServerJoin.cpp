#include "../includes/Server.hpp"

namespace irc 
{

void Server::join(User &user, const std::string &channel_name, std::map<std::string, irc::Channel> &channels) {
    std::cout << "In server join method!!! \n";
    std::map<std::string, irc::Channel>::iterator found_channel = channels.find(channel_name);
    // check whether channel exists
    if (found_channel == channels.end()) {
        // channel doesn't exist, so we create a new one
        irc::Channel new_channel = irc::Channel(channel_name);
        std::cout << new_channel.getName() << std::endl;
        new_channel.addUser(user, true);
        this->channels.insert({channel_name, new_channel});
        std::cout << "channel nicks: " << new_channel.getUsers().begin()->first << std::endl;
    } else {
        // channel exists
        found_channel->second.addUser(user, false);
    }
    // add to user joined channels
    user.joinChannel(channel_name);
    std::string message = "JOIN " + channel_name + "\r\n";
    send(user.getFd(), message.c_str(), message.length(), 0);
    irc::Channel& joined_channel = this->channels.find(channel_name)->second;
    if (joined_channel.getTopic() != "") {
        message = "332 " + joined_channel.getTopic() + "\r\n";
    }
    message = "353 " + joined_channel.getChannelNicks() + "\r\n";
    send(user.getFd(), message.c_str(), message.length(), 0);
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