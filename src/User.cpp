#include "../includes/User.hpp"

namespace irc
{
    User::~User() {}

    User::User(int fd) : fd(fd), nickname("unknown"), username("unknown") {}

    User::User() : fd(-1), nickname(""), username("") {}

    User::User(std::string nickname, std::string username, int fd): fd(fd), nickname(nickname), username(username) {}

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
            this->fd = src.getFd();
            this->joined_channels = src.getJoinedChannels();
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

    int User::getFd() const
    {
        return this->fd;
    }
    
    const std::set<std::string>& User::getJoinedChannels() const
    {
        return this->joined_channels;
    }

    void User::setNickname(const std::string &nickname)
    {
        this->nickname = nickname;
    }

    void User::setUsername(const std::string &username)
    {
        this->username = username;
    }

    void User::joinChannel(const std::string &channel_name)
    {
        joined_channels.insert(channel_name);
    }

    void User::leaveChannel(const std::string &channel_name)
    {
        joined_channels.erase(channel_name);
    }

}