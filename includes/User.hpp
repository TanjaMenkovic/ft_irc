#pragma once

#include <string>
#include <map>

/*
Class User containts:
- important data for each user:
    - fd - The file descriptor of the socket that represents the connection to the user
    - nickname
    - username
    - joined_channels - map of channel names the user has joined and is the user operator of each channel
- variables for authenticating the user when connecting to the server:
    - authenticated - true if user received nickname, username and passward
    - nick_received - did the user received the nickname when connected
    - user_received - did the user received the username
    - pass_received - did the user received the password
- constructors and destructor
- getters and setters
- methods:
    - joinChannel - method for adding a name of the channel into joined_channel set
    - leaveChannel - method for erasing channel name from joined_channel set
    - isInChannel - method for checking if channel name is inside od the set
*/

class Channel;

namespace irc
{

class User 
{
    private:
        int fd;
        std::string nickname;
        std::string username;
        std::map<std::string, bool> joined_channels;
        bool authenticated;
        bool nick_received;
        bool user_received;
        bool pass_received;
        bool mode_received;

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
        bool getModeReceived() const;
        bool getAuthenticated() const;
        int  getFd() const;
        const std::map<std::string, bool>& getJoinedChannels() const;

        void setNickname(const std::string &nickname);
        void setUsername(const std::string &username);
        void setNickReceived();
        void setUserReceived();
        void setPassReceived();
        void setModeReceived();
        void setAsAuthenticated();

        void joinChannel(const std::string &channel_name, bool is_operator);
        void leaveChannel(const std::string &channel_name);

        bool isInChannel(const std::string &channel_name);
        bool isOperator(const std::string &channel_name);

        bool GetOperator(std::string channel_name);
        void SetOperator(std::string channel_name, bool is_operator);

        // void send_numeric_reply(int reply_code, const std::string &message, const std::string &server_name);
};

}