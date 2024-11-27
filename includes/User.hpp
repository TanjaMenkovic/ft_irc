#pragma once

#include <string>
#include <iostream>
#include <set>

namespace irc
{

class User 
{
    private:
        int fd; // The file descriptor of the socket that represents the connection to the user
        std::string nickname;
        std::string username;
        std::set<std::string> joined_channels;  // Set of channel names the user has joined

    public:
        ~User();
        User(int fd);
        User();
        User(std::string nickname, std::string username, int fd);
        User(const User &copy);
        User &operator=(const User &src);

        std::string getUsername() const;
        std::string getNickname() const;
        int getFd() const;
        const std::set<std::string>& getJoinedChannels() const;

        void setNickname(const std::string &nickname);
        void setUsername(const std::string &username);

        void joinChannel(const std::string &channel_name);
        void leaveChannel(const std::string &channel_name);
};

}