@echo off
if "%1"=="" (
    echo Usage: %0 ^<instance_name^> [verbose] [max_duration_seconds]
    exit /b 1
)

set INSTANCE=data\%1.tsp
set VERBOSE=%2
set MAX_DURATION_SECONDS=%3

::echo Running Little's Algorithm on %INSTANCE%...
::bin\little_algorithm %INSTANCE% %VERBOSE% %MAX_DURATION_SECONDS% > results\little_%1_results.txt

echo Running Tabu Search on %INSTANCE%...
bin\tabu_search 1500 80 %INSTANCE% %VERBOSE% %MAX_DURATION_SECONDS% > results\tabu_%1_results.txt

echo All algorithms have been executed. Check the results folder for output.
