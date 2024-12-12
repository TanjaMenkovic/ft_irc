#include "../includes/Channel.hpp"

namespace irc
{
    Channel::~Channel() {}

    Channel::Channel() 
    : topic(""), password(""), name(""), user_limit(-1), invite_only(false), topic_restricted_to_operators(true) {}

    Channel::Channel(const std::string &name) 
    : topic(""), password(""), name(name), user_limit(-1), invite_only(false), topic_restricted_to_operators(true) {}

    Channel::Channel(const Channel &copy)
    {
        *this = copy;
    }

    Channel &Channel::operator=(const Channel &src)
    {
        if (this != &src)
        {
            this->topic = src.getTopic();             
            this->password = src.getPassword();  
            this->name = src.getName();
            this->user_limit = src.getUserLimit();
            this->invite_only = src.getInviteOnly();  
            this->topic_restricted_to_operators = src.getTopicRestricted();   
        }
        return *this;
    }

    const std::string& Channel::getTopic() const
    {
        return this->topic;
    }

    const std::string& Channel::getPassword() const
    {
        return this->password;
    }

    const std::string& Channel::getName() const
    {
        return this->name;
    }

    int Channel::getUserLimit() const
    {
        return this->user_limit;
    }
        
    bool Channel::getInviteOnly() const
    {
        return this->invite_only;
    }
    
    bool Channel::getTopicRestricted() const
    {
        return this->topic_restricted_to_operators;
    }

    bool Channel::getIsUserInvited(const int& fd) const
    {
        return (find(invited_users.begin(), invited_users.end(), fd) != invited_users.end());
    }
    

    // Setters 
    void Channel::setTopic(const std::string& newTopic)
    {
        this->topic = newTopic;
    }

    void Channel::setPassword(const std::string& newPassword)
    {
        this->password = newPassword;
    }

    void Channel::setUserLimit(int limit)
    {
        this->user_limit = limit;
    }
    
    void Channel::setInviteOnly(bool inviteOnly)
    {
        this->invite_only = inviteOnly;
    }
    
    void Channel::setTopicRestrictedToOperators(bool restricted)
    {
        this->topic_restricted_to_operators = restricted;
    }

    void Channel::addInvitedUser(const int& client_fd) 
    {
        if (find(invited_users.begin(), invited_users.end(), client_fd) == invited_users.end()) {
            this->invited_users.push_back(client_fd);
        }
    }

    void Channel::removeInvitedUser(const int& client_fd)
    {
        std::vector<int>::iterator it = find(invited_users.begin(), invited_users.end(), client_fd);
        if (it != invited_users.end()) {
            invited_users.erase(it);
        }
    }

}