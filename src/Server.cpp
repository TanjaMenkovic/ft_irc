#include "../includes/Server.hpp"

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

    /*
    signal(SIGINT, [](int signum)
    {
        //close(server_socket);
        // try to close the server somehow
        std::cout << "\nServer closed.\n";
        exit(0);
    };*/
    
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

        char buffer[1024] = {0};
        recv(client_socket, buffer, sizeof(buffer), 0);
        std::cout << "Received: " << buffer << "\n";

        send(client_socket, "Welcome to the IRC server\n", 27, 0);
        close(client_socket);
    }

    close(server_socket);

    return true;
}

}
