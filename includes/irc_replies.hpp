#pragma once

// # define user_id(nickname, username) (":" + nickname + "!" + username + "@ft_irc")

// # define RPL_WELCOME(user_id, nickname) (":ft_irc 001 " + nickname + " :Welcome to the Internet Relay Network " + user_id + "\r\n")
// # define RPL_YOURHOST(client, servername, version) (":ft_irc 002 " + client + " :Your host is " + servername + " (ft_irc), running version " + version + "\r\n")
// # define RPL_CREATED(client, datetime) (":ft_irc 003 " + client + " :This server was created " + datetime + "\r\n")
// # define RPL_MYINFO(client, servername, version, user_modes, chan_modes, chan_param_modes) (":ft_irc 004 " + client + " " + servername + " " + version + " " + user_modes + " " + chan_modes + " " + chan_param_modes + "\r\n")
// # define RPL_ISUPPORT(client, tokens) (":ft_irc 005 " + client + " " + tokens " :are supported by this server\r\n")

// # define ERR_UNKNOWNCOMMAND(client, command) (":ft_irc 421 " + client + " " + command + " :Unknown command\r\n")

// INVITE
# define ERR_NEEDMOREPARAMS(nickname, command) (":ft_irc 461 " + nickname + " " + command + " :Not enough parameters.\r\n")
# define ERR_NOSUCHCHANNEL(nickname, channel) (":ft_irc 403 " + nickname + " " + channel + " :No such channel\r\n")
# define ERR_NOTONCHANNEL(nickname, channel) (":ft_irc 442 " + nickname + " #" + channel + " :The user is not on this channel.\r\n")
# define ERR_USERONCHANNEL(username, nickname, channel) (":ft_irc 443 " + username + " " + nickname+ " " + channel + " :Is already on channel\r\n")
# define RPL_INVITING(operator, invited_user, channel) (":ft_irc 341 " + operator + " " + invited_user + " " + channel + "\r\n")
// :ohertzbe!~ohertzbe@194.136.126.52 INVITE ohertzbe_ :#chan4412
# define RPL_INVITE(nickname, username, invited, channel) (":" + nickname + "!~" + username + "@ft_irc INVITE " + invited + " " + channel + "\r\n")

// JOIN
// use getChannelNicks to get list_of_nicks
# define RPL_NAMREPLY(nickname, channel_name, list_of_nicks) (":ft_irc 353 " + nickname + " = " + channel_name + " :" + list_of_nicks + "\r\n")
# define RPL_ENDOFNAMES(nickname, channel_name) (":ft_irc 366 " + nickname + " " + channel_name + " :End of /NAMES list.\r\n")
# define RPL_JOIN(nickname, username, channel_name) (":" + nickname + "!~" + username + "@ft_irc" + " JOIN :" +  channel_name + "\r\n")
// # define ERR_BANNEDFROMCHAN(client, channel) ("474 " + client + " #" + channel + " :Cannot join channel (+b)\r\n")
# define ERR_BADCHANNELKEY(nickname, channel_name) (":ft_irc 475 " + nickname + " #" + channel_name + " :Cannot join channel (+k) - incorrect password\r\n")

// // KICK
// # define ERR_USERNOTINCHANNEL(client, nickname, channel) ("441 " + client + " " + nickname + " #" + channel + " :They aren't on that channel\r\n")
# define ERR_CHANOPRIVSNEEDED(nickname, channel_name) (":ft_irc 482 " + nickname + " " +  channel_name + " :You're not channel operator\r\n")
# define RPL_KICK(channel_name, kicked, reason) (":ft_irc KICK " + channel_name + " " + kicked + " " + reason + "\r\n")
//>> :ohertzbe!~ohertzbe@194.136.126.51 KICK #hah5 idiot :such an idiot

// // KILL
// # define ERR_NOPRIVILEGES(client) ("481 " + client + " :Permission Denied- You're not an IRC operator\r\n")
// # define RPL_KILL(user_id, killed, comment) (user_id + " KILL " + killed + " " + comment + "\r\n")

// // MODE
// /* user mode */
# define MODE_USERMSG(nickname, username, channel_name, mode) (":" + nickname + "!~" + username + "@ft_irc" + " MODE " + channel_name + " :" + mode + "\r\n")
// #define ERR_UMODEUNKNOWNFLAG(client) (":ft_irc 501 " + client + " :Unknown MODE flag\r\n")
// #define ERR_USERSDONTMATCH(client) ("502 " + client + " :Cant change mode for other users\r\n")
// #define RPL_UMODEIS(client, mode) (":ft_irc 221 " + client + " " + mode + "\r\n")
// /* channel mode */
// #define MODE_CHANNELMSG(channel, mode) (":ft_irc MODE #" + channel + " " + mode + "\r\n")
// #define MODE_CHANNELMSGWITHPARAM(channel, mode, param) (":ft_irc MODE #" + channel + " " + mode + " " + param + "\r\n")
# define RPL_CHANNELMODEIS(nickname, channel_name, modes) (":ft_irc 324 " + nickname + " " + channel_name + " +" + modes + "\r\n")
// #define RPL_CHANNELMODEISWITHKEY(client, channel, mode, password) (":ft_irc 324 " + client + " #" + channel + " " + mode + " " + password + "\r\n")
#define ERR_CANNOTSENDTOCHAN(client, channel) (":ft_irc 404 " + client + " " + channel + " :Cannot send to channel\r\n")
# define ERR_CHANNELISFULL(nickname, channel_name) (":ft_irc 471 " + nickname + " " + channel_name + " :Cannot join channel (+l) - channel is full, try again later\r\n")
# define ERR_INVITEONLYCHAN(nickname, channel_name) (":ft_irc 473 " + nickname + " " + channel_name + " :Cannot join channel (+i) - you must be invited\r\n")
# define ERR_UNKNOWNMODE(nickname, mode) (":ft_irc 472 " + nickname + " " + mode + " :is not a recognised channel mode.\r\n")

