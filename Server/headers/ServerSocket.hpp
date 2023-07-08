#ifndef SERVERSOCKET_HPP
#define SERVERSOCKET_HPP

#include "../../Common/Socket.hpp"
#include <fstream>
#include <algorithm>

class ServerSocket : public Socket {
    public:
    ServerSocket(int port);
    bool bind();
    bool listen(int backlog = 5);
    int accept();



    private:
    int port;

};

#endif