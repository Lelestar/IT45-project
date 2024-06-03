#!/bin/bash

if [ -z "$1" ]
then
  echo "Usage: $0 <instance_name> [verbose] [max_duration_seconds]"
  exit 1
fi

INSTANCE="data/$1.tsp"
VERBOSE=$2
MAX_DURATION_SECONDS=$3

echo "Running Little's Algorithm on $INSTANCE..."
./bin/little_algorithm $INSTANCE $VERBOSE $MAX_DURATION_SECONDS > results/little_${1}_results.txt

echo "Running Tabu Search on $INSTANCE..."
./bin/tabu_search 1500 80 $INSTANCE $VERBOSE $MAX_DURATION_SECONDS > results/tabu_${1}_results.txt

echo "Running Genetic Algorithm on $INSTANCE..."
./bin/genetic_algorithm 100000 100 0.8 0.5 $INSTANCE $VERBOSE $MAX_DURATION_SECONDS > results/genetic_${1}_results.txt

echo "All algorithms have been executed. Check the results folder for output."