// #define ERR_CHANOPRIVSNEEDED(client, channel) (":ft_irc 482 " + client + " #" + channel + " :You're not channel operator\r\n")
#define ERR_INVALIDMODEPARAM(nickname, channel_name, mode, param) (":ft_irc 696 " + nickname + " #" + channel_name + " " + mode + " " + param + " : invalid parameter\r\n")
// // RPL_ERR a broadcoast quand user pas +v ou operator veut parler
//       // dans notre cas c'était tiff (client) qui voulait send a message
//       // :lair.nl.eu.dal.net 404 tiff #pop :Cannot send to channel
// #define RPL_ADDVOICE(nickname, username, channel, mode, param) (":" + nickname + "!" + username + "@ft_irc MODE #" + channel + " " + mode + " " + param + "\r\n")

// // MOTD
// #define ERR_NOSUCHSERVER(client, servername) (":ft_irc 402 " + client + " " + servername + " :No such server\r\n")
// #define ERR_NOMOTD(client) (":ft_irc 422 " + client + " :MOTD File is missing\r\n")
// #define RPL_MOTDSTART(client, servername) (":ft_irc 375 " + client + " :- " + servername + " Message of the day - \r\n")
// #define RPL_MOTD(client, motd_line) (":ft_irc 372 " + client + " :" + motd_line + "\r\n")
// #define RPL_ENDOFMOTD(client) (":ft_irc 376 " + client + " :End of /MOTD command.\r\n")

// // NICK
# define ERR_NONICKNAMEGIVEN(nickname) (":ft_irc 431 " + nickname + " :There is no nickname.\r\n")
# define ERR_ERRONEUSNICKNAME(nickname, new_nickname) (":ft_irc 432 " + nickname + " " + new_nickname + " :Erroneus nickname\r\n")
# define ERR_NICKNAMEINUSE(nickname, new_nickname) (":ft_irc 433 " + nickname + " " + new_nickname + " :Nickname is already in use.\r\n")
# define RPL_NICK(nickname, username, new_nickname) (":" + nickname + "!" + username + "@ft_irc NICK :" +  new_nickname + "\r\n")

// // NOTICE
// # define RPL_NOTICE(nick, username, target, message) (":" + nick + "!" + username + "@ft_irc NOTICE " + target + " " + message + "\r\n")

// // OPER
// # define ERR_NOOPERHOST(client) ("491 " + client + " :No O-lines for your host\r\n")
// # define RPL_YOUREOPER(client) ("381 " + client + " :You are now an IRC operator\r\n")

// // PART
// # define RPL_PART(user_id, channel, reason) (user_id + " PART #" + channel + " " + (reason.empty() ? "." : reason ) + "\r\n")

// // PASS
// # define ERR_PASSWDMISMATCH(client) (":ft_irc 464 " + client + " :Password incorrect.\r\n")

// // PING
// # define RPL_PONG(user_id, token) (user_id + " PONG " + token + "\r\n")

// // QUIT
// :tvalimak_!~tvalimak@freenode-l2g.s3h.4nuk5f.IP QUIT :Quit: leaving
# define RPL_QUIT(username, nickname, reason) (":" + nickname + "!~" + username + "@ft_irc" + " QUIT :Quit: " + reason + "\r\n")
// # define RPL_ERROR(user_id, reason) (user_id + " ERROR :" + reason + "\r\n")

// PRIVMSG
# define ERR_NOSUCHNICK(nickname, target) (":ft_irc 401 " + nickname + " " + target + " :No such nick/channel\r\n")
// # define ERR_NORECIPIENT(client) ("411 " + client + " :No recipient given PRIVMSG\r\n")
// # define ERR_NOTEXTTOSEND(client) ("412 " + client + " :No text to send\r\n")
# define RPL_PRIVMSG(nick, username, target, message) (":" + nick + "!" + "~" + username + "@ft_irc PRIVMSG " + target + " " + message + "\r\n")

// TOPIC
//# define RPL_TOPIC(nickname, channel_name, topic) (":ft_irc 332 " + nickname + " #" + channel_name + " " + topic + "\r\n")
# define RPL_TOPIC(nickname, channel_name, topic) (":ft_irc 332 " + nickname + channel_name + " " + topic + "\r\n")
//# define RPL_NOTOPIC(nickname, channel_name) (":ft_irc 331 " + nickname + " #" + channel_name + " :No topic is set\r\n")
# define RPL_NOTOPIC(nickname, channel_name) (":ft_irc 331 " + nickname + channel_name + " :No topic is set\r\n")
# define RPL_CHANGETOPIC(nickname, username, channel_name, new_topic) (":" + nickname + "!~" + username + "@ft_irc" + " TOPIC " + channel_name + " " + new_topic + "\r\n")
// // USER
// # define ERR_ALREADYREGISTERED(client) (":ft_irc 462 " + client + " :You may not reregister.\r\n")
