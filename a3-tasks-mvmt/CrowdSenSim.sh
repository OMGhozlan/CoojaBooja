#!/bin/bash
echo "** THIS IS THE CrowdSenSim SIMULATOR **"


if python -W ignore crowdsensim2.py; then
    # The job:
echo "-| Compiling files..."
g++ -w ./src/CrowdSenSim.cpp ./src/Utilities.cc ./src/Simulation.cc ./src/Statistics.cc ./src/ClockManagement.cc ./src/ReadValues.cc ./src/Antenna.cc ./src/Event.cc ./src/HeatMap.cc ./src/Location.cc  ./src/Position.cc ./src/Sample.cc ./src/Smartphones.cc ./src/User.cc  -Wall -O3 -o main-exe 
if [ $? -ne 0 ]
then
    echo "-| Compilation failed!"
    exit 1
else
    echo "-| Compilation succeeded without warnings!"
    time ./main-exe $1 $2 $3 $4
fi
rm main-exe
# Run Statistics of simulation
firefox -private-window http://localhost/CrowdSenSim/Statistics.php 
echo "** END **"

else
    echo "Exit forced"
fi



