#include "../includes/Channel.hpp"

namespace irc
{
    Channel::~Channel() {}

    Channel::Channel() 
    : topic(""), password(""), name(""), user_limit(-1), invite_only(false), topic_restricted_to_operators(false) {}

    Channel::Channel(const std::string &name) 
    : topic(""), password(""), name(name), user_limit(-1), invite_only(false), topic_restricted_to_operators(false) {}

    Channel::Channel(const Channel &copy)
    {
        *this = copy;
    }

    Channel &Channel::operator=(const Channel &src)
    {
        if (this != &src)
        {
            this->users = src.getUsers();            
            this->operators = src.getOperators();     
            this->topic = src.getTopic();             
            this->password = src.getPassword();  
            this->name = src.getName();
            this->user_limit = src.getUserLimit();
            this->invite_only = src.getInviteOnly();  
            this->topic_restricted_to_operators = src.getTopicRestricted();   
        }
        return *this;
    }

    // Getters 
    const std::set<std::string>& Channel::getUsers() const
    {
        return this->users;
    }

    const std::set<std::string>& Channel::getOperators() const
    {
        return this->operators;
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

    // functions
    bool Channel::addUser(const User &user)
    {
        if (users.size() >= static_cast<size_t>(user_limit) && user_limit != -1) {
            std::cout << "User limit reached!" << std::endl;
            return false;
        }
        if (users.find(user.getNickname()) != users.end()) {
            std::cout << " User is already in the channel!" << std::endl;
            return false;
        }
        users.insert(user.getNickname());
        return true;
    }
    
    bool Channel::removeUser(const User &user)
    {
        if (users.find(user.getNickname()) == users.end()) {
            std::cout << "User is not in the channel!" << std::endl;
            return false; 
        }
        users.erase(user.getNickname());
        return true;
    }
    
    bool Channel::hasUser(const User &user) const
    {
        return users.find(user.getNickname()) != users.end();
    }
    
    bool Channel::addOperator(const User &user)
    {
        if (!hasUser(user)) {
            return false;  // User must be part of the channel to be an operator
        }
        operators.insert(user.getNickname());
        return true;
    }
    
    bool Channel::removeOperator(const User &user)
    {
        if (operators.find(user.getNickname()) == operators.end()) {
            std::cout << "Operator is not in the channel!" << std::endl;
            return false; 
        }
        operators.erase(user.getNickname());
        return true;
    }
    
    // Check if a user is an operator in the channel
    bool Channel::isOperator(const User &user) const
    {
        return operators.find(user.getNickname()) != operators.end();
    }

    // Broadcast a message to all users in the channel
    void Channel::broadcastMessage(const std::string &message, const User &sender) const
    {
        for (const std::string &user_nick : users) {
            if (user_nick != sender.getNickname()) {
                std::cout << "Sending message to: " << user_nick << " from " << sender.getNickname() << ": " << message <<  std::endl;
            }
        }
    }


}