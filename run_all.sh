#!/bin/bash

if [ -z "$1" ]
then
  echo "Usage: $0 <instance_name> [verbose]"
  exit 1
fi

INSTANCE="data/$1.tsp"
VERBOSE=$2

echo "Running Little's Algorithm on $INSTANCE..."
./bin/little_algorithm $INSTANCE $VERBOSE > results/little_$1_results.txt

echo "All algorithms have been executed. Check the results folder for output."