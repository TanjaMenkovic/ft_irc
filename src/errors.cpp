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