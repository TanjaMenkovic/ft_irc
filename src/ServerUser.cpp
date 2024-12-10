#include "../includes/Server.hpp"

namespace irc 
{

/*
funcitons  for adding user in the channel, removing user from the channel
adding operators and removing operators, checking is it operator or not

*/


/*
did we reach the limit how many users can be in the channel
*/
bool Server::IsLimitReached(const std::string &channel_name) {
    int limit = channels.find(channel_name)->second.getUserLimit();
    int number_of_users = 0;
    for (auto &[client_fd, user]: users) {
        if (user.isInChannel(channel_name)) {
            number_of_users++;
        }
    }
    return limit < number_of_users ? false : true;
}

/*
    Create a map of channels that user is in.
*/
map Server::getUsers<> {

}

}