#include "../headers/Server.hpp"

#define PORT 4000

int main(int argc, char *argv[])
{

   Server server;

   if (argc == 2){
      server.runBackup(argv[1]);
   } else {
      server.run();
   }

   return 0;

}