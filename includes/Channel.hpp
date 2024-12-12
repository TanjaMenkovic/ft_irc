#pragma once

#include <string>
#include <algorithm>
#include <vector>
#include "User.hpp"

/*
Class Channel containts:
- important data for each channel:
    - name - the most important one, connecting Channel class with other classes
    - invited_users - contains socket fd's of users that have received an invite to channel
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
        std::vector<int> invited_users;
        int user_limit;                             
        bool invite_only; // i                          
        bool topic_restricted_to_operators;  // t    

    public:
        ~Channel();
        Channel();
        Channel(const std::string &name);
        Channel(const Channel &copy);
        Channel &operator=(const Channel &src);

        const std::string& getName() const;
        const std::string& getTopic() const;
        const std::string& getPassword() const;
        int  getUserLimit() const;
        bool getInviteOnly() const;
        bool getTopicRestricted() const;
        bool getIsUserInvited(const int& fd) const;

        // Setters
        void setTopic(const std::string& newTopic);
        void setPassword(const std::string& newPassword);    
        void setUserLimit(int limit);
        void setInviteOnly(bool inviteOnly);
        void setTopicRestrictedToOperators(bool restricted);
        void addInvitedUser(const int& client_fd);
        void removeInvitedUser(const int& client_fd);
};

}