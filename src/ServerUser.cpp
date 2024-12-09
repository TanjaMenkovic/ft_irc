#include "../includes/Server.hpp"

namespace irc 
{

/*
funcitons  for adding user in the channel, removing user from the channel
adding operators and removing operators, checking is it operator or not

*/

/*
- check if channel exists in the map and if not, create a new channel in map and put user to be operator 
- in users set add name of the channel
*/
bool Server::addUser(int client_fd, std::string channel_name)
{
    std::map<std::string, irc::Channel>::iterator found_channel = channels.find(channel_name);

    if (found_channel == channels.end()) {
        irc::Channel new_channel = irc::Channel(channel_name);
        new_channel.addUser(user, true);
        this->channels.insert({channel_name, new_channel});
        // std::cout << "channel nicks: " << new_channel.getUsers().begin()->first << std::endl;
    } else {
        // channel exists
        found_channel->second.addUser(user, false);
    }



    if (users.size() >= static_cast<size_t>(user_limit) && user_limit != -1) {
        std::string message = "User limit reached!\r\n";
        std::cout << message << std::endl;
        // send(user.getFd(), message.c_str(), message.length(), 0);
        return false;
    }
    if (users.find(user.getNickname()) != users.end()) {
        std::string message = " User is already in the channel!\r\n";
        std::cout << message << std::endl;
        // send(user.getFd(), message.c_str(), message.length(), 0);
        return false;
    }
    if (is_operator) {
        users.insert({user.getNickname(), true});
        std::string message = "added user " + user.getNickname() + " to channel: " + name + "\r\n";
        std::cout << message << std::endl;
        // send(user.getFd(), message.c_str(), message.length(), 0);
    } else {
        std::string message = "added user as non-operator to channel\r\n";
        std::cout << message << std::endl;
        // send(user.getFd(), message.c_str(), message.length(), 0);
        users.insert({user.getNickname(), false});
    }
        return true;
}

}