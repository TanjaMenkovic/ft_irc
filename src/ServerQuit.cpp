#include "../includes/Server.hpp"

namespace irc 
{

void Server::quit(User &user, const std::string reason, std::map<std::string, irc::Channel> &channels) {
    int client_fd = user.getFd(); // Assuming User has a method to get its socket file descriptor
    const std::string& user_nickname = user.getNickname(); // Get the user's nickname

    // Remove user from all channels
    for (std::map<std::string, irc::Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
        irc::Channel& channel = it->second;

        // Check if the user is in the channel
        const auto& users_map = channel.getUsers();
        if (users_map.find(user_nickname) != users_map.end()) {
            channel.removeUser(user); // Remove the user from the channel

            // Notify remaining users in the channel
            channel.broadcastMessage(user, user_nickname + " has quit: " + reason, users);
        }
    }

    // Log the quit event
    std::cout << "User " << user_nickname << " disconnected: " << reason << std::endl;

    // Remove the user from the server's user list
    users.erase(client_fd);
}

} // namespace irc