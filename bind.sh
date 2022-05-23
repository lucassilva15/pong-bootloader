#!/bin/bash

FIFO=fifo
PORT1=10001
PORT2=10002

NC="nc -v -k -l"

if [ ! -e $FIFO ]; then
        mkfifo $FIFO
fi

$NC -p $PORT1 < $FIFO | $NC -p $PORT2 > $FIFO
