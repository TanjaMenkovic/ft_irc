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
// with the channel's name in form of #<channelname>

// First check if the topic command is used inside a channel, if it is then it can be ran without parameters
// and it will just return the current topic of the channel if there is any.


void Server::topic(int client_fd, std::vector<std::string> tokens) {


}
}