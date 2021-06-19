#!/bin/bash
echo "** THIS IS THE CrowdSenSim SIMULATOR **"

for ((number=1;number < 101;number++))
{
echo "RIpetition Number   "  $number

if python -W ignore crowdsensim2.py; then
    # The job:



for ((i=1;i < 4;i++))
{	
	rm Inputs/Setup.txt
	cp $i/Setup.txt Inputs/Setup.txt

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
	
	echo "** END **"



	}

else
	echo "Exit forced"
fi

}








