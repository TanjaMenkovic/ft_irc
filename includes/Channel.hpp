#pragma once

#include <string>
#include <iostream>
#include <set>
#include <map>
#include "User.hpp"

namespace irc
{

class Channel
{
    private:
        std::string topic;
        std::string password;
        std::string name;
        std::set<std::string> users;                // Set of users' nicknames in the channel
        std::set<std::string> operators;            // Set of operators' nicknames (channel admins)
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
        const std::set<std::string>& getUsers() const;
        const std::set<std::string>& getOperators() const;
        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::string& getPassword() const;
        int getUserLimit() const;
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
        bool addOperator(const User &user);
        bool removeOperator(const User &user);
        bool isOperator(const User &user) const;
        void broadcastMessage(const std::string &message, const User &sender) const;
};

}