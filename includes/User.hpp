#pragma once

#include <string>
#include <iostream>
#include <set>
#include <sys/socket.h>

class Channel;

namespace irc
{

class User 
{
    private:
        int fd; // The file descriptor of the socket that represents the connection to the user
        std::string nickname;
        std::string username;
        std::set<std::string> joined_channels;  // Set of channel names the user has joined
        bool authenticated = false;
        bool nick_received = false;
        bool user_received = false;
        bool pass_received = false;

    public:
        ~User();
        User(int fd);
        User();
        User(std::string nickname, std::string username, int fd);
        User(const User &copy);
        User &operator=(const User &src);

        std::string getUsername() const;
        std::string getNickname() const;
        bool getNickReceived() const;
        bool getUserReceived() const;
        bool getPassReceived() const;
        bool getAuthenticated() const;
        int  getFd() const;
        const std::set<std::string>& getJoinedChannels() const;

        void setNickname(const std::string &nickname);
        void setUsername(const std::string &username);
        void setNickReceived();
        void setUserReceived();
        void setPassReceived();
        void setAsAuthenticated();

        void joinChannel(const std::string &channel_name);
        void leaveChannel(const std::string &channel_name);

        void send_numeric_reply(int reply_code, const std::string &message, const std::string &server_name);
};

}