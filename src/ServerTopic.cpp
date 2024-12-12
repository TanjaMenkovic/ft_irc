#include "../includes/Server.hpp"

namespace irc 
{

/*      Command: TOPIC
   Parameters: <channel> [<topic>]

   The TOPIC message is used to change or view the topic of a channel.
   The topic for channel <channel> is returned if there is no <topic>
   given.  If the <topic> parameter is present, the topic for that
   channel will be changed, if the channel modes permit this action.

   Numeric Replies:

           ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
           RPL_NOTOPIC                     RPL_TOPIC
           ERR_CHANOPRIVSNEEDED            ERR_NOSUCHCHANNEL
*/

// If client uses the topic command while connected to an server but it hasnt joined any channels, the command needs to be provided
// with the channel's name in form of #<channelname> in order to return a topic of specific channel.

// First check if the topic command is used inside a channel, if it is then it can be ran without parameters
// and it will just return the current topic of the channel if there is any.

// User has to be an operator of the channel in order to change its topic. 
// If user is not an operator of the channel it tries to change topic in, messages in following format will be displayed:
// #channel1 You do not have access to change the topic on this channel

// Tested on irc.freenode.net
// command /topic lol given on a channel results in client sending:
// << TOPIC #channel1 :lol
// server responded with:
// >> :tvalimak_!~tvalimak@freenode-ebn.s3h.4nuk5f.IP TOPIC #channel1 :lol


// /mode +t would make only operators be able to change topic
// channel class has bool variable topic_restricted_to_operators
// if it's true only operators can change
// else anyone can

// when inside #channel1 and command /topic asd is used, other client that is not insde that
// channel gets the topic in format: " Topic for asd: " when it gives command /topic #channel1

void Server::topic(int client_fd, std::vector<std::string> tokens) {
	std::string channel_name;
	std::string topic;
	std::string message;

    std::cout << "inside topic command with user " << users[client_fd].getNickname() << "\n";

    // first check if the channel exists if channelname param is provided
    if (channels.find(tokens[0]) == channels.end()) {
        std::cout << "ERR_NOSUCHCHANNEL\n";
        message = ERR_NOSUCHCHANNEL(users[client_fd].getNickname(), tokens[0]);
        send_to_user(client_fd, message);
        return ;
    }
    std::cout << "Inside topic 0\n";
    // first if block is to check if the command is provided with channelname but no other params,
    // in this case we just want to return the topic of the given channel if it exists
    if (tokens[0].at(0) == '#' && tokens.size() == 1) {
        // get the topic of the channel
        topic = channels[tokens[0]].getTopic();
        std::cout << topic << "\n";
        if (topic.empty()) {
            message = RPL_NOTOPIC(users[client_fd].getNickname(), tokens[0]);
            send_to_user(client_fd, message);
            return ;
            }
        channel_name = channels[tokens[0]].getName();
        message = RPL_TOPIC(users[client_fd].getNickname(), channel_name, topic);
        send_to_user(client_fd, message);
        return ;
    }
    std::cout << "Inside topic 1\n";
    // second if-else block to see if channel name is provided with the topic parameter
	// first add check that if channel name is given and if the channel exists and the user is joined to that channel
    if (tokens[0].at(0) == '#' && tokens.size() > 1) {
        if (!users[client_fd].isInChannel(tokens[0])) {
            // user is not part of the given channel
            std::cout << "ERR_NOTONCHANNEL\n";
        	message = ERR_NOTONCHANNEL(users[client_fd].getNickname(), tokens[0]);
        	send_to_user(client_fd, message);
        	return ;
        }
        std::cout << "Inside topic 2\n";
        // if channel mode is +t, add check to see if user is operator in the channel
        if (channels[tokens[0]].getTopicRestricted()) {
            if (!users[client_fd].isOperator(tokens[0])) {
                message = ERR_CHANOPRIVSNEEDED(users[client_fd].getNickname(), tokens[0]);
                send_to_user(client_fd, message);
        	    return ;
            }
        }
        std::cout << "Inside topic 3\n";
        // Concatenate the rest of the tokens after tokens[0]
        std::string new_topic;
        for (size_t i = 1; i < tokens.size(); ++i) {
            if (i > 1) {
                new_topic += " "; // Add a space between tokens
            }
            new_topic += tokens[i];
        }
        channel_name = channels[tokens[0]].getName();
        if (channel_name == " ") {
           std::cout << "Channel name is missing\n";
           return ; 
        }
        // >> :tvalimak_!~tvalimak@freenode-ebn.s3h.4nuk5f.IP TOPIC #channel1 :lol
        message = RPL_CHANGETOPIC(users[client_fd].getNickname(), users[client_fd].getUsername(), channels[tokens[0]].getName(), new_topic);
        channels[tokens[0]].setTopic(new_topic);
        std::cout << "Channel's " << channel_name << " topic changed into " << channels[tokens[0]].getTopic() << "\n";
        send_to_channel(channels[tokens[0]].getName(), message);
        std::cout << "Inside topic 4\n";
        }
    }
}
