#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <iterator>
#include "./ClientSocket.hpp"

class Client
{
public:
    Client(const std::string &username, const std::string &serverIP, int port);
    ClientSocket socket;

private:
    std::string username;
};

#endif