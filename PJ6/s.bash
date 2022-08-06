    #!/bin/bash
    for t in 32 64 128 256
    do
        for b in 2 4 8 16
        do
            g++ -DLOCAL_SIZE=$t -DNMB=$b -o first first.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
            ./first&>>out.txt

            g++ -DLOCAL_SIZE=$t -DNMB=$b -o first_2 first_2.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
            ./first_2&>>out2.txt

            g++ -DLOCAL_SIZE=$t -DNMB=$b -o first_3 first_3.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
            ./first_3&>>out3.txt
        done
    done
