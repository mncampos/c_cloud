#include "../headers/BackupHandler.hpp"

BackupHandler::BackupHandler(int backupSocket, ServerSocket *serverSocket)
{
    this->backupSocket = backupSocket;
    this->serverSocket = serverSocket;
}

void BackupHandler::handleBackup()
{

    while (true)
    {

        // @TODO: atualizar replicas
        // fazer uma funcao q verifica se o sinal esta ativo e atualiza todas replicas
        // o conteudo é tudo que esta no servidor principal

        // @TODO: heartbeat
        // recebeu mensagem tipo heartbeat que backup envia para principal -> servidor principal  devolve mensagem do tipo heartbeat
    }
}