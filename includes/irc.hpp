#pragma once

#include "Server.hpp"
#include <iostream>

bool isValidPort(const std::string& str);
bool isValidPassword(const std::string& str);

irc::User* find_user_by_nickname(const std::string& nickname);