#include "../includes/Server.hpp"

namespace irc 
{

bool Server::isNicknameTaken(const std::string &nickname) const {
    for (const auto &user : users) {
        if (user.second.getNickname() == nickname) {
            return true; // Nickname is taken
        }
    }
    return false; // Nickname is available
}

void Server::handle_nick(int client_fd, const std::string& line)
{
    if (line.find("NICK ") == 0) {
        std::string nickname = line.substr(5);

        std::string original = nickname;
        int counter = 1;

        while (isNicknameTaken(nickname)) {
            nickname = original + std::to_string(counter);
            counter++;
        }

        users[client_fd].setNickname(nickname);
        std::cout << "User set nickname: " << users[client_fd].getNickname() << std::endl;
        this->users[client_fd].setNickReceived();
    }
}

void Server::handle_user(int client_fd, const std::string& line)
{
    if (line.find("USER ") == 0){
        std::size_t start_pos = 5;
        std::size_t space_pos = line.find(" ", start_pos);

        std::string username = line.substr(start_pos, space_pos - start_pos);
        users[client_fd].setUsername(username);
        std::cout << "User set username: " << users[client_fd].getUsername() << std::endl;
        this->users[client_fd].setUserReceived();
    }
}

// Helper function to handle the password phase
void Server::handle_password_phase(int client_fd, const std::string& line) {
    if (line.find("PASS ") == 0) {
        std::string client_password = line.substr(5); // Extract password
        if (!validateClientPassword(client_password)) {
            std::string error_message = "ERROR: Incorrect password. Connection closed.\n";
            send(client_fd, error_message.c_str(), error_message.size(), 0);
            std::cerr << "Client provided an incorrect password. Connection terminated.\n";
        } else {
            std::string success_message = "Password accepted. Welcome to the server!\n";
            send(client_fd, success_message.c_str(), success_message.size(), 0);
            this->users[client_fd].setPassReceived();
        }
    }
}

void Server::authentication(int client_fd, std::string line) {
    if (this->users[client_fd].getNickReceived() == false) {
        handle_nick(client_fd, line);
    }
    if (this->users[client_fd].getUserReceived() == false)
        handle_user(client_fd, line);

    if (this->users[client_fd].getPassReceived() == false) {
        handle_password_phase(client_fd, line);
    }

    if (this->users[client_fd].getUserReceived() 
        && this->users[client_fd].getNickReceived()
        && this->users[client_fd].getPassReceived()) 
    {
        send_welcome_message(client_fd, users[client_fd].getNickname()); // Replace with actual username
        users[client_fd].setAsAuthenticated();
    }
}

// Helper function to send the IRC welcome message
void Server::send_welcome_message(int client_fd, const std::string& nickname) {
    std::string welcome_msg = ":ft_irc 001 " + nickname + " :Welcome to the IRC Network, " + nickname + "!\r\n";
    send(client_fd, welcome_msg.c_str(), welcome_msg.length(), 0);
    std::cout << "Sent welcome message to client: " << nickname << "\n";
}

void Server::send_mode_message(int client_fd, const std::string& nickname) {
    std::string welcome_msg = ":ft_irc 002 " + nickname + " :Your host is ft_irc.\r\n";
    send(client_fd, welcome_msg.c_str(), welcome_msg.length(), 0);
    std::cout << "Sent welcome message to client: " << nickname << "\n";
}

}