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
            this->topic = src.getTopic();             
            this->password = src.getPassword();  
            this->name = src.getName();
            this->user_limit = src.getUserLimit();
            this->invite_only = src.getInviteOnly();  
            this->topic_restricted_to_operators = src.getTopicRestricted();   
        }
        return *this;
    }

    User* Channel::getUserByNickname(const std::string &nickname) const {
        auto it = userptrs.find(nickname);
        if (it != userptrs.end()) {
            return it->second;  // Return the User* if found
        }
        return nullptr;  // Return nullptr if not found
    }

    bool Channel::hasUserStr(const std::string &nickname) const {
        return users.find(nickname) != users.end();
    }

    // Getters 
    const std::unordered_map<std::string, bool>& Channel::getUsers() const
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
        users.insert({user.getNickname(), false});
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
    
    bool Channel::addOperator(const User &user)
    {
        std::unordered_map<std::string, bool>::iterator found_user;
        found_user = this->users.find(user.getNickname()); 
        if (found_user == users.end()) {
            std::cout << "add error message for user not in channel\n";
        }
        // if in channel, make operator
        found_user->second = true;
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
    void Channel::broadcastMessage(const std::string &message, const User &sender) const
    {
        for (const std::pair<const std::string, bool> &user : users) {
            if (user.first != sender.getNickname()) {
                std::cout << "Sending message to: " << user.first << " from " << sender.getNickname() << ": " << message <<  std::endl;
            }
        }
    }


}