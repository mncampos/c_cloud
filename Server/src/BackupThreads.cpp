#include <iostream>
#include "../headers/Server.hpp"

void *backupElectionSocket(void *arg)
{
}

void *backupHeartBeat(void *arg)
{
    while (true)
    {
        sleep(10);
        cout << "pao de batata";
    }
}

void *backupDataSync(void *arg)
{
}