# Sistemas Operacionas II - Dropbox
## Integrantes :
- Mateus Nunes Campos 
- Vinicius Fraga Coromberque
- Leonardo Bilhalva
- João Pilotti


## Como executar
```docker-compose up -d``` -> vai levantar a infraestrutura toda

```docker-compose logs servidor_1``` -> mostra o que esta rolando no servidor -> o correto é estar escutando na porta 4000

#### levanta o cliente 1

```docker-compose exec cliente_1 bash``` -> entra no terminal do cliente 1

``` /usr/bin/g++ -fdiagnostics-color=always -g $(pwd)/Client/src/*.cpp $(pwd)/Common/src/*.cpp -o $(pwd)/Client/main.o && $(pwd)/Client/main.o Client_1 192.168.100.2 4000 ``` -> cliente_1 esta pronto para ser utilizado

#### levanta o cliente 2

```docker-compose exec cliente_2 bash``` -> entra no terminal do cliente 2

```/usr/bin/g++ -fdiagnostics-color=always -g $(pwd)/Client/src/*.cpp $(pwd)/Common/src/*.cpp -o $(pwd)/Client/main.o && $(pwd)/Client/main.o Client_2 192.168.100.2 4000``` -> cliente_2 esta pronto para ser utilizado

## comandos uteis

```docker-compose down``` -> exclui a infraestrutura toda

```docker-compose ps``` -> mostra quais containers estão online