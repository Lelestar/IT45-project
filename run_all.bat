@echo off
if "%1"=="" (
    echo Usage: %0 ^<instance_name^>
    exit /b 1
)

set INSTANCE=data\%1.tsp

echo Running Little's Algorithm on %INSTANCE%...
bin\little_algorithm %INSTANCE% > results\little_%1_results.txt

echo All algorithms have been executed. Check the results folder for output.
