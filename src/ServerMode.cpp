#include "../includes/Server.hpp"

namespace irc 
{

/**
 * @brief The MODE command is used to set or remove options (or modes) from a given target.
 * 
 * Parameters: <target> [<modestring> [<mode arguments>...]]
 * 
 * <modestring> starts with a plus ('+', 0x2B) or minus ('-', 0x2D) character, and is made up of the following characters:
 * '+': Adds the following mode(s).
 * '-': Removes the following mode(s).
 * 'a-zA-Z': Mode letters, indicating which modes are to be added/removed.
 * 
 * User case :
 * If <modestring> is given, the supplied modes will be applied : a MODE message will be sent to the user containing the changed modes. 
 * If one or more modes sent are not implemented on the server, 
 * the server MUST apply the modes that are implemented, and then send the ERR_UMODEUNKNOWNFLAG (501) in reply along with the MODE message.
 * 
 * Channel case :
 * syntax command : /mode <channel> <+ ou -> <mode> [parametres]
 * If the user has permission to change modes on the target, the supplied modes will be applied based on the type of the mode (see below). 
 * For type A, B, and C modes, arguments will be sequentially obtained from <mode arguments>. 
 * - If a type B or C mode does not have a parameter when being set, the server MUST ignore that mode. 
 * - If a type A mode has been sent without an argument : 
 * the contents of the list MUST be sent to the user, 
 * unless it contains sensitive information the user is not allowed to access. 
 * When the server is done processing the modes, a MODE command is sent to all members of the channel containing the mode changes. 
 * Servers MAY choose to hide sensitive information when sending the mode changes.
 * 
 * Exemples :
 * - +o : /mode #cool +o MEAT (MEAT devient opérateur sur #cool)
 * Message à send aux users du channel : ':irc.example.com MODE #cool +o MEAT'; 
 * The irc.example.com server gave channel operator privileges to MEAT on #cool.
 * - +k : /mode #cool +k COOLKEY (protège le channel par le mot de passe COOLKEY)
 * - +s : /mode #cool +s (le channel devient secret)
 * - plusieur modes : /mode #cool +ts (le channel est en mode +t +s)
 * 
 * @param server
 * @param client_fd User sending a msg
 * @param cmd_infos Structure w/ prefix, command name and message
 * 
 * Useful link : https://modern.ircdocs.horse/#mode-message
 * https://www.techbull.com/techbull/guide/internet/irccommande.html
 * 
 * 
 * 
 * 
 * RPL_CHANNELMODEIS(nickname, channel_name, modes) (":ft_irc 324 " + nickname + " #" + channel_name + " +" + modes + "\r\n")
 * ERR_NOSUCHNICK(nickname, target) (":ft_irc 401 " + nickname + " " + target + " :No such nick\r\n")
 * MODE_USERMSG(nickname, username, channel_name, mode) (":" + nickname + "!~" + username + "@ft_irc" + " MODE " + channel_name + " :" + mode + "\r\n")
 * ERR_INVALIDMODEPARAM(nickname, channel_name, mode, param) (":ft_irc 696 " + nickname + " #" + channel_name + " " + mode + " " + param + " : invalid parameter\r\n")
 * ERR_UNKNOWNMODE(nickname, mode) (":ft_irc 472 " + nickname + " " + mode + " :is not a recognised channel mode.\r\n")
 * ERR_CHANOPRIVSNEEDED(nickname, channel_name) (":ft_irc 482 " + nickname + " #" +  channel_name + " :You're not channel operator\r\n")
 * 
 * when I just type mode without any arg I am getting:
 * :*.freenode.net 324 tmenkovi #bla :+nt <- NO!
 * 
 * when I try incorrect mode I am getting:
 * :*.freenode.net 401 tmenkovi fsfs :No such nick
 * 
 * when I try mode without a sign:
 * 
 * when I change +i everyone is getting
 * :tmenkovi!~tmenkovi@freenode-ebn.s3h.4nuk5f.IP MODE #bla :+i
 * 
 * when I change -t, other users are getting:
 * :tmenkovi!~tmenkovi@freenode-ebn.s3h.4nuk5f.IP MODE #bla :-t
 * 
 * if I'm not an operator and I try to change k I get
 * :*.freenode.net 482 tmenkovi_ #bla :You must have channel halfop access or above to set channel mode k
 * otherwise everyone is getting:
 * :tmenkovi!~tmenkovi@freenode-ebn.s3h.4nuk5f.IP MODE #bla +k :pass
 * 
 * when I try incorrect user I get:
 * MODE #bla +o user
 * :*.freenode.net 401 tmenkovi user :No such nick
 * 
 * when I try l without parameter:
 * MODE #bla +l
 * >> :*.freenode.net 696 tmenkovi #bla l * :You must specify a parameter for the limit mode. Syntax: <limit>.
 * 
 * when I try to change l and I'm not operator:
 * MODE #bla +l 10
 * >> :*.freenode.net 482 tmenkovi_ #bla :You must have channel halfop access or above to set channel mode l
 * 
 * when I change l everyone else is getting:
 * :tmenkovi!~tmenkovi@freenode-ebn.s3h.4nuk5f.IP MODE #bla +l :10
 * 
 */

void Server::handle_mode(int client_fd, std::vector<std::string> tokens)
{  
    std::string channel = tokens[0]; 

    // if (channel[0] != '#' && channel[0] != '&') { // check later if this is even needed
    //     std::cerr << "Error: Invalid channel name '" << channel << "'. Channel names must start with '#' or '&'.\n";
    //     return ;
    // }
    //std::string channel_name = channel.substr(1);
    std::cout << "\nChannel name: " << channel << std::endl;
    if (users[client_fd].isInChannel(channel) == false) {
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
            channel_topic(client_fd, channel, "+t");
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