#!/bin/bash

for t in 1 2 4
do
  echo NUMT = $t
  for n  in 10 20 40 80 160 320 640 1000

  do
        g++ -DNUMT=$t -DNUMNODES=$n proj2.cpp  -o proj2  -lm  -fopenmp
    ./proj2
  done
done
