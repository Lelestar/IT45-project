#!/bin/bash

if [ -z "$1" ]
then
  echo "Usage: $0 <instance_name> [verbose] [max_duration_seconds]"
  exit 1
fi

INSTANCE="data/$1.tsp"
VERBOSE=$2
MAX_DURATION_SECONDS=$3

echo "Running Tabu Search on $INSTANCE..."
./bin/tabu_search 1500 80 $INSTANCE $VERBOSE $MAX_DURATION_SECONDS > results/tabu_${1}_results.txt

echo "Tabu Search has been executed. Check the results folder for output."