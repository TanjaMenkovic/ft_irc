#include "../includes/Server.hpp"

std::atomic<bool> server_running(true);

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

void signal_handler(int signal) {
    if (signal == SIGINT || signal == SIGTERM || signal == SIGQUIT) {
        std::cout << "\nSignal " << signal << " received. Shutting down server...\n";
        server_running = false; // Set flag to false for clean shutdown
    }
}

int Server::setup_server() {
    // Register the signal handler
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    std::signal(SIGQUIT, signal_handler);

    int server_socket = create_socket();
    if (server_socket < 0) {
        return false;
    }

    if (!bind_and_listen(server_socket)) {
        return false;
    }

    std::cout << "Server listening on port " << port << "...\n";

    std::vector<pollfd> fds = {{server_socket, POLLIN, 0}};

    while (true) {
        if (!poll_connections(server_socket, fds)) {
            break;
        }
    }

    std::cout << "Server shutting down...\n";
    close(server_socket);
    return true;
}

int Server::create_socket() const {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Socket creation failed\n";
        return -1;
    }

    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    return server_socket;
}

bool Server::bind_and_listen(int server_socket) const {
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Listen on all available interfaces
    server_addr.sin_port = htons(port);        // Set the port number

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "bind failed\n";
        close(server_socket);
        return false;
    }

    if (listen(server_socket, 10) < 0) {
        perror("Listen failed");
        close(server_socket);
        std::cerr << "listen failed\n";
        return false;
    }

    return true;
}

bool Server::poll_connections(int server_socket, std::vector<pollfd>& fds) {
    while (server_running) {
        int poll_count = poll(fds.data(), fds.size(), -1);

        // Handle poll errors
        if (poll_count < 0) {
            if (errno == EINTR) {
                // Poll interrupted by signal, check shutdown_requested
                continue;
            }
            std::cerr << "Poll failed\n";
            return false;
        }

        // Process ready file descriptors
        for (size_t i = 0; i < fds.size(); ++i) {
            if (fds[i].revents & POLLIN) {
                if (fds[i].fd == server_socket) {
                    accept_new_client(server_socket, fds);
                } else {
                    int client_fd = fds[i].fd;
                    if (!process_client_input(client_fd)) {
                        close_client(client_fd, fds, i);
                        --i; // Adjust index as client is removed
                    }
                }
            }
        }
    }

    std::cout << "Shutdown requested, exiting poll loop.\n";
    return false;
}

void Server::accept_new_client(int server_socket, std::vector<pollfd>& fds) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
    if (client_socket < 0) {
        std::cerr << "accept failed\n";
        return;
    }

    std::cout << "New client connected.\n";
    std::string prompt = "Validating server password... ";
    send(client_socket, prompt.c_str(), prompt.size(), 0);

    fds.push_back({client_socket, POLLIN, 0});
    // Create a User instance and store it in the users map
    users[client_socket] = User(client_socket);
}

bool Server::process_client_input(int client_fd) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
        std::cout << "Client disconnected or error occurred.\n";
        return false;
    }

    // Handle potential IRC line endings and command prefixes
    std::string client_data(buffer);
    std::stringstream ss(client_data);
    std::string line;
   

    while (std::getline(ss, line)) {
        // Strip \r if it exists (for IRC compatibility)
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) {
            continue;  // Skip empty lines
        }

        if (users[client_fd].getAuthenticated() == false)
            authentication(client_fd, line);

        if (users[client_fd].getModeReceived() == false) {
            if (line.find("MODE ") == 0){
                send_mode_message(client_fd, users[client_fd].getNickname());
                this->users[client_fd].setModeReceived();
            }
        } else {
            parse_commands(client_fd, line);
        }
    }

    return true;
}

void Server::close_client(int client_fd, std::vector<pollfd>& fds, size_t index) {
    close(client_fd);
    fds.erase(fds.begin() + index);
}

}
