#include "../includes/irc.hpp"

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
        //errorr
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

    if (server.setup_server() == false)
    {
        //error
        std::cout << "Server setup failed!" << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}