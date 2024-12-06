#include "../includes/Server.hpp"

namespace irc 
{

void Server::handle_mode(int client_fd, std::vector<std::string> tokens)
{
    (void) client_fd;
    (void) tokens;
    // std::string input = line.substr(5);
    // std::istringstream iss(input);
    // std::vector<std::string> tokens;
    // std::string token;
    
    // while (iss >> token) {
    //     tokens.push_back(token);
    // }
    
    // if (tokens.empty()) {
    //     std::cerr << "Error: No channel name provided in MODE command.\n";
    //     return ;
    // }   

    // std::string channel = tokens[0];
    // if (channel[0] != '#' && channel[0] != '&') {
    //     std::cerr << "Error: Invalid channel name '" << channel << "'. Channel names must start with '#' or '&'.\n";
    //     return ;
    // } else {
    //     channel = channel.substr(1);
    // }

    // // add checking if channel name is inside of our available channels in class, and if not to print an error: 
    // // if channel doen't exist: No such channel
    // // if we are not at correct channel: You're not on that channel
    // // if not operator: You're not channel operator

    // if (tokens.size() < 2) {
    //     std::cerr << "Error: No mode provided for channel " << channel << ".\n";
    //     return ;
    // }

    // for (size_t i = 1; i < tokens.size(); ++i) {
    //     const std::string& mode = tokens[i];
        
    //     if (mode[0] == '+') {
    //         // do something
    //     } else if (mode[0] == '-') {
    //         // do something
    //     } else if (mode[0] == 'i' || mode[0] == 't' || mode[0] == 'k') {
    //         // :server 324 your_nickname #channel +itk
    //         return ;
    //     } else {
    //         // is unknown mode char to me
    //         return ;
    //     } 


    // }

    // std::cout << "Finished processing MODE command for channel " << channel << std::endl;
}


}