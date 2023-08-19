FROM ubuntu:22.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y tzdata
RUN apt-get install -y vim build-essential git cmake net-tools gdb clang

WORKDIR /work


# ref: https://www.youtube.com/watch?v=93kgF8eI6Do&t
# docker build -t linux-cpp . ## build linux cpp image
# docker run -it --rm -v "$(pwd)":/work linux-cpp ## run image and copy working directory to docker
# open in vsc terminal the docker container
# install C/C++ Extension pack on vsc -> for running CPP
# (opt) install Code runner extension for faster build shorctut on vsc
# enjoy building on docker :)