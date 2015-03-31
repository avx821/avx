#ifndef VEHICLESTATE_H
#define VEHICLESTATE_H

#include "Waypoint.h"
#include "Attitude.h"

/**
 * This class is a data structure for storing the system's current understanding
 * of the location and attitude of the flight vehicle.
 */
class VehicleState {
public:

	/**
	 * Constructor.
	 */
	VehicleState(){}

	/**
	 * Destructor.
	 */
	~VehicleState(){}

	/**
	 * Sets the vehicle's location waypoint. The latitude must is 
	 * measured in degrees and must be in the range -90 to 90 (inclusive). The 
	 * longitude is measured in degrees and must be in the range -180 to 180
	 * (inclusive). The altitude is measured in meters and must be nonnegative. 
	 * 
	 * @param latitude The new latitude in degrees.
	 * @param longitude The new longitude in degrees.
	 * @param altitude The new altitude in meters.
	 */
	void setLocation(double latitude,
					 double longitude,
					 double altitude);

	/**
	 * Sets the vehicle's roll, pitch, and yaw. The roll, pitch, and yaw are
	 * measured in degrees and must be in the range -180 to 180 (inclusive).
	 * 
	 * @param roll The new roll in degrees.
	 * @param pitch The new pitch in degrees.
	 * @param yaw The new yaw in degrees.
	 */
	void setAttitude(double roll,
					 double pitch,
					 double yaw);

	/**
	 * Accessor for the vehicle's waypoint.
	 *
	 * @return The vehicle's waypoint.
	 */
	Waypoint getLocation();

	/**
	 * Accessor for the vehicle's attitude.
	 *
	 * @return The vehicle's attitude.
	 */
	Attitude getAttitude();

private:

	/**
	 * A waypoint storing the vehicle's current location.
	 */
	Waypoint location;

	/**
	 * An attitude storing the vehicle's current attitude.
	 */
	Attitude attitude;
};

#endif