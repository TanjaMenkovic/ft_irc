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
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        return false;
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        return false;
    }

    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        return false;
    }

    std::cout << "Server listening on port " << port << "...\n";

    std::vector<pollfd> fds;
    fds.push_back({server_socket, POLLIN, 0}); // Add listening socket to poll
    std::vector<std::pair<int, bool>> client_password_status; // Track if a client has validated their password

    char buffer[1024];

    while (true) {
        int poll_count = poll(fds.data(), fds.size(), -1); // Wait indefinitely
        if (poll_count < 0) {
            perror("Poll failed");
            break;
        }

        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_socket) {
                    // Accept new client connection
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
                    if (client_socket < 0) {
                        perror("Accept failed");
                        continue;
                    }

                    std::cout << "New client connected.\n";
                    std::string prompt = "Please enter the server password: ";
                    send(client_socket, prompt.c_str(), prompt.size(), 0);

                    // Add new client to poll
                    fds.push_back({client_socket, POLLIN, 0});
                    client_password_status.push_back({client_socket, false}); // Set as not validated initially
                } else {
                    // Data from existing client
                    memset(buffer, 0, sizeof(buffer));
                    int bytes_received = recv(fds[i].fd, buffer, sizeof(buffer) - 1, 0);
                    if (bytes_received <= 0) {
                        std::cout << "Client disconnected or error occurred.\n";
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i); // Remove client socket
                        client_password_status.erase(client_password_status.begin() + i); // Remove client password status
                        --i;
                    } else {
                        // Handle client input line by line
                        std::string client_data(buffer);
                        std::stringstream ss(client_data);
                        std::string line;

                        while (std::getline(ss, line, '\n')) {
                            // Remove trailing \r if present (from CRLF endings)
                            if (!line.empty() && line.back() == '\r') {
                                line.pop_back();
                            }

                            if (!client_password_status[i].second) {
                                // Password validation phase
                                if (line.find("PASS ") == 0) {
                                    std::string client_password = line.substr(5); // Extract password
                                    if (!validateClientPassword(client_password)) {
                                        std::string error_message = "ERROR: Incorrect password. Connection closed.\n";
                                        send(fds[i].fd, error_message.c_str(), error_message.size(), 0);
                                        std::cerr << "Client provided an incorrect password. Connection terminated.\n";
                                        close(fds[i].fd);
                                        fds.erase(fds.begin() + i);
                                        client_password_status.erase(client_password_status.begin() + i);
                                        --i;
                                    } else {
                                        std::string success_message = "Password accepted. Welcome to the server!\n";
                                        send(fds[i].fd, success_message.c_str(), success_message.size(), 0);
                                        client_password_status[i].second = true; // Mark password validated
                                    }
                                } else {
                                    std::string error_message = "ERROR: Please provide a valid password using PASS command.\n";
                                    send(fds[i].fd, error_message.c_str(), error_message.size(), 0);
                                }
                            } else {
                                // Handle messaging phase after password is validated
                                if (line.find("CAP ") == 0) {
                                    std::cout << "Received capability command: " << line << std::endl;
                                } else {
                                    std::cerr << "Unrecognized command: " << line << std::endl;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    close(server_socket);
    return true;
}

}
