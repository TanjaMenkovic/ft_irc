#include "../includes/Server.hpp"

namespace irc {

// Checks if the client's provided password is valid
bool Server::validateClientPassword(const std::string &clientPassword) const {
    return clientPassword == password;
}

Server::Server(): port(0), password("unknown"), users(), channels() {}

Server::Server(int port, std::string password): port(port), password(password), users(), channels() {}

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

const std::map<int, irc::User>& Server::getUsers() const {
    return this->users;
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
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

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

    // Iterate through the file descriptors and handle events
    for (size_t i = 0; i < fds.size(); ++i) {
        if (fds[i].revents & POLLIN) {
            if (fds[i].fd == server_socket) {
                accept_new_client(server_socket, fds, client_status);
            } else {
                int client_fd = fds[i].fd;
                // Validate if client status is valid and exists before processing input
                if (i < client_status.size() && !process_client_input(client_fd, client_status, i)) {
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
    client_status.push_back({client_socket, false}); // False means client hasn't authenticated
}

// Helper function to process client input
bool Server::process_client_input(int client_fd, std::vector<std::pair<int, bool>>& client_status, size_t index) {
    if (index >= client_status.size()) {
        std::cerr << "Invalid index: " << index << std::endl;
        return false;  // Exit if the index is invalid
    }

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        std::cout << "Client disconnected or error occurred.\n";
        return false;
    }

    std::string client_data(buffer);
    std::stringstream ss(client_data);
    std::string line;

    while (std::getline(ss, line, '\n')) {
        if (!line.empty() && line.back() == '\r') {
            line.pop_back(); // Remove trailing \r
        }

        if (!client_status[index].second) { // Check if client is authenticated
            if (!handle_password_phase(client_fd, line, client_status, index)) {
                return false; // If password is incorrect, disconnect the client
            }
        } else {
            handle_client_message(client_fd, line); // Process the command after authentication
        }
    }

    return true;
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
            client_status[index].second = true; // Mark client as authenticated
        }
    } else if (line.find("CAP ") == 0) {
        // Suppress errors for CAP commands
        std::cout << "Received capability command: " << line << std::endl;
    } else if (!line.empty()) {
        std::cout << "Received unexpected command during password phase: " << line << std::endl;
    }

    return true;
}

// Helper function to handle client messages after password validation
void Server::handle_client_message(int client_fd, const std::string& line) {
    if (line.find("NICK ") == 0) {
        std::string nickname = line.substr(5);
        users[client_fd].setNickname(nickname);
        std::cout << "User set nickname: " << nickname << std::endl;

        // Send server welcome messages
        std::string welcome = 
            ":server_name 001 " + nickname + " :Welcome to the IRC network\r\n"
            ":server_name 002 " + nickname + " :Your host is server_name, running version 1.0\r\n"
            ":server_name 003 " + nickname + " :This server was created today\r\n";
        send(client_fd, welcome.c_str(), welcome.size(), 0);

    } else if (line.find("USER ") == 0) {
        // Handle USER command
        users[client_fd].setUsername(line.substr(5)); // Set username
        std::cout << "User set username: " << users[client_fd].getUsername() << std::endl;

    } else if (line.find("JOIN ") == 0) {
        // Handle JOIN command
        std::string channel_name = line.substr(5);
        if (channels.find(channel_name) == channels.end()) {
            channels[channel_name] = Channel(channel_name); // Create new channel
        }
        channels[channel_name].addUser(users[client_fd]);

        std::cout << "User joined channel: " << channel_name << std::endl;

        // Optionally send JOIN response
        std::string join_response = ":" + users[client_fd].getNickname() + " JOIN :" + channel_name + "\r\n";
        send(client_fd, join_response.c_str(), join_response.size(), 0);
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

}