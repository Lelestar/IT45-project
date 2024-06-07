@echo off
if "%1"=="" (
    echo Usage: %0 ^<instance_name^> [verbose] [max_duration_seconds]
    exit /b 1
)

set INSTANCE=data\%1.tsp
set VERBOSE=%2
set MAX_DURATION_SECONDS=%3

echo Running Genetic Algorithm on %INSTANCE%...
bin\genetic_algorithm 100000 100 0.8 0.5 %INSTANCE% %VERBOSE% %MAX_DURATION_SECONDS% > results\genetic_%1_results.txt

echo Genetic Algorithm has been executed. Check the results folder for output.