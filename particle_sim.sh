#!/bin/bash
# particle_sim.sh

# This program will take input configuration files and output a coordinates
# file and PNG plots of the physics simulation.  Further details can be found
# in README.md.

# usage: particle_sim <config> <initialization> <output>

# Check for the right number of arguments

if [[ $# -ne 3 ]]
then
    echo "Invalid number of arguments!"
    echo "Usage: particle_sim: <config> <initialization> <output>"
    echo "See README.md for further details."
    exit 1
fi

# Compile the Lex, Yacc, and C files, if needed

if [ ! -f configparse -o ! -f initparse -o ! -f main ]
then
    echo "Compiling files... "
    make > /dev/null 2>&1
fi

# Parse the input files, and check if they parsed correctly

echo "Parsing configuration file..."
./configparse < $1 > INPUTCONFIG

if [[ $? -ne 0 ]]
then
    echo "Failed to parse configuration file!  Exiting."
    exit 1
fi

if [[ ! -s INPUTCONFIG ]]
then
    echo "Encountered empty configuration file after parsing!  Exiting."
    exit 1
fi

echo "Parsing initialization file..."
./initparse < $2 > INPUTINIT

if [[ $? -ne 0 ]]
then
    echo "Failed to parse initialization file!  Exiting."
    exit 1
fi

if [[ ! -s INPUTINIT ]]
then
    echo "Encountered empty initialization file after parsing!  Exiting."
    exit 1
fi

# Run the actual simulation

echo "Initializing simulation..."

./main INPUTCONFIG INPUTINIT

if [[ $? -ne 0 ]]
then
    echo "Simulation failed to complete!  Exiting."
    exit 1
fi

# Create plots

echo "Creating plots..."
python3 createplot.py $3

rm INPUTCONFIG INPUTINIT OUTPUTCOORDS

exit 0
