#!/bin/bash

if [ -z "$1" ]
then
  echo "Usage: $0 <instance_name>"
  exit 1
fi

INSTANCE="data/$1.tsp"

echo "Running Little's Algorithm on $INSTANCE..."
./bin/little_algorithm $INSTANCE > results/little_$1_results.txt

echo "All algorithms have been executed. Check the results folder for output."
