
# This is the default make command, type 'make' to run.
#
default: make

# Ensures all necessary object files are compiled, then
# creates a binary of the program.
#
make: Waypoint.o WaypointManager.o
	g++ -o AirborneNavigation Waypoint.o \
		      WaypointManager.o \
		      AirborneNavigation.cpp -llcm

# Compiles the Waypoint object file.
#
Waypoint.o:  
	g++ -c Waypoint.cpp

# Compiles the WaypointManager object file.
#
WaypointManager.o:
	g++ -c WaypointManager.cpp

# Deletes all object files and binaries.
#
clean: 
	rm AirNav *.o
