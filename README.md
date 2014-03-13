Particle simulation, by Bernard Xie.

This program will take in the initial state of a particle setup and run a physics simulation on the particles, taking into account electromagnetic forces, gravity, and collisions, and it will output coordinates and PNG plots of the paths of the particles during the simulation.  The simulation is simplified, and does not take into account phenomena such as relativistic effects, quantum mechanical effects, or rigid body motion.  Additionally, particles will not split into fun things like leptons and quarks.

The shell script takes three arguments: first, a simulation configuration file that specifies the duration of the simulation, as well as allowing the specification of forces that all particles are subject to (such as a constant gravitational force or a magnetic field).  The second argument is a file which specifies the initial data for each particle, such as initial velocity and charge.  See CONFIG_DETAILS.txt and INITIALIZATION_DETAILS.txt for further details.  The third argument is the output name of the plots, which will be suffixed with '_xyzplot.png'.

When fully compiled, this project will create a lot of files.  Run "make clean" in the terminal in order to remove the unnecessary ones.

As my final project for CMSC 16200 at the University of Chicago, this program uses several different languages to accomplish this task:

The configuration file parser is written in Lex and YACC.
The particle simulator is written in C.
The plot creator is written in Python, with the help of Matplotlib.
The shell wrapper for the entire particle simulation is written in Bash.

A big thank you goes out to Thomas Smid, whose implementation of a 3D particle collision system was adapted by me and implemented in this code.  Without him, there would be no collision detection.  The original code can be found at
http://www.plasmaphysics.org.uk/programs/coll3d_cpp.htm

It is quite possible that there will be bugs to this program.  If so, the best way to get in contact with me is via GitHub.
