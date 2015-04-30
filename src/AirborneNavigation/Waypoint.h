#ifndef WAYPOINT_H
#define WAYPOINT_H

#include <vector>

/**
 * This class is a data structure representing a GPS waypoint.
 */
class Waypoint {
public:

	/**
	 * Constructor.
	 */
	Waypoint(){}

	/**
	 * Destructor.
	 */
	~Waypoint(){}

	/**
	 * Sets the waypoint's latitude, longitude, and altitude. The latitude must is 
	 * measured in degrees and must be in the range -90 to 90 (inclusive). The 
	 * longitude is measured in degrees and must be in the range -180 to 180
	 * (inclusive). The altitude is measured in meters and must be nonnegative. 
	 * The method will throw an exception if any parameters are out of range.
	 * 
	 * @param latitude The new latitude in degrees.
	 * @param longitude The new longitude in degrees.
	 * @param altitude The new altitude in meters.
	 *
	 * @throws WaypointException if any parameter is out of range.
	 */
	void setWaypoint(double latitude,
					 double longitude,
					 double altitude);

	/**
	 * Acessor for the waypoint. Will throw an error if the waypoint
	 * has not been instantiated.
	 *
	 * @throws WaypointException if waypoint not instantiated.
	 *
	 * @return The vector representing the waypoint.
	 */
	std::vector<double> getWaypoint();

	/**
	 * Calculated the approximate distance between the given waypoint and this 
	 * waypoint in meters.  
	 *
	 * @throws WaypointException if either waypoint is not instantiated.
	 *
	 * @param start The waypoint to calculate the distance from.
	 *
	 * @return double The approximate distance in meters.
	 */
	double getDistance(Waypoint start);

private:

	/**
	 * Stores the waypoint's latitude and longitude in degrees, and altitude in 
	 * meters, in that order.
	 */
	std::vector<double> waypoint;
};

#endif
