#!/bin/bash

# Script to run the random heuristic experiments

start_time=$(date +"%Y-%m-%d %H:%M:%S")
echo "Script started at: $start_time"

mkdir -p RandomHeuristicExperimentsRepeat

g++ -o RandomHeuristicExperiment SatExp.cpp

num_runs=100

timeout_duration=150

for num_vars in 60 85; do
    for ratio in $(seq 3 0.2 6); do
        output_file="RandomHeuristicExperiments/${num_vars}_${ratio}_output.txt"

        for (( run=1; run<=$num_runs; run++ )); do
            timeout $timeout_duration ./RandomHeuristicExperiment $run 1 $num_vars $ratio $timeout_duration >> $output_file 2>&1
        done

        sleep 1
    done
done

end_time=$(date +"%Y-%m-%d %H:%M:%S")
echo "Script ended at: $end_time"