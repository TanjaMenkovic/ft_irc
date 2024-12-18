#include "../includes/Server.hpp"

namespace irc 
{

void Server::parse_commands(int client_fd, const std::string& line)
{
    std::istringstream iss(line);
    std::vector<std::string> tokens;
    std::string token;
    
    std::string command_name;

    if (iss)
        iss >> command_name;

    while (iss >> token) {
        tokens.push_back(token);
    }
    
    if (tokens.empty()) {
        std::cerr << "Error: No channel name provided in command.\n";
        return ;
    } 

    std::unordered_map<std::string, int> commandMap = {
        {"KICK", 1},
        {"INVITE", 2},
        {"TOPIC", 3},
        {"MODE", 4},
        {"JOIN", 5},
        {"PRIVMSG", 6},
        {"NICK", 7},
        {"PING", 8},
        {"QUIT", 9}
    };

    auto it = commandMap.find(command_name);
    if (it != commandMap.end()) {
        switch (it->second) {
            case 1:
                if (tokens.size() < 2) {
                    std::string error_message =  ERR_NEEDMOREPARAMS(users[client_fd].getNickname(), "KICK");
                    send_to_user(client_fd, error_message);
                    return ;
                }
                kick(client_fd, tokens);
                break;
            case 2:
                if (tokens.size() < 2) {
                    std::string error_message =  ERR_NEEDMOREPARAMS(users[client_fd].getNickname(), "INVITE");
                    send_to_user(client_fd, error_message);
                }
                invite(client_fd, tokens);
                break;
            case 3:
                topic(client_fd, tokens);
                break;
            case 4:
                handle_mode(client_fd, tokens);
                break;
            case 5:
                if (tokens.size() == 1) {
                    tokens.push_back("");
                }
                join(client_fd, tokens[0], tokens[1]);
                break;
            case 6:
                if (tokens.size() < 2) {
                    std::string error_message =  ERR_NEEDMOREPARAMS(users[client_fd].getNickname(), "PRIVMSG");
                    send_to_user(client_fd, error_message);
                    return ;
                }
                privmsg(client_fd, tokens);
                break;
            case 7:
                if (tokens.size() < 1)  {
                    tokens.push_back("");
                }
                change_nick(client_fd, tokens[0]);
                break;
            case 8:
                handle_ping_pong(client_fd, tokens);
                break;
            case 9:
                quit(client_fd, tokens[0]);
                break;
            default:
                std::string message = ERR_UNKNOWNCOMMAND(users[client_fd].getNickname(), command_name);
                send_to_user(client_fd, message);
                break;
        }
    } else {
        std::string message = ERR_UNKNOWNCOMMAND(users[client_fd].getNickname(), command_name);
        send_to_user(client_fd, message);
    }

}

}