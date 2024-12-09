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

        // ServerMode.cpp
        void handle_mode(int client_fd, std::vector<std::string> tokens);

        // ServerPing.cpp
        void handle_ping_pong(int client_fd, std::vector<std::string> tokens);

        // ServerNick.cpp
        void change_nick(int client_fd, std::string new_nickname);

        // ServerUtils.cpp
        void send_to_joined_channels(int client_fd, std::string message);
        void send_to_channel(int client_fd, std::string channel_name, std::string message);

        // Server Commands
        void join(User &user, const std::string &channel_name, std::map<std::string, irc::Channel> &channels);
        void kick(User &user, const std::string client_data);
        void quit(int client_fd, const std::string reason, std::map<std::string, irc::Channel> &channels);
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