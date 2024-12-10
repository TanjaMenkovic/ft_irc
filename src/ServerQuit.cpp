#include "../includes/Server.hpp"

namespace irc 
{

// quit function has to go through all channels that the quitting user is part of, print a quit message to the channel
// and then remove that user from that channel before he quits the server.

void Server::quit(int client_fd, const std::string& reason)
{
    std::string message;
    const std::string& user_nickname = users[client_fd].getNickname(); // Get the user's nickname

    // Map to store channels the user is part of
    std::map<std::string, irc::Channel*> user_channels;

    // Iterate through all channels
    for (std::map<std::string, irc::Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
        irc::Channel& channel = it->second;

        // Assuming getChannelNicks returns a map or a list of nicknames in the channel
        const auto& nicknames = getChannelNicks(channel.getName());

        // Check if the user's nickname exists in the channel
        if (nicknames.find(user_nickname) != nicknames.end()) {
            user_channels[it->first] = &channel; // Add to the map
        }
    }

    // Remove the user from all channels they were part of
    for (std::map<std::string, irc::Channel*>::iterator it = user_channels.begin(); it != user_channels.end(); ++it) {
        irc::Channel* channel = it->second;
        channel->removeUser(client_fd); // Assuming removeUser is a function that removes a user by client_fd
    }

    // Construct the quit message
    message = ":" + user_nickname + " QUIT :" + reason + "\r\n";

    // Send the quit message to all users (assuming broadcastMessage exists)
    broadcastMessage(message);

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

} // namespace irc