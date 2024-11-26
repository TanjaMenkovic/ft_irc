#pragma once

#include <string>
#include <iostream>
#include <vector>
#include "Channel.hpp"

class Channel;

namespace ircz
{

class User 
{
    private:
        std::string nickname;
        std::string username;
        // keep track of all channels user is in, so if nickname is changed, it is updated on all channels
        std::vector<Channel*> channels;
        // keep track of which channel user is currently typing in
        Channel* current_channel;

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