/*ubuntu
gcc
src.c
src.exe -BINARY EXE DOESNOT REQUIRE OS
*/
/*
Build
from ub
STAGE 2
FROM scratch:blank image

scratch is used when binary files are there

Build
FROM maven:3.8-openjdk-11 AS builder
WORKDIR/app

COPY --from=builder SRC DEST


FOR JAVA
COPY --maven=builder 
*/

#include<stdio.h>
int main()
{
    printf("Hello World");
    return 0;
}