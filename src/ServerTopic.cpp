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
           ERR_CHANOPRIVSNEEDED
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


void Server::topic(int client_fd, std::vector<std::string> tokens) {
	std::string server_name;
	std::string topic;
	std::string message;

	// first add check that if channel name is given and if the user is operator on a given channel
    if (tokens[0].at(0) == '#') {
        if (!users[client_fd].isInChannel(tokens[0])) {
        	message = ERR_NOTONCHANNEL(users[client_fd].getNickname(), tokens[0]);
        	send_to_user(client_fd, message);
        	return ;
    }
	// if no channelname is given, check if the command was input on a channel that the user is joined in
}
}