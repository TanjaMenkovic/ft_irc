// private:
//         std::vector<std::string> users;
//         std::vector<std::string> operators;
//         std::string topic;
//         std::string password;

//     public:
//         ~Channel();
//         Channel();
//         Channel(std::vector<std::string> users, std::vector<std::string> operators);
//         Channel(const Channel &copy);
//         Channel &operator=(const Channel &src);


#include "../includes/Channel.hpp"

namespace irc
{
    Channel::~Channel() {}

    Channel::Channel(): topic(""), password("") {}

    Channel::Channel(std::string _user): topic(""), password("") 
    {
        users.push_back(_user);
        operators.push_back(_user);
    }

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
        }
        return *this;
    }

    // Getters 
    const std::vector<std::string>& Channel::getUsers() const
    {
        return users;
    }

    const std::vector<std::string>& Channel::getOperators() const
    {
        return operators;
    }

    const std::string& Channel::getTopic() const
    {
        return topic;
    }

    const std::string& Channel::getPassword() const
    {
        return password;
    }

    // Setters 
    void Channel::setTopic(const std::string& newTopic)
    {
        topic = newTopic;
    }

    void Channel::setPassword(const std::string& newPassword)
    {
        password = newPassword;
    }

}