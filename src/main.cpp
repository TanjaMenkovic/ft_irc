#include "../includes/irc.hpp"

bool isValidPort(const std::string& str) 
{
    if (str.empty()) 
        return false;

    for (size_t i = 0; i < str.length(); i++) 
    {
        if (!std::isdigit(str[i]))
            return false;
    }

    if (str.length() > 1 && str[0] == '0')
        return false;

    if (std::stoi(str) < 0 || std::stoi(str) > 65535)
        return false;
    return true;
}

bool isValidPassword(const std::string& str)
{
    if (str.empty())
        return false;
    return true;
}

int setup_server(irc::Server server)
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
    server_addr.sin_port = htons(server.getPort());

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

    std::cout << "Server listening on port " << server.getPort() << "...\n";

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

int main(int argc, char **argv) 
{
    if (argc != 3)
    {
        //error
        std::cerr << "Incorrect number of values!" << std::endl;
        return EXIT_FAILURE;
    }

    if (!isValidPort(argv[1]))
    {
        //error
        std::cerr << "Port is incorrect!" << std::endl;
        return EXIT_FAILURE;
    }

    if (!isValidPassword(argv[2]))
    {
        //error
        std::cerr << "Password is incorrect!" << std::endl;
        return EXIT_FAILURE;
    }

    int input;
    try {
    input = std::stoi(argv[1]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid argument provided for port. It must be a number.\n";
        return 1; 
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Port number out of range.\n";
        return 1; 
    } catch (...) {
        std::cerr << "Error: An unknown error occurred while parsing the port.\n";
        return 1; 
    }

    std::cout << "......Server starting on port " << input << "......" << std::endl;
    irc::Server server(input, argv[2]);

    if (setup_server(server) == false)
    {
        //error
        std::cout << "Server setup failed!" << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}