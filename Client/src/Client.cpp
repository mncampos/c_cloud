#include "../headers/Client.hpp"

Client::Client(const std::string &username, const std::string &serverIP, int port)
    : username(username), socket(serverIP, port) {
}
