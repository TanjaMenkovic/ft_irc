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
 * ERR_INVALIDMODEPARAM(client, channel, mode, password) (":ft_irc 696 " + client + " #" + channel + " " + mode + " " + password + " : password must only contained alphabetic character\r\n")
 * 
 * 
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
    std::string channel = tokens[0]; // check later if this is even needed
    if (channel[0] != '#' && channel[0] != '&') {
        std::cerr << "Error: Invalid channel name '" << channel << "'. Channel names must start with '#' or '&'.\n";
        return ;
    } else {
        channel = channel.substr(1);
    }

    std::string message;

    if (tokens.size() < 2) {
        // just ignoring it (check this)
        return ;
    }

    for (size_t i = 1; i < tokens.size(); ++i) {
        const std::string& mode = tokens[i];


        if ((mode[0] == '+' || mode[0] == '-') && mode.length() == 1)
            return ; // ignore it (check this)

        if (mode[0] == '+') {
            if (mode[1] == 'i' && mode.length() == 2 && tokens.size() >= 2) {

            } else if (mode[1] == 't' && mode.length() == 2  && tokens.size() >= 2) {

            } else if (mode[1] == 'k' && mode.length() == 2  && tokens.size() >= 3) {

            } else if (mode[1] == 'o' && mode.length() == 2  && tokens.size() >= 3) {

            } else if (mode[1] == 'l' && mode.length() == 2  && tokens.size() >= 3) {

            } else { // incorrect mode
                message = 
            }
        } else if (mode[0] == '-') {
            if (mode[1] == 'i' && mode.length() == 2) {

            } else if (mode[1] == 't' && mode.length() == 2 && tokens.size() >= 2) {

            } else if (mode[1] == 'k' && mode.length() == 2 && tokens.size() >= 2) {

            } else if (mode[1] == 'o' && mode.length() == 2 && tokens.size() >= 3) {

            } else if (mode[1] == 'l' && mode.length() == 2 && tokens.size() >= 2) {

            } else { // incorrect mode

            }
        } else { // if it doesn't start with +/-
            message = ERR_NOSUCHNICK(users[client_fd].getNickname(), tokens[i]);
            send_to_user(client_fd, message);
            return ;
        } 


    }

    std::cout << "Finished processing MODE command for channel " << channel << std::endl;
}

void Server::channel_invite(int client_fd) {
    std::string message;

    message = MODE_USERMSG(nickname, username, channel_name, mode);

    send_to_
}

}