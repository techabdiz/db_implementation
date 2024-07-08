FROM gcc:latest

RUN mkdir /home/src
WORKDIR /home/src
COPY *.c .

RUN gcc main.c -o main

COPY *.ini .

ENTRYPOINT [ "./main" ]