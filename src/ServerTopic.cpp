#include "../includes/Server.hpp"

namespace irc 
{

void Server::topic(int client_fd, std::vector<std::string> tokens) {
	std::string channel_name;
	std::string topic;
	std::string message;

    // first check if the channel exists if channelname param is provided
    if (channels.find(tokens[0]) == channels.end()) {
        message = ERR_NOSUCHCHANNEL(users[client_fd].getNickname(), tokens[0]);
        send_to_user(client_fd, message);
        return ;
    }
    if (tokens.size() == 0) {
        message = ERR_NEEDMOREPARAMS(users[client_fd].getNickname(), tokens[0]);
        send_to_user(client_fd, message);
        return ;
    }
    // first if block is to check if the command is provided with channelname but no other params,
    // in this case we just want to return the topic of the given channel if it exists
    if (tokens[0].at(0) == '#' && tokens.size() == 1) {
        // get the topic of the channel
        topic = channels[tokens[0]].getTopic();
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
    // second if-else block to see if channel name is provided with the topic parameter
	// first add check that if channel name is given and if the channel exists and the user is joined to that channel
    if (tokens[0].at(0) == '#' && tokens.size() > 1) {
        if (!users[client_fd].isInChannel(tokens[0])) {
            // user is not part of the given channel
        	message = ERR_NOTONCHANNEL(users[client_fd].getNickname(), tokens[0]);
        	send_to_user(client_fd, message);
        	return ;
        }
        // if channel mode is +t, add check to see if user is operator in the channel
        if (channels[tokens[0]].getTopicRestricted()) {
            if (!users[client_fd].isOperator(tokens[0])) {
                message = ERR_CHANOPRIVSNEEDED(users[client_fd].getNickname(), tokens[0]);
                send_to_user(client_fd, message);
        	    return ;
            }
        }
        std::string new_topic;
        for (size_t i = 1; i < tokens.size(); ++i) {
            if (i > 1) {
                new_topic += " ";
            }
            new_topic += tokens[i];
        }
        channel_name = channels[tokens[0]].getName();
        if (channel_name == " ") {
           return ; 
        }
        message = RPL_CHANGETOPIC(users[client_fd].getNickname(), users[client_fd].getUsername(), channels[tokens[0]].getName(), new_topic);
        channels[tokens[0]].setTopic(new_topic);
        send_to_channel(channels[tokens[0]].getName(), message);
        }
    }
}
