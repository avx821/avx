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
	 * Sets the parameters that identify this waypoint's mission and location
	 * within the mission. These parameters are not checked in any way and 
	 * thus should be used carefully.
	 *
	 * @param mission_ID the integer identifying this waypoint's mission.
	 * @param iterator_ID the integer identifying this waypoint's 
	 * 		  location within its mission.
	 */
	void setMissionParameters(int mission_ID,
							  int iterator_ID);

	/**
	 * Sets the waypoint's latitude, longitude, and altitude. Due to issues with
	 * GPS noise, these inputs are unbounded, and thus should be used with care.
	 * 
	 * @param latitude The new latitude in degrees.
	 * @param longitude The new longitude in degrees.
	 * @param altitude The new altitude in meters.
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
	 * Accessor for this waypoint's mission ID. Does not check if the mission ID
	 * has been instantiate and thus should be used with care.
	 *
	 * @return int The waypoint's mission ID.
	 */
	int getMission();

	/**
	 * Accessor for this waypoint's mission iterator.  Does not check if the 
	 * iterator has been instantiated and thus should be used with care.
	 *
	 * @return int The waypoint's mission iterator.
	 */
	int getIterator();

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
	 * Stores the waypoint's mission ID.
	 */
	int mission;

	/**
	 * Stores the waypoint's number it its mission.
	 */
	int iterator;

	/**
	 * Stores the waypoint's latitude and longitude in degrees, and altitude in 
	 * meters, in that order.
	 */
	std::vector<double> waypoint;
};

#endif
