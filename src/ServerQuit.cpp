#include "../includes/Server.hpp"

namespace irc 
{

void Server::quit(int client_fd, const std::string& reason) {

    // Construct the quit message
    std::string message = RPL_QUIT(users[client_fd].getUsername(), users[client_fd].getNickname(), reason);

    // Get the list of channels the user is part of
    std::map<std::string, bool> joined_channels = users[client_fd].getJoinedChannels();

    // Iterate through the joined channels
    for (const auto& [channel_name, is_operator] : joined_channels) {
            if (channels.find(channel_name) != channels.end()) {
                send_to_channel(channel_name, message);
            }
            // Call leaveChannel for the channel
            users[client_fd].leaveChannel(channel_name);
            is_channel_empty(channel_name);
    }
    // Remove the user from the server
    users.erase(client_fd);
}

// Function that iterates through all users and checks if any user is part of an given channel.
// If no user is found to be part of the given channel, the channel will be removed.
void Server::is_channel_empty(std::string channel_name) {
    for (auto it : this->users) {
        if (it.second.isInChannel(channel_name)) {
            return ;
        }
    }
    // delete the channel
    this->channels.erase(channel_name);
}

}
