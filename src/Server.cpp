#include "../includes/Server.hpp"
#include <cstring>    // For memset
#include <unistd.h>   // For close()
#include <sstream>    // For std::stringstream
#include <iostream>   // For std::cout, std::cerr
#include <poll.h>     // For pollfd

namespace irc {

bool Server::validateClientPassword(const std::string &clientPassword) const {
    return clientPassword == password;
}

Server::Server(): port(0), password("unknown") {}

Server::Server(int port, std::string password): port(port), password(password) {}

Server::~Server() {}

Server::Server(const Server &copy) {
    *this = copy;
}

Server &Server::operator=(const Server &src) {
    if (this != &src) {
        this->port = src.getPort();
        this->password = src.getPassword();
    }
    return *this;
}

int Server::getPort() const {
    return this->port;
}

std::string Server::getPassword() const {
    return this->password;
}

int Server::setup_server() {
    int server_socket = create_socket();
    if (server_socket < 0) {
        return false;
    }

    if (!bind_and_listen(server_socket)) {
        return false;
    }

    std::cout << "Server listening on port " << port << "...\n";

    std::vector<pollfd> fds = {{server_socket, POLLIN, 0}};
    std::vector<std::pair<int, bool>> client_status; // Track if client is authenticated

    while (true) {
        if (!poll_connections(server_socket, fds, client_status)) {
            break;
        }
    }

    close(server_socket);
    return true;
}

// Helper function to create the server socket
int Server::create_socket() const {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return -1;
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    return server_socket;
}

// Helper function to bind and listen on the socket
bool Server::bind_and_listen(int server_socket) const {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Listen on all available interfaces
    server_addr.sin_port = htons(port);        // Set the port number

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        return false;
    }

    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        return false;
    }

    return true;
}

// Helper function to handle polling and client connections
bool Server::poll_connections(int server_socket, std::vector<pollfd>& fds, std::vector<std::pair<int, bool>>& client_status) {
    int poll_count = poll(fds.data(), fds.size(), -1);
    if (poll_count < 0) {
        perror("Poll failed");
        return false;
    }

    for (size_t i = 0; i < fds.size(); ++i) {
        if (fds[i].revents & POLLIN) {
            if (fds[i].fd == server_socket) {
                accept_new_client(server_socket, fds, client_status);
            } else {
                int client_fd = fds[i].fd;
                if (!process_client_input(client_fd, client_status, i)) {
                    close_client(client_fd, fds, client_status, i);
                    --i; // Adjust index as client is removed
                }
            }
        }
    }

    return true;
}

// Helper function to accept a new client
void Server::accept_new_client(int server_socket, std::vector<pollfd>& fds, std::vector<std::pair<int, bool>>& client_status) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket < 0) {
        perror("Accept failed");
        return;
    }

    std::cout << "New client connected.\n";
    std::string prompt = "Validating server password... ";
    send(client_socket, prompt.c_str(), prompt.size(), 0);

    fds.push_back({client_socket, POLLIN, 0});
    client_status.push_back({client_socket, false});

    // Create a User instance and store it in the users map
    users[client_socket] = User(client_socket);
}

// I guess we need to figue out what IRSSI is expecting from the server to know its connected

bool Server::process_client_input(int client_fd, std::vector<std::pair<int, bool>>& client_status, size_t index) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        std::cout << "Client disconnected or error occurred.\n";
        return false;
    }

    // Print the raw buffer to debug incoming data
    std::cout << "Raw buffer received: " << buffer << "\n";

    // Handle potential IRC line endings and command prefixes
    std::string client_data(buffer);
    std::stringstream ss(client_data);
    bool nick = false;
    bool user = false;

    std::string line;
    
    while (std::getline(ss, line)) {
        // Strip \r if it exists (for IRC compatibility)
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // Print each cleaned line
        std::cout << "Cleaned Line: " << line << "\n";

        if (line.empty()) {
            continue;  // Skip empty lines
        }

        if (nick == false)
            nick = handle_nick(client_fd, line);

        if (user == false)
            user = handle_user(client_fd, line);

        // Handle password phase if applicable
        if (!client_status[index].second) {

            if (!handle_password_phase(client_fd, line, client_status, index)) {
                return false;
            }
        }

        if (client_status[index].second && !client_status[index].first && nick && user) {
            send_welcome_message(client_fd, users[client_fd].getNickname()); // Replace with actual username
            client_status[index].first = true; // Mark welcome message as sent
        }

        // Check for PING command and respond with PONG
        if (client_data.find("PING ") == 0) {
            handle_ping_pong(client_fd, client_data, "my_server_name");
        }
         if (client_data.find("JOIN ") == 0) {
            join(users[client_fd], "testing", this->channels);
        }
    }

    return true;
}

