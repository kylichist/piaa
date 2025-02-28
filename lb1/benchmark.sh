#!/bin/bash

make build

>benchmark.txt

for ((i = 2; i <= 20; i++)); do
    start=$(date +%s.%N)
    echo "$i" | ./a.out
    end=$(date +%s.%N)

    time=$(echo "$end - $start" | bc)
    echo "($i, $time)" >>benchmark.txt
done
