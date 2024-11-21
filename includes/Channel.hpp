#pragma once

#include <string>
#include <iostream>
#include <vector>

namespace irc
{

class Channel
{
    private:
        std::vector<std::string> users;
        std::vector<std::string> operators;
        std::string topic;
        std::string password;

    public:
        ~Channel();
        Channel();
        Channel(std::string _user);
        Channel(const Channel &copy);
        Channel &operator=(const Channel &src);

        // Getters
        const std::vector<std::string>& getUsers() const;
        const std::vector<std::string>& getOperators() const;
        const std::string& getTopic() const;
        const std::string& getPassword() const;

        // Setters
        void setTopic(const std::string& newTopic);
        void setPassword(const std::string& newPassword);    
        
};

}