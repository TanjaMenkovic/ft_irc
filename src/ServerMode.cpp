#include "../includes/Server.hpp"

namespace irc 
{

/*
Syntax: MODE <modestring> [<mode arguments>...]
- <modestring> starts with a plus ('+', 0x2B) or minus ('-', 0x2D) character, 
and is made up of the following characters:
· '+': Adds the following mode, 
· '-': Removes the following mode.
- 'a-zA-Z': Mode letters, indicating which modes are to be added/removed.
We are supposed to handle:
· i: Set/remove Invite-only channel
· t: Set/remove the restrictions of the TOPIC command to channel
operators
· k: Set/remove the channel key (password)
· o: Give/take channel operator privilege
· l: Set/remove the user limit to channel 
*/

void Server::handle_mode(int client_fd, std::vector<std::string> tokens)
{  
    std::string channel = tokens[0]; 

    if (channel[0] != '#' && channel[0] != '&') { // <-- check later if this is even needed
        std::cerr << "Error: Invalid channel name '" << channel << "'. Channel names must start with '#' or '&'.\n";
        return ;
    }
    //std::string channel_name = channel.substr(1);
    std::cout << "\nChannel name: " << channel << std::endl;
    if (users[client_fd].isInChannel(channel) == false) { // <-- check later if this is even needed
        std::cout << "User is not in the channel!" << std::endl;
        return ;
    }

    std::string message;

    if (tokens.size() < 2) {
        // just ignoring it (check this)
        std::cout << "Mode doesn't have any arguments!" << std::endl;
        return ;
    }

    // check separately when it is not starting with +/-
    // if I type /mode i, irssi will not send #channal_name, so it will treat "i" as a channel nama
    // and go to the first error for invalid channal_name

    const std::string& mode = tokens[1];
    if ((mode[0] == '+' || mode[0] == '-') && mode.length() == 1)
        return ; // ignore it (check this)

    if (mode[0] == '+' && mode.length() == 2) {
        if (check_if_operator(client_fd, channel) == false) {
            return ;
        }
        if (mode[1] == 'i' && tokens.size() >= 2) {
            channel_invite(client_fd, channel, "+i");
        } else if (mode[1] == 't' && tokens.size() >= 2) {
            channel_topic(client_fd, channel, "+t");
        } else if (mode[1] == 'k' && tokens.size() >= 3) {
            channel_password(client_fd, channel, "+k", tokens[2]);
        } else if (mode[1] == 'o' && tokens.size() >= 3) {
            channel_user(client_fd, channel, "+o", tokens[2]);
        } else if (mode[1] == 'l' && tokens.size() >= 3) {
            channel_limit(client_fd, channel, "+l", tokens[2]);
        } else { // incorrect mode
            message = ERR_UNKNOWNMODE(users[client_fd].getNickname(), mode);
            send_to_user(client_fd, message);
            return ;
        }
    } else if (mode[0] == '-' && mode.length() == 2) {
        if (check_if_operator(client_fd, channel) == false) {
            return ;
        }
        if (mode[1] == 'i' && tokens.size() >= 2) {
            channel_invite(client_fd, channel, "-i");
        } else if (mode[1] == 't' && tokens.size() >= 2) {
            channel_topic(client_fd, channel, "-t");
        } else if (mode[1] == 'k' && tokens.size() >= 2) {
            channel_password(client_fd, channel, "-k", "");
        } else if (mode[1] == 'o' && tokens.size() >= 3) {
            channel_user(client_fd, channel, "-o", tokens[2]);
        } else if (mode[1] == 'l' && tokens.size() >= 2) {
            channel_limit(client_fd, channel, "-l", "");
        } else { // incorrect mode
            message = ERR_UNKNOWNMODE(users[client_fd].getNickname(), mode);
            send_to_user(client_fd, message);
            return ;
        }
    } else { // if it doesn't start with +/-
        message = ERR_NOSUCHNICK(users[client_fd].getNickname(), mode);
        send_to_user(client_fd, message);
        return ;
    } 


    std::cout << "Finished processing MODE command for channel " << channel << std::endl;
}

bool Server::check_if_operator(int client_fd, std::string channel_name) {
    if (users[client_fd].GetOperator(channel_name) == false) {
        std::string message = ERR_CHANOPRIVSNEEDED(users[client_fd].getNickname(), channel_name);
        send_to_user(client_fd, message);
        return false;
    }
    return true;
}

void Server::channel_invite(int client_fd, std::string channel_name, std::string mode) {
    std::string message;

    message = MODE_USERMSG(users[client_fd].getNickname(), users[client_fd].getUsername(), channel_name, mode);
    send_to_joined_channels(client_fd, message);

    if (mode == "+i") {
        channels[channel_name].setInviteOnly(true);
    } else {
        channels[channel_name].setInviteOnly(false);
    }
}

void Server::channel_topic(int client_fd, std::string channel_name, std::string mode) {
    std::string message;

    message = MODE_USERMSG(users[client_fd].getNickname(), users[client_fd].getUsername(), channel_name, mode);
    send_to_joined_channels(client_fd, message);

    if (mode == "+t") {
        channels[channel_name].setTopicRestrictedToOperators(true);
    } else {
        channels[channel_name].setTopicRestrictedToOperators(false);
    }
}

void Server::channel_password(int client_fd, std::string channel_name, std::string mode, std::string password) {
    std::string message;

    if (mode == "+k") {
        channels[channel_name].setPassword(password);
        mode += " " + password;
    } else {
        channels[channel_name].setPassword("");
    }
    
    message = MODE_USERMSG(users[client_fd].getNickname(), users[client_fd].getUsername(), channel_name, mode);
    send_to_joined_channels(client_fd, message);
}

bool Server::is_in_channel(std::string user_nickname, std::string channel_name, bool is_operator) {
    for (auto it : this->users) {
        if (it.second.getNickname() == user_nickname && it.second.isInChannel(channel_name)) {
            it.second.SetOperator(channel_name, is_operator);
            return true;
        }
    }
    return false;
}

void Server::channel_user(int client_fd, std::string channel_name, std::string mode, std::string user_nickname) {
    std::string message;
    bool is_operator;

    if (mode == "+o") {
        is_operator = true;
    } else {
        is_operator = false;
    }

    if (is_in_channel(user_nickname, channel_name, is_operator) == false) {
        message = ERR_NOSUCHNICK(user_nickname, mode);
        send_to_user(client_fd, message);
        return ;
    }

    mode += " " + user_nickname;
    message = MODE_USERMSG(users[client_fd].getNickname(), users[client_fd].getUsername(), channel_name, mode);
    send_to_joined_channels(client_fd, message);
}

static bool isValidLimit(const std::string& str) 
{
    if (str.empty()) 
        return false;

    for (size_t i = 0; i < str.length(); i++) 
    {
        if (!std::isdigit(str[i]))
            return false;
    }

    if (str.length() > 1 && str[0] == '0')
        return false;

    int str_int = std::stoi(str);
    if (str != std::to_string(str_int))
        return false;

    return true;
}

void Server::channel_limit(int client_fd, std::string channel_name, std::string mode, std::string limit_str) {
    std::string message;
    int limit;

    if (isValidLimit(limit_str) == false) {
        message = ERR_INVALIDMODEPARAM(users[client_fd].getNickname(), channel_name, mode, limit_str);
        send_to_user(client_fd, message);
        return ;
    }

    limit = std::stoi(limit_str);
    if (mode == "+l") {
        channels[channel_name].setUserLimit(limit);
        mode += " " + std::to_string(limit);
    } else {
        channels[channel_name].setUserLimit(-1);
    }
    
    message = MODE_USERMSG(users[client_fd].getNickname(), users[client_fd].getUsername(), channel_name, mode);
    send_to_joined_channels(client_fd, message);
}

}