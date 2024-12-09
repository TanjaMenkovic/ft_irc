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

    bool User::getNickReceived() const 
    {
        return this->nick_received;
    }

    bool User::getUserReceived() const
    {
        return this->user_received;

    }

    bool User::getPassReceived() const
    {
        return this->pass_received;
    }

    bool User::getAuthenticated() const
    {
        return this->authenticated;
    }

    int User::getFd() const
    {
        return this->fd;
    }
    
    const std::map<std::string, bool>& User::getJoinedChannels() const
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

    void User::setNickReceived()
    {
        this->nick_received = true;
    }

    void User::setUserReceived()
    {
        this->user_received = true;
    }

    void User::setPassReceived()
    {
        this->pass_received = true;
    }

    void User::setAsAuthenticated()
    {
        this->authenticated = true;
    }

    void User::joinChannel(const std::string &channel_name, bool is_operator)
    {
        joined_channels[channel_name] = is_operator;
    }

    void User::leaveChannel(const std::string &channel_name)
    {
        joined_channels.erase(channel_name);
    }

    bool User::isInChannel(const std::string &channel_name)
    {
        if (joined_channels.find(channel_name) != joined_channels.end())
            return true;
        return false;
    }

    bool User::isOperator(const std::string &channel_name)
    {
        if (isInChannel(channel_name) && joined_channels[channel_name])
            return true;
        return false;
    }

    // void User::send_numeric_reply(int reply_code, const std::string &message, const std::string &server_name) {
    //     std::string formatted_reply = ":" + server_name + " " + std::to_string(reply_code) + " " + getNickname() + " :" + message + "\r\n";
    //     send(fd, formatted_reply.c_str(), formatted_reply.size(), 0);
    // }

}