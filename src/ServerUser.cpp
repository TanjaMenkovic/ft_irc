#include "../includes/Server.hpp"

namespace irc 
{
    
// did we reach the limit how many users can be in the channel
bool Server::IsLimitReached(const std::string &channel_name) {
    int limit = channels.find(channel_name)->second.getUserLimit();
    if (limit == -1) {
        return false;
    }
    int number_of_users = 0;
    for (auto &[client_fd, user]: users) {
        if (user.isInChannel(channel_name)) {
            number_of_users++;
        }
    }
    return limit <= number_of_users ? true : false;
}

}