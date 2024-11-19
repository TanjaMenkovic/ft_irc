#include "../includes/Server.hpp"
#include <set> // For managing multiple client sockets

namespace irc
{

Server::Server(): port(0), password("unknown") {}

Server::Server(int port, std::string password): port(port), password(password) {}

Server::~Server() {}

Server::Server(const Server &copy)
{
	*this = copy;
}

Server &Server::operator=(const Server &src)
{
	if (this != &src)
    {
        this->port = src.getPort();
        this->password = src.getPassword();
    }
    return *this;
}

int Server::getPort() const
{
    return this->port;
}

std::string Server::getPassword() const
{
    return this->password;
}

int Server::initializeSocket()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
        throw std::runtime_error("Error creating socket");

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        close(server_socket);
        throw std::runtime_error("Bind failed");
    }

    if (listen(server_socket, SOMAXCONN) < 0)
    {
        close(server_socket);
        throw std::runtime_error("Listen failed");
    }

    return server_socket;
}

void Server::handleClient(int client_socket)
{
    char buffer[1024] = {0};
    int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);

    if (bytes_received <= 0) // Client disconnected or error
    {
        std::cout << "Client disconnected: " << client_socket << "\n";
        close(client_socket);
    }
    else // Process the message
    {
        std::cout << "Received from client " << client_socket << ": " << buffer << "\n";

        // Respond to the client
        std::string response = "Message received: " + std::string(buffer);
        send(client_socket, response.c_str(), response.length(), 0);
    }
}

int Server::setup_server()
{
    int server_socket = initializeSocket();
    std::set<int> client_sockets; // Track all connected client sockets

    std::cout << "Server listening on port " << port << "...\n";

    fd_set active_fds, read_fds;
    FD_ZERO(&active_fds);
    FD_SET(server_socket, &active_fds);
    int max_fd = server_socket;

    while (true)
    {
        read_fds = active_fds; // Copy the active set for monitoring

        if (select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr) < 0)
        {
            perror("Select failed");
            break;
        }

        for (int fd = 0; fd <= max_fd; ++fd)
        {
            if (FD_ISSET(fd, &read_fds)) // Activity detected
            {
                if (fd == server_socket) // New client connection
                {
                    sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);

                    if (client_socket < 0)
                    {
                        perror("Accept failed");
                        continue;
                    }

                    std::cout << "New client connected: " << client_socket << "\n";
                    FD_SET(client_socket, &active_fds);
                    client_sockets.insert(client_socket);
                    if (client_socket > max_fd)
                        max_fd = client_socket;
                }
                else // Activity on an existing client
                {
                    handleClient(fd);
                    if (client_sockets.find(fd) != client_sockets.end())
                    {
                        // Client disconnected, remove from sets
                        FD_CLR(fd, &active_fds);
                        client_sockets.erase(fd);
                    }
                }
            }
        }
    }

    // Clean up on server exit
    for (int client_socket : client_sockets)
    {
        close(client_socket);
    }

    close(server_socket);
    return true;
}

}

