#include "../includes/Server.hpp"

namespace irc 
{

// Message irssi broadcasts to channels when user quits:
// 18:34 -!- tvalimak_ [~tvalimak@freenode-3ad.s3h.4nuk5f.IP] has quit [Quit: leaving]

void Server::quit(int client_fd, const std::string& reason) {

    // Construct the quit message
    std::string message = RPL_QUIT(users[client_fd].getUsername(), users[client_fd].getNickname(), reason);

    // Get the list of channels the user is part of
    std::map<std::string, bool> joined_channels = users[client_fd].getJoinedChannels();

    // Iterate through the joined channels
    for (const auto& [channel_name, is_operator] : joined_channels) {
        std::cout << channel_name << is_operator << "\n";
            if (channels.find(channel_name) != channels.end()) {
                send_to_channel(channel_name, message);
            }
            // Call leaveChannel for the channel
            users[client_fd].leaveChannel(channel_name);
    }
    // Remove the user from the server
    users.erase(client_fd);
}

}

 // namespace irc