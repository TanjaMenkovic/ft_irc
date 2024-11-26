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

// Helper function to process client input
bool Server::process_client_input(int client_fd, std::vector<std::pair<int, bool>>& client_status, size_t index) {
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

        if (!client_status[index].second) {
            if (!handle_password_phase(client_fd, line, client_status, index)) {
                return false;
            }
        } else {
            handle_client_message(line);
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


}
