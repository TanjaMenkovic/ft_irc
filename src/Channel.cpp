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

    Channel &Channel::operator=(Channel &src)
    {
        if (this != &src)
        {
            this->users = src.getUsers();            
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
    std::unordered_map<std::string, bool>& Channel::getUsers()
    {
        return (this->users);
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
    bool Channel::addUser(const User &user, bool is_operator)
    {
        if (users.size() >= static_cast<size_t>(user_limit) && user_limit != -1) {
            std::string message = "User limit reached!\r\n";
            send(user.getFd(), message.c_str(), message.length(), 0);
            return false;
        }
        if (users.find(user.getNickname()) != users.end()) {
            std::string message = " User is already in the channel!\r\n";
            send(user.getFd(), message.c_str(), message.length(), 0);
            return false;
        }
        if (is_operator) {
            users.insert({user.getNickname(), true});
            std::string message = "added user " + user.getNickname() + " to channel: " + name + "\r\n";
            send(user.getFd(), message.c_str(), message.length(), 0);
        } else {
            std::string message = "added user as non-operator to channel\r\n";
            users.insert({user.getNickname(), false});
        }
        return true;
    }
    
    bool Channel::removeUser(const User &user)
    {
        if (users.find(user.getNickname()) == users.end()) {
            std::string message = "User is not in channel!!\r\n";
            send(user.getFd(), message.c_str(), message.length(), 0);
        }
        users.erase(user.getNickname());
        return true;
    }
    
    // std::unordered_map<std::string, bool> users; 
    bool Channel::addOperator(const User &op_user, std::string &nick_to_promote, std::map<int, irc::User> &_users)
    {
        std::unordered_map<std::string, bool>::const_iterator found_op;
        found_op = this->getUsers().find(op_user.getNickname());
        // if operator user is found in channel users and is an operator
        if (found_op != this->getUsers().end() && found_op->second == true) {
            // check that user is part of channel
            std::unordered_map<std::string, bool>::iterator found_nick = this->getUsers().find(nick_to_promote);
            if (found_nick != this->getUsers().end()) {
                // broadcast message to all?
                found_nick->second = true;
                broadcastMessage(op_user, "user made operator message goes here!\r\n", _users);
            }
        }
        return true;
    }
    
    // Check if a user is an operator in the channel
    bool Channel::isOperator(const User &user) const
    {
        std::unordered_map<std::string, bool>::const_iterator found_user;
        found_user = this->users.find(user.getNickname());
        return (found_user != users.end() && found_user->second == true ? true : false);
    }

    // Broadcast a message to all users in the channel
    void Channel::broadcastMessage(const User &_user, const std::string &message, std::map<int, irc::User> &_users) const
    {
        // find all users that are in channel execpt user.client_fd
        // and send message to clients
        for (const std::pair<int, irc::User> &user: _users) {
            std::unordered_map<std::string, bool>::const_iterator found_user;
            // this->users contains all channel members, look for user in channel list
            found_user = this->users.find(user.second.getNickname());
            // if user was found, send message to user
            // need to add proper message syntax here
             // now message not broadcast to _user, maybe change this
            if (found_user != this->users.end() && user.second.getFd() != _user.getFd()) {
                send(user.second.getFd(), message.c_str(), message.length(), 0);
            }
        }
    }
}