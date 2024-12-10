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
/*
void Server::quit(int client_fd, const std::string reason, std::map<std::string, irc::Channel> &channels) {
    std::string message;
    const std::string& user_nickname = users[client_fd].getNickname(); // Get the user's nickname

    // Remove user from all channels
    for (std::map<std::string, irc::Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
        irc::Channel& channel = it->second;

        // Check if the user is in the channel
        const auto& users_map = channel.getUsers();  // recreate the missing functions into ServerUser.cpp
        if (users_map.find(user_nickname) != users_map.end()) {
            channel.removeUser(client_fd); // Remove the user from the channel, recreate the missing functions into ServerUser.cpp

            // Notify remaining users in the channel
            message = RPL_QUIT(users[client_fd].getUsername(), users[client_fd].getNickname(), reason);
            send_to_joined_channels(client_fd, message);
        }
    }

    // Log the quit event
    std::cout << "User " << user_nickname << " disconnected: " << reason << std::endl;

    // Remove the user from the server's user list
    users.erase(client_fd);
}*/

 // namespace irc