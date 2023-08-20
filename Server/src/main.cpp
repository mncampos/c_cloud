#include "../headers/Server.hpp"

#define PORT 4000

int main(int argc, char *argv[])
{

   Server server;

   if (argc == 1){
      server.runBackup(argv[0]);
   }

   server.run();

   return 0;

}