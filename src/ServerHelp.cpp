#include "../includes/Server.hpp"

const irc::User* find_user_by_nickname(const std::string& nickname, const irc::Server& serv) {
    for (auto& [fd, user] : serv.getUsers()) {
        if (user.getNickname() == nickname) {
            return &user;
        }
    }
    return nullptr; // Not found
}
