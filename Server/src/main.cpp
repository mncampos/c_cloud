#include "../headers/Server.hpp"

#define PORT 4000

int main(int argc, char *argv[])
{
   Server server;
   if (!server.start())
   {
      std::cerr << "Error initializing server!" << std::endl;
      return -1;
   }

   server.run();

   return 0;
}