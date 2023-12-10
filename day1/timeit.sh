#!/bin/env zsh

START=`date +%s.%N`
./bin/mainc ./puzzle.txt
END=`date +%s.%N`

runtime=$(echo "$END - $START" | bc -l)
echo $runtime
