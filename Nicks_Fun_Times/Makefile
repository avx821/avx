
# This is the default make command, type 'make' to run.
#
default: make

# Ensures all necessary object files are compiled, then
# creates a binary of the program.
#
make: Waypoint.o Attitude.o WaypointManager.o VehicleState.o
	g++ -o AirNav main.cpp \
				   Waypoint.o \
				   Attitude.o \
				   WaypointManager.o \
				   VehicleState.o \

# Compiles the Waypoint object file.
Waypoint.o:  
	g++ -c Waypoint.cpp

# Compiles the Attitude object file.
Attitude.o:
	g++ -c Attitude.cpp

WaypointManager.o:
	g++ -c WaypointManager.cpp

VehicleState.o:
	g++ -c VehicleState.cpp

# Deletes all object files and binaries.
#
clean: 
	rm AirNav *.o