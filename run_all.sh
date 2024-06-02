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
./bin/little_algorithm $INSTANCE $VERBOSE $MAX_DURATION_SECONDS > results/little_$1_results.txt

echo "All algorithms have been executed. Check the results folder for output."