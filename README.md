Dropbox-like  made in pure cpp and UNIX pthreads/sockets

## Instructions :
```docker-compose up -d``` 

```docker-compose logs servidor_1``` 

```docker-compose exec cliente_1 bash``` 

``` /usr/bin/g++ -fdiagnostics-color=always -g $(pwd)/Client/src/*.cpp $(pwd)/Common/src/*.cpp -o $(pwd)/Client/main.o && $(pwd)/Client/main.o Client_1 192.168.100.2 4000 ``` 

```docker-compose exec cliente_2 bash``` 

```/usr/bin/g++ -fdiagnostics-color=always -g $(pwd)/Client/src/*.cpp $(pwd)/Common/src/*.cpp -o $(pwd)/Client/main.o && $(pwd)/Client/main.o Client_2 192.168.100.2 4000``` 

```docker-compose down```

```docker-compose ps``` 
