#include "../includes/Server.hpp"

namespace irc 
{

// send to each user that is in the same channel as the client_fd
void Server::send_to_joined_channels(int client_fd, std::string message)
{
    std::map<std::string, bool> joined_channels = users[client_fd].getJoinedChannels();

    for (auto channel_name : joined_channels) {
        auto it = channels.find(channel_name.first);
        if (it != channels.end())
            send_to_channel(channel_name.first, message);
    }
}

// send to every user in a specific channel
void Server::send_to_channel(std::string channel_name, std::string message) {
    for (auto it : this->users) {
        if (it.second.isInChannel(channel_name)) {
            send(it.first, message.c_str(), message.length(), 0);
        }
    }
}

void Server::send_to_channel_not_fd(int fd, std::string channel_name, std::string message) {
    for (auto it : this->users) {
        if (it.second.isInChannel(channel_name) && it.first != fd) {
            send(it.first, message.c_str(), message.length(), 0);
        }
    }
}

// sending only to user with client_fd
void Server::send_to_user(int client_fd, std::string message) {
    send(client_fd, message.c_str(), message.length(), 0);
}

}