#ifndef WAYPOINTMANAGER_H
#define WAYPOINTMANAGER_H

#include <vector>

#include "Waypoint.h"

/**
 * This class manages waypoints and checks if the vehicle has hit the current 
 * waypoint and should move on to the next.
 */
class WaypointManager {
public:

	/**
	 * Constructor.
	 */
	WaypointManager(){}

	/**
	 * Destructor.
	 */
	~WaypointManager(){}

	/**
	 * Sets the vehicle location.  
	 *
	 * @param vehicle The vehicle to monitor.
	 */
	 void setVehicleLocation(Waypoint location);

	/**
	 * Used to set the vector of waypoints.  The first waypoint must be the start
	 * of the vector, and the waypoints must progress in order thereafter.  
	 *
	 * @param waypoints A vector of waypoints.
	 */
	void setWaypoints(std::vector<Waypoint> waypoints);

	/**
	 * Accessor for the current waypoint. This method will check to see if the
	 * vehicle has progressed to the current waypoint.  Will throw an error if
	 * the manager has no more waypoints.
	 *
	 * @throws WaypointManagerException if at there are no additional waypoints.
	 *
	 * @return The current waypoint.
	 */
	Waypoint getCurrentWaypoint();

private:

	/**
	 * The tolerance for hitting a waypoint in meters.
	 */
	final double tolerance = 50;

	/**
	 * A vector containing the waypoints.
	 */
	std::vector<Waypoint> myWaypoints;

	/**
	 * The current waypoint index.
	 */
	int current;

	/**
	 * The locatin of the vehicle navigating to the waypoints.
	 */
	Waypoint vehicle;
};

#endif