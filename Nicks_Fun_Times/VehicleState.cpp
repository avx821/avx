#include "VehicleState.h"

void VehicleState::setLocation(double latitude, double longitude, double altitude) {
	location.setWaypoint(latitude, longitude, altitude);
}

void VehicleState::setAttitude(double pitch, double roll, double yaw) {
	attitude.setAttitude(roll, pitch, yaw);
}

Waypoint VehicleState::getLocation() {
	return location;
}

Attitude VehicleState::getAttitude() {
	return attitude;
}