#pragma once

#include <string>
#include <iostream>

namespace irc
{

class User 
{
    private:
        std::string nickname;
        std::string username;

    public:
        ~User();
        User();
        User(std::string nickname, std::string username);
        User(const User &copy);
        User &operator=(const User &src);

        std::string getUsername() const;
        std::string getNickname() const;
};

}