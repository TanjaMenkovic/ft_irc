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