bool Server::isNicknameTaken(const std::string &nickname) const {
    for (const auto &user : users) {
        if (user.second.getNickname() == nickname) {
            return true; // Nickname is taken
        }
    }
    return false; // Nickname is available
}

bool Server::handle_nick(int client_fd, const std::string& line)
{
    if (line.find("NICK ") == 0) {
        std::string nickname = line.substr(5);

        std::string original = nickname;
        int counter = 1;

        while (isNicknameTaken(nickname)) {
            nickname = original + std::to_string(counter);
            counter++;
        }

        users[client_fd].setNickname(nickname);
        std::cout << "User set nickname: " << users[client_fd].getNickname() << std::endl;
        return true;
    }
    return false;
}

bool Server::handle_user(int client_fd, const std::string& line)
{
    if (line.find("USER ") == 0){
        std::size_t start_pos = 5;
        std::size_t space_pos = line.find(" ", start_pos);

        std::string username = line.substr(start_pos, space_pos - start_pos);
        users[client_fd].setUsername(username);
        std::cout << "User set username: " << users[client_fd].getUsername() << std::endl;
        return true;
    }
    return false;
}

// :tantalum.libera.chat PONG tantalum.libera.chat :tantalum.libera.chat

void Server::handle_ping_pong(int client_fd, const std::string& line, const std::string& server_name) {
    if (line.find("PING ") == 0) {
        std::cout << "Responding to PING...\n";

        // Extract the parameter after "PING "
        std::string ping_param = line.substr(5);

        // Construct a proper PONG response with the server name and parameter
        std::string pong_response = ":" + server_name + " PONG " + server_name + " :" + ping_param + "\r\n";

        // Send the response to the client
        send(client_fd, pong_response.c_str(), pong_response.size(), 0);

        std::cout << "Sent: " << pong_response;
    }
}

// Helper function to send the IRC welcome message
void Server::send_welcome_message(int client_fd, const std::string& nickname) {
    std::string welcome_msg = ":ft_irc 001 " + nickname + " :Welcome to the IRC Network, " + nickname + "!\r\n";
    send(client_fd, welcome_msg.c_str(), welcome_msg.length(), 0);
    std::cout << "Sent welcome message to client: " << nickname << "\n";
}

// Helper function to handle the password phase
bool Server::handle_password_phase(int client_fd, const std::string& line, std::vector<std::pair<int, bool>>& client_status, size_t index) {
    if (line.find("PASS ") == 0) {
        std::string client_password = line.substr(5); // Extract password
        if (!validateClientPassword(client_password)) {
            std::string error_message = "ERROR: Incorrect password. Connection closed.\n";
            send(client_fd, error_message.c_str(), error_message.size(), 0);
            std::cerr << "Client provided an incorrect password. Connection terminated.\n";
            return false;
        } else {
            std::string success_message = "Password accepted. Welcome to the server!\n";
            send(client_fd, success_message.c_str(), success_message.size(), 0);
            client_status[index].second = true;
        }
    } else if (line.find("CAP ") == 0) {
        // Suppress errors for CAP commands
        std::cout << "Received capability command: " << line << std::endl;
    } else if (!line.empty()) {
        std::cout << "Received unexpected command: " << line << std::endl;
    }

    return true;
}

// Helper function to handle client messages after password validation
void Server::handle_client_message(const std::string& line) {
    if (line.find("CAP ") == 0) {
        std::cout << "Received capability command: " << line << std::endl;
    } else {
        std::cerr << "Unrecognized command: " << line << std::endl;
    }
}

// Helper function to close a client connection
void Server::close_client(int client_fd, std::vector<pollfd>& fds, std::vector<std::pair<int, bool>>& client_status, size_t index) {
    close(client_fd);
    fds.erase(fds.begin() + index);
    client_status.erase(client_status.begin() + index);
}


void Server::join(User &user, const std::string &channel_name, std::map<std::string, irc::Channel> &channels) {
    std::string client_message;
    std::map<std::string, irc::Channel>::iterator found_channel = channels.find(channel_name);
    // check whether channel exists
    if (found_channel == channels.end()) {
        // channel doesn't exist, so we create a new one
        irc::Channel new_channel = irc::Channel(channel_name);
        this->channels.insert({channel_name, new_channel});
    } else {
        // channel exists
        found_channel->second.addUser(user);
    }
    // add to user joined channels
    user.joinChannel(channel_name);
}

}
