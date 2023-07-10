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
#include <algorithm>
#include <numeric>
#include <atomic>
#include "../../Common/FileHandler.hpp"

class Client
{
public:
    Client(const std::string &username, const std::string &serverIP, int port);
    ClientSocket socket;
    void uploadFile(std::string fileName);
    void downloadFile(std::string fileName);
    void deleteFile(std::string fileName);
    void listServerFiles();
    void listClientFiles();
    void getSyncDir();
    void requestSync();
    void listenForSignal();
    std::string getUsername();

private:
    std::string username;
};

#endif