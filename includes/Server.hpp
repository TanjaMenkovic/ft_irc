#pragma once

#include <iostream> // For std::cout, std::cerr
#include <cstring> // For memset
#include <cstdlib>
#include <cstdio>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // For close()
#include <cctype>  // For std::isspace
#include <signal.h>
#include <vector>
#include <poll.h> // For pollfd
#include <sstream> // For std::stringstream
#include <map>
#include <unordered_map>
#include <atomic>
#include <csignal>
#include <thread>
#include <mutex>

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
        void close_client(int client_fd, std::vector<pollfd>& fds, size_t index);

        // ServerAuthentication.cpp
        void authentication(int client_fd, std::string line);
        bool isNicknameTaken(const std::string &nickname) const;
        void handle_nick(int client_fd, const std::string& line);
        void handle_user(int client_fd, const std::string& line);
        void handle_password_phase(int client_fd, const std::string& line);
        void send_mode_message(int client_fd, const std::string& nickname);
        void send_welcome_message(int client_fd, const std::string& nickname);

        // ServerParser.cpp
        void parse_commands(int client_fd, const std::string& line);

        //ServerInvite.cpp
        void invite(int client_fd, std::vector<std::string> tokens);
        void invite_user(const std::string &invited_user, const std::string& channel_name, int client_fd);

        // ServerPrivmsg.cpp
        void privmsg(int client_fd, std::vector<std::string> tokens);
        void send_privmsg(int client_fd, const std::string& receiver, const std::string& privmessage);

        // ServerKick.cpp
        void kick_user(const std::string& channel_name, const std::string& reason, const std::string& user_to_kick);
        void kick(int client_fd, std::vector<std::string> tokens);

        // ServerTopic.cpp
        void topic(int client_fd, std::vector<std::string> tokens);

        // ServerMode.cpp
        void handle_mode(int client_fd, std::vector<std::string> tokens);
        void channel_invite(int client_fd, std::string channel_name, std::string mode);
        void channel_topic(int client_fd, std::string channel_name, std::string mode);
        void channel_password(int client_fd, std::string channel_name, std::string mode, std::string password);
        void channel_limit(int client_fd, std::string channel_name, std::string mode, std::string limit_str);
        void channel_user(int client_fd, std::string channel_name, std::string mode, std::string user_nickname);
        bool is_in_channel(std::string user_nickname, std::string channel_name, bool is_operator);
        bool check_if_operator(int client_fd, std::string channel_name);

        // ServerPing.cpp
        void handle_ping_pong(int client_fd, std::vector<std::string> tokens);

        // ServerNick.cpp
        void change_nick(int client_fd, std::string new_nickname);

        // ServerUtils.cpp
        void send_to_joined_channels(int client_fd, std::string message);
        void send_to_joined_channels_not_fd(int client_fd, std::string message);
        void send_to_channel(std::string channel_name, std::string message);
        void send_to_channel_not_fd(int fd, std::string channel_name, std::string message);
        void send_to_user(int client_fd, std::string message);

        // ServerUser.cpp
        bool IsLimitReached(const std::string &channel_name);

        // ServerJoin.cpp
        std::string getChannelNicks(std::string channel_name);
        void addUser(int client_fd, std::string channel_name);
        void join(int client_fd, std::string channel_name, std::string channel_pass);

        // ServerQuit.cpp
        void quit(int client_fd, const std::string& reason);
        void is_channel_empty(std::string channel_name);

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