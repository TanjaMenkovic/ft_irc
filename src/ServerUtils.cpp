#include "../includes/Server.hpp"

namespace irc 
{

// send to each user that is in the same channel as the client_fd
void Server::send_to_joined_channels(int client_fd, std::string message)
{
    std::set<std::string> joined_channels = users[client_fd].getJoinedChannels();

    for (std::string channel_name : joined_channels) {
        auto it = channels.find(channel_name);
        if (it != channels.end())
            send_to_channel(client_fd, channel_name, message);
    }
}

// send to everu user in a specific channel except client_fd
void Server::send_to_channel(int client_fd, std::string channel_name, std::string message) {
    for (auto it : this->users) {
        if (it.first != client_fd && it.second.isInChannel(channel_name)) {
            send(it.first, message.c_str(), message.length(), 0);
        }
    }
}

}