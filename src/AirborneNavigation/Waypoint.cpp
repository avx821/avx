#include <exception>
#include <cmath>

#include "Waypoint.h"

// Exception for use in the Waypoint class.
class WaypointException: public std::exception {
	virtual const char* what() const throw() {
		return "Waypoint Exception";
	}
};

void Waypoint::setMissionParameters(int mission_ID, int iterator_ID) {
	mission = mission_ID;
	iterator = iterator_ID;
}

void Waypoint::setWaypoint(double latitude, double longitude, double altitude) {

	// Delete former waypoint values.
	waypoint.clear();

	// Set parameters.
	waypoint.push_back(latitude);
	waypoint.push_back(longitude);
	waypoint.push_back(altitude);
}

std::vector<double> Waypoint::getWaypoint() {

	// If the vector is not the correct size the waypoint is not instantiated.
	if (waypoint.size() != 3) {
		throw new WaypointException;
	}

	// If instantiated return the waypoint.
	return waypoint;
}

int Waypoint::getMission() {
	return mission;
}

int Waypoint::getIterator() {
	return iterator;
}

double Waypoint::getDistance(Waypoint start) {

	// If the vector is not the correct size the waypoint is not instantiated.
	if (waypoint.size() != 3) {
		throw new WaypointException;
	}

	// Get waypoint vector from the given waypoint.
	std::vector<double> start_coordinates;
	start_coordinates = start.getWaypoint();

	// Conversion from differential latitude and longitude to meters is approximate.
	double latitude_diff = (std::abs(start_coordinates[0] - waypoint[0])/180)*6371000;
	double longitude_diff = (std::abs(start_coordinates[1] - waypoint[1])/180)*6371000;;
	double altitude_diff = std::abs(start_coordinates[2] - waypoint[2]);

	// Calculate the actual distance.
	return std::sqrt((latitude_diff * latitude_diff) + 
					 (longitude_diff * longitude_diff) + 
					 (altitude_diff * altitude_diff));
}
