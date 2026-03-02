FROM ubuntu:22.04 AS build

RUN apt-get update && apt-get install -y gcc

COPY hello.c .

RUN gcc -static -o hello hello.c

CMD ["./hello"]