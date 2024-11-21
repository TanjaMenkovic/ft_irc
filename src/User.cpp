#include "../includes/User.hpp"

namespace irc
{
    User::~User() {}

    User::User(): nickname("unknown"), username("unknown") {}

    User::User(std::string nickname, std::string username): nickname(nickname), username(username) {}

    User::User(const User &copy)
    {
        *this = copy;
    }

    User &User::operator=(const User &src)
    {
        if (this != &src)
        {
            this->nickname = src.getNickname();
            this->username = src.getUsername();
        }
        return *this;
    }

    std::string User::getUsername() const
    {
        return this->username;
    }

    std::string User::getNickname() const
    {
        return this->nickname;
    }

}