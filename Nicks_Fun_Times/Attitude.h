#ifndef ATTITUDE_H
#define ATTITUDE_H

#include <vector>

/**
 * This class is a data structure representing an attidue.
 */
class Attitude {
public:

	/**
	 * Constructor.
	 */
	Attitude(){}

	/**
	 * Destructor. 
	 */
	~Attitude(){}

	/**
	 * Sets the attitudes's roll, pitch, and yaw. The roll, pitch, and yaw are
	 * measured in degrees and must be in the range -180 to 180 (inclusive). Will
	 * throw an error if any of the given parameters are out of range.
	 * 
	 * @param roll The new roll in degrees.
	 * @param pitch The new pitch in degrees.
	 * @param yaw The new yaw in degrees.
	 *
	 * @throws AttitudeException if any parameter is out of range.
	 */
	void setAttitude(double roll,
					 double pitch,
					 double yaw);

	/**
	 * Accessor for the attitude.  Will throw an error if the waypoint has
	 * not been instantiated.
	 *
	 * @throws AttitudeException if attitude not instantiated.
	 *
	 * @return The vector representing the attitude.
	 */
	std::vector<double> getAttitude();

private:

	/**
	 * Stores the attitude's roll, pitch, and yaw in degrees, in that order.
	 */
	std::vector<double> attitude;
};

#endif