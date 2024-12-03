#pragma once

#include <string>
#include <iostream>
#include <set>
#include <map>
#include <unordered_map>
#include "User.hpp"

namespace irc
{

class Channel
{
    private:
        std::string topic;
        std::string password;
        std::string name;
        std::map<std::string, User*> userptrs;
        std::unordered_map<std::string, bool> users;           // Set of users' nicknames in the channel            // Set of operators' nicknames (channel admins)
        int user_limit;                             // Maximum number of users allowed in the channel (-1 for unlimited)
        bool invite_only;                           // Whether the channel is invite-only
        bool topic_restricted_to_operators;         // Whether changing the topic is restricted to operators only

    public:
        ~Channel();
        Channel();
        Channel(const std::string &name);
        Channel(const Channel &copy);
        Channel &operator=(const Channel &src);

        // Getters
        const std::unordered_map<std::string, bool>& getUsers() const;
        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::string& getPassword() const;
        User* getUserByNickname(const std::string &nickname) const;
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
        
        bool addUser(const User &user);
        bool removeUser(const User &user);
        bool hasUser(const User &user) const;
        bool hasUserStr(const std::string &nickname) const;;         // added for kick command to find user from channel only with given string
        bool addOperator(const User &user);
        bool isOperator(const User &user) const;
        void broadcastMessage(const std::string &message, const User &sender) const;
};

}