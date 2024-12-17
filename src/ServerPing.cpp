#include "../includes/Server.hpp"

namespace irc 
{

void Server::handle_ping_pong(int client_fd, std::vector<std::string> tokens) {
    // Construct a proper PONG response with the server name and parameter
    std::string pong_response = ":" + tokens[0] + " PONG " + tokens[0] + " :" + tokens[0] + "\r\n";

    // Send the response to the client
    send(client_fd, pong_response.c_str(), pong_response.size(), 0);
}

}