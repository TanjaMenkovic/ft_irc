#include "../includes/Server.hpp"

namespace irc 
{

// void Server::quit(int client_fd, const std::string reason, std::map<std::string, irc::Channel> &channels) {
//     std::string message;
//     const std::string& user_nickname = users[client_fd].getNickname(); // Get the user's nickname

//     // Remove user from all channels
//     for (std::map<std::string, irc::Channel>::iterator it = channels.begin(); it != channels.end(); ++it) {
//         irc::Channel& channel = it->second;

//         // Check if the user is in the channel
//         const auto& users_map = channel.getUsers();
//         if (users_map.find(user_nickname) != users_map.end()) {
//             channel.removeUser(client_fd); // Remove the user from the channel

//             // Notify remaining users in the channel
//             message = RPL_QUIT(users[client_fd].getUsername(), users[client_fd].getNickname(), reason);
//             send_to_joined_channels(client_fd, message);
//         }
//     }

//     // Log the quit event
//     std::cout << "User " << user_nickname << " disconnected: " << reason << std::endl;

//     // Remove the user from the server's user list
//     users.erase(client_fd);
// }

} // namespace irc