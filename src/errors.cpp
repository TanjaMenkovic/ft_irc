#include "../includes/irc.hpp"

/*
Ports are 16-bit unsigned integers, so the range is:
0 - 65535
However, in practice:
System-reserved ports (0–1023) are typically not available for custom use 
(unless you have root/administrator privileges).
It's recommended to use ports 1024–49151 for custom servers.
The remainder of the ports from 49152 to 65535 can be used dynamically by applications.
Dynamic ports, also known as private or ephemeral ports, include ports 49152 to 65535 
and are never assigned.
*/

bool isValidPort(const std::string& str) 
{
    if (str.empty()) 
        return false;

    for (size_t i = 0; i < str.length(); i++) 
    {
        if (!std::isdigit(str[i]))
            return false;
    }

    if ((str.length() > 1 && str[0] == '0') || str.length() > 5)
        return false;

    if (std::stoi(str) < 1024 || std::stoi(str) > 65535) // <-- SHOULD WE CHANGE TO 49151?
        return false;
    return true;
}

/*
Passward is not allowed to have white spaces in it
*/
static bool has_whitespace(const std::string& str) {
    return std::any_of(str.begin(), str.end(), ::isspace);
}

bool isValidPassword(const std::string& str)
{
    if (str.empty() || has_whitespace(str))
        return false;
    
    return true;
}