#pragma once

#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <vector>
#include <map>
#include <cstring>    // For memset
#include <unistd.h>   // For close()
#include <sstream>    // For std::stringstream
#include <iostream>   // For std::cout, std::cerr
#include <poll.h>     // For pollfd

#include "User.hpp"
#include "Channel.hpp"

namespace irc
{

class Server
{
    private:
        int port;
        std::string password;
        std::map<int, irc::User> users;
        std::map<std::string, irc::Channel> channels;

        int create_socket() const;
        bool bind_and_listen(int server_socket) const;
        bool poll_connections(int server_socket, std::vector<pollfd>& fds, std::vector<std::pair<int, bool>>& client_status);
        void accept_new_client(int server_socket, std::vector<pollfd>& fds, std::vector<std::pair<int, bool>>& client_status);
        bool process_client_input(int client_fd, std::vector<std::pair<int, bool>>& client_status, size_t index);
        bool handle_password_phase(int client_fd, const std::string& line, std::vector<std::pair<int, bool>>& client_status, size_t index);
        void handle_client_message(int client_fd, const std::string& line);
        void close_client(int client_fd, std::vector<pollfd>& fds, std::vector<std::pair<int, bool>>& client_status, size_t index);

    public:
        ~Server();
        Server();
        Server(int port, std::string password);
        Server(const Server &copy);
        Server &operator=(const Server &src);

        int getPort() const;
        std::string getPassword() const;
        const std::map<int, irc::User>& getUsers() const;

        int setup_server();
        bool validateClientPassword(const std::string &clientPassword) const;

};

const User* find_user_by_nickname(const std::string& nickname, const Server& serv);

}