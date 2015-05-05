
# This is the default make command, type 'make' to run.
#
default: make

# Ensures all necessary object files are compiled, then
# creates a binary of the program.
#
make: ModeStatusListener.o
	g++ -o StateMachine StateMachine.cpp \
	       ModeStatusListener.o -llcm

# Compiles the Waypoint object file.
ModeStatusListener.o:  
	g++ -c ModeStatusListener.cpp

# Deletes all object files and binaries.
#
clean: 
	rm StateMachine *.o
