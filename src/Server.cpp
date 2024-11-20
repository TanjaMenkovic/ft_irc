#include "../includes/Server.hpp"

namespace irc
{

bool Server::validateClientPassword(const std::string &clientPassword) const {
    return clientPassword == password;
}

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

int Server::setup_server()
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
        return false;
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Bind failed");
        close(server_socket);
        return false;
    }

    if (listen(server_socket, 10) < 0)
    {
        perror("Listen failed");
        close(server_socket);
        return false;
    }

    std::cout << "Server listening on port " << port << "...\n";

   while (true)
{
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);

    if (client_socket < 0)
    {
        perror("Accept failed");
        continue;
    }

    std::cout << "Client connected.\n";
    std::string prompt = "Please enter the server password: ";
    send(client_socket, prompt.c_str(), prompt.size(), 0);

    char password_buffer[1024];
    memset(password_buffer, 0, sizeof(password_buffer));
    int bytes_received = recv(client_socket, password_buffer, sizeof(password_buffer) - 1, 0);

    if (bytes_received <= 0)
    {
        std::cerr << "Failed to receive password or client disconnected.\n";
        close(client_socket);
        continue;
    }

    std::string client_password(password_buffer);
    client_password.erase(client_password.find_last_not_of("\r\n") + 1); // Remove newline characters

    // Validate password
    if (!validateClientPassword(client_password))
    {
        std::string error_message = "ERROR: Incorrect password. Connection closed.\n";
        send(client_socket, error_message.c_str(), error_message.size(), 0);
        std::cerr << "Client provided an incorrect password. Connection terminated.\n";
        close(client_socket);
        continue;
    }

    std::string success_message = "Password accepted. Welcome to the server!\n";
    send(client_socket, success_message.c_str(), success_message.size(), 0);

    // Read messages from the client in a loop
    char buffer[1024];
    while (true)
    {
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

        if (bytes_received <= 0)
        {
            std::cout << "Client disconnected or error occurred.\n";
            break; // Exit the loop if the client disconnects or an error occurs
        }

        std::cout << "Received: " << buffer << "\n";

        // Send a response back to the client
        send(client_socket, "Message received\n", 17, 0);
    }

    close(client_socket); // Close the client socket after the client disconnects
}

    close(server_socket);

    return true;
}

}
