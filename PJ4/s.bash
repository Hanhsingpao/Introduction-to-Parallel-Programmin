    #!/bin/bash
    for s in 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608
    do
    echo ArraySize = $s
       g++ -DSIZE=$s -o proj4 proj4.cpp -lm -fopenmp
       ./proj4&>> out.csv
    done

