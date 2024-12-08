#pragma once

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <set>
#include <map>
#include <unordered_map>
#include "User.hpp"

/*
Class Channel containts:
- important data for each channel:
    - name - the most important one, connecting Channel class with other classes
    - topic
    - password - the channel key (password)
    - user_limit - Maximum number of users allowed in the channel (-1 for unlimited)
    - invite_only -  Whether the channel is invite-only
    - topic_restricted_to_operators - Whether changing the topic is restricted to operators only
- constructors and destructor
- getters and setters
*/

namespace irc
{

class Channel
{
    private:
        std::string topic;
        std::string password;
        std::string name;
        // std::map<std::string, User*> userptrs;
        // std::unordered_map<std::string, bool> users;           // Set of users' nicknames in the channel            // Set of operators' nicknames (channel admins)
        int user_limit;                             
        bool invite_only;                           
        bool topic_restricted_to_operators;         

    public:
        ~Channel();
        Channel();
        Channel(const std::string &name);
        Channel(const Channel &copy);
        Channel &operator=(const Channel &src);

        // Getters
        // const std::unordered_map<std::string, bool>& getUsers() const;
        // std::string getChannelNicks();
        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::string& getPassword() const;
        // User* getUserByNickname(const std::string &nickname) const;
        int  getUserLimit() const;
        bool getInviteOnly() const;
        bool getTopicRestricted() const;

        // Setters
        void setTopic(const std::string& newTopic);
        void setPassword(const std::string& newPassword);    
        void setUserLimit(int limit);
        void setInviteOnly(bool inviteOnly);
        void setTopicRestrictedToOperators(bool restricted);

        // functions
        
        // bool addUser(const User &user, bool is_operator);
        // bool removeUser(const User &user);
        // bool addOperator(const User &op_user, std::string &nick_to_promote, std::map<int, irc::User> &_users);
        // bool isOperator(const User &user) const;
        // void broadcastMessage(const User &user, const std::string &message, const std::map<int, irc::User> &users) const;
};

}