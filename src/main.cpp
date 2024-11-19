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

    return 0;
}