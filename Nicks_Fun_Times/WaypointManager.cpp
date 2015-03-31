#include "WaypointManager.h"

#include <exception>

// Exception for use in the WaypointManager class.
class WaypointManagerException: public std::exception {
	virtual const char* what() const throw() {
		return "Waypoint Manager Exception";
	}
};

void WaypointManager::setVehicle(VehicleState vehicle) {
	myVehicle = vehicle;
}

void WaypointManager::setWaypoints(std::vector<Waypoint> waypoints) {

	// We try to effectively reset the waypoint vector we are checking.
	myWaypoints = waypoints;
	current = 0;
}

Waypoint WaypointManager::getCurrentWaypoint() {

	// Check if we are within 50 meters of the current waypoint.
	if (myVehicle.getLocation().getDistance(myWaypoints[current]) < 50) {

		// Increment to the next waypoint.
		current = current + 1;
	}

	// Check if we have hit all of our waypoints.
	if (current >= myWaypoints.size()) {
		WaypointManagerException throwThis;
		throw throwThis;
	}

	// Return the current waypoint.
	return myWaypoints[current];
}