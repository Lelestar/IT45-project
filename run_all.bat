@echo off
if "%1"=="" (
    echo Usage: %0 ^<instance_name^> [verbose]
    exit /b 1
)

set INSTANCE=data\%1.tsp
set VERBOSE=%2

echo Running Little's Algorithm on %INSTANCE%...
bin\little_algorithm %INSTANCE% %VERBOSE% > results\little_%1_results.txt

echo All algorithms have been executed. Check the results folder for output.
