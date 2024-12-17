#pragma once

# define ERR_UNKNOWNCOMMAND(nickname, command) (":ft_irc 421 " + nickname + " " + command + " :Unknown command\r\n")

// // INVITE
# define ERR_NEEDMOREPARAMS(nickname, command) (":ft_irc 461 " + nickname + " " + command + " :Not enough parameters.\r\n")
# define ERR_NOSUCHCHANNEL(nickname, channel) (":ft_irc 403 " + nickname + " " + channel + " :No such channel\r\n")
# define ERR_NOTONCHANNEL(nickname, channel) (":ft_irc 442 " + nickname + " " + channel + " :The user is not on this channel.\r\n")
# define ERR_USERONCHANNEL(username, nickname, channel) (":ft_irc 443 " + username + " " + nickname+ " " + channel + " :Is already on channel\r\n")
# define RPL_INVITING(operator, invited_user, channel) (":ft_irc 341 " + operator + " " + invited_user + " " + channel + "\r\n")
# define RPL_INVITE(nickname, username, invited, channel) (":" + nickname + "!~" + username + "@ft_irc INVITE " + invited + " " + channel + "\r\n")

// JOIN
// use getChannelNicks to get list_of_nicks
# define RPL_NAMREPLY(nickname, channel_name, list_of_nicks) (":ft_irc 353 " + nickname + " = " + channel_name + " :" + list_of_nicks + "\r\n")
# define RPL_ENDOFNAMES(nickname, channel_name) (":ft_irc 366 " + nickname + " " + channel_name + " :End of /NAMES list.\r\n")
# define RPL_JOIN(nickname, username, channel_name) (":" + nickname + "!~" + username + "@ft_irc" + " JOIN :" +  channel_name + "\r\n")
# define ERR_BADCHANNELKEY(nickname, channel_name) (":ft_irc 475 " + nickname + " " + channel_name + " :Cannot join channel (+k) - incorrect password\r\n")

// // KICK
# define ERR_CHANOPRIVSNEEDED(nickname, channel_name) (":ft_irc 482 " + nickname + " " +  channel_name + " :You're not channel operator\r\n")
# define RPL_KICK(username, nickname, channel_name, kicked, reason) (":" + nickname + "!~" + username + "@ft_irc KICK " + channel_name + " " + kicked + " " + reason + "\r\n")

// // MODE
# define MODE_USERMSG(nickname, username, channel_name, mode) (":" + nickname + "!~" + username + "@ft_irc" + " MODE " + channel_name + " " + mode + "\r\n")
# define RPL_CHANNELMODEIS(nickname, channel_name, modes) (":ft_irc 324 " + nickname + " " + channel_name + " +" + modes + "\r\n")
# define ERR_CANNOTSENDTOCHAN(nickname, channel) (":ft_irc 404 " + nickname + " " + channel + " :Cannot send to channel\r\n")
# define ERR_CHANNELISFULL(nickname, channel_name) (":ft_irc 471 " + nickname + " " + channel_name + " :Cannot join channel (+l) - channel is full, try again later\r\n")
# define ERR_INVITEONLYCHAN(nickname, channel_name) (":ft_irc 473 " + nickname + " " + channel_name + " :Cannot join channel (+i) - you must be invited\r\n")
# define ERR_UNKNOWNMODE(nickname, mode) (":ft_irc 472 " + nickname + " " + mode + " :is not a recognised channel mode.\r\n")
#define ERR_INVALIDMODEPARAM(nickname, channel_name, mode, param) (":ft_irc 696 " + nickname + " " + channel_name + " " + mode + " " + param + " : invalid parameter\r\n")

// // NICK
# define ERR_NONICKNAMEGIVEN(nickname) (":ft_irc 431 " + nickname + " :There is no nickname.\r\n")
# define ERR_ERRONEUSNICKNAME(nickname, new_nickname) (":ft_irc 432 " + nickname + " " + new_nickname + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(nickname, new_nickname) (":ft_irc 433 " + nickname + " " + new_nickname + " :Nickname is already in use.\r\n")
# define RPL_NICK(nickname, username, new_nickname) (":" + nickname + "!" + username + "@ft_irc NICK :" +  new_nickname + "\r\n")

// // QUIT
# define RPL_QUIT(username, nickname, reason) (":" + nickname + "!~" + username + "@ft_irc" + " QUIT :Quit: " + reason + "\r\n")

// // PRIVMSG
# define ERR_NOSUCHNICK(nickname, target) (":ft_irc 401 " + nickname + " " + target + " :No such nick/channel\r\n")
# define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + "~" + username + "@ft_irc PRIVMSG " + target + " " + message + "\r\n")

// TOPIC
# define RPL_TOPIC(nickname, channel_name, topic) (":ft_irc 332 " + nickname + " " + channel_name + " " + topic + "\r\n")
# define RPL_NOTOPIC(nickname, channel_name) (":ft_irc 331 " + nickname + channel_name + " :No topic is set\r\n")
# define RPL_CHANGETOPIC(nickname, username, channel_name, new_topic) (":" + nickname + "!~" + username + "@ft_irc" + " TOPIC " + channel_name + " " + new_topic + "\r\n")

