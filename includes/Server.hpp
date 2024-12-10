#pragma once

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <signal.h>
#include <vector>
#include <poll.h>
#include <sstream>
#include <map>
#include <atomic>
#include <csignal>

#include "User.hpp"
#include "Channel.hpp"
#include "irc_replies.hpp"

/*
Class Server contains:
- important data for each server:
    - port
    - password
- map of users that are currently in a Server (we can find each user based on the fd of the user)
- map of channels that are currently in the Server (we can find each channel based on channel name)
- constructors and destructor
- getters and setters
- helping methods that are private (explanation of each should be next to it's definiton) 
- outside of server we can access only methods:
    - setup_server - main function that is creating server and doing everything
    - validateClientPassword - for validation is password correct when loging into server

Class Server connects classes User and Channel and contains all users and channels that are in a server 
and because of that in Server should be functions for manipulation of users and channels!
*/

namespace irc
{

class Server
{
    private:
        int port;
        std::string password;
        std::map<int, irc::User> users;
        std::map<std::string, irc::Channel> channels;

        // Server.cpp
        int create_socket() const;
        bool bind_and_listen(int server_socket) const;
        bool poll_connections(int server_socket, std::vector<pollfd>& fds);
        void accept_new_client(int server_socket, std::vector<pollfd>& fds);
        bool process_client_input(int client_fd);
        void send_welcome_message(int client_fd, const std::string& nickname);
        void handle_ping_pong(int client_fd, const std::string& line, const std::string &server_name);
        void close_client(int client_fd, std::vector<pollfd>& fds, size_t index);

        // ServerAuthentication.cpp
        void authentication(int client_fd, std::string line);
        bool isNicknameTaken(const std::string &nickname) const;
        void handle_nick(int client_fd, const std::string& line);
        void handle_user(int client_fd, const std::string& line);
        void handle_password_phase(int client_fd, const std::string& line);

        // ServerParser.cpp
        void parse_commands(int client_fd, const std::string& line);


        // ServerKick.cpp
        void kick_user(const std::string& channel_name, const std::string& reason, const std::string& user_to_kick);
        void kick(int client_fd, std::vector<std::string> tokens);

        // ServerMode.cpp
        void handle_mode(int client_fd, std::vector<std::string> tokens);

        // ServerPing.cpp
        void handle_ping_pong(int client_fd, std::vector<std::string> tokens);

        // ServerNick.cpp
        void change_nick(int client_fd, std::string new_nickname);

        // ServerUtils.cpp
        void send_to_joined_channels(int client_fd, std::string message);
        void send_to_channel(std::string channel_name, std::string message);
        void send_to_user(int client_fd, std::string message);

        // ServerUser.cpp
        bool IsLimitReached(const std::string &channel_name);

        // ServerJoin.cpp
        std::string getChannelNicks(std::string channel_name);
        void addUser(int client_fd, std::string channel_name);
        void join(int client_fd, std::string channel_name, std::string channel_pass);

        // ServerQuit.cpp
        void quit(int client_fd, const std::string& reason);
    public:
        ~Server();
        Server();
        Server(int port, std::string password);
        Server(const Server &copy);
        Server &operator=(const Server &src);

        int getPort() const;
        std::string getPassword() const;

        int setup_server();
        bool validateClientPassword(const std::string &clientPassword) const;
};

}