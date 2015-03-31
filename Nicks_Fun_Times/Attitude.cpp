#include "Attitude.h"

#include <exception>

// Exception for use in the Attitude class.
class AttitudeException: public std::exception {
	virtual const char* what() const throw() {
		return "Attitude Exception";
	}
};

void Attitude::setAttitude(double roll, double pitch, double yaw) {

	// Make sure parameters are within range.
	if (roll < -180 || roll > 180 || 
		pitch < -180 || pitch > 180 ||
		yaw < -180 || yaw > 180) {
		throw new AttitudeException;
	}

	// Delete former waypoint values.
	attitude.clear();

	// Set parameters.
	attitude.push_back(roll);
	attitude.push_back(pitch);
	attitude.push_back(yaw);
}

std::vector<double> Attitude::getAttitude() {

	// If the vector is not the correct size the attitude is not instantiated.
	if (attitude.size() != 3) {
		throw new AttitudeException;
	}

	// If instantiated return the attitude.
	return attitude;
}