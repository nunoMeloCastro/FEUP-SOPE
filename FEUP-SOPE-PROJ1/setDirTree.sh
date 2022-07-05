#!/bin/bash

DIRNAME="dir"
INITPATH=$(pwd)
CURPATH=$(pwd)
CHARS="0123456789ABCDEFGHIJKLMNOPQRSTUVXWYZabcdefghijklmnopqrstuvxwyz+-_."

for DIRNO in {1..6}
do
    DIRNAME=""
    for i in {1..8}
    do 
        DIRNAME+="${CHARS:RANDOM%${#CHARS}:1}"
    done

    PERM=""
    for i in {0..2}
    do
        PERM+="${DIRPERMS:RANDOM%${#DIRPERMS}:1}"
    done
    
    install -m 777 -d "$DIRNAME"
    CURPATH+="/$DIRNAME"
    cd $CURPATH
    
    for FILENO in {1..11}
    do
        FILENAME=""
        for i in {1..8}
        do 
            FILENAME+="${CHARS:RANDOM%${#CHARS}:1}"
        done
        touch "$FILENAME"
    done
done

exit
