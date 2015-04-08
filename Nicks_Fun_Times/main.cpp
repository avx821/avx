#include <vector>

#include "lcm/lcm-cpp.hpp"
#include "naviodata_t.hpp"

#include "Waypoint.h"
#include "WaypointManager.h"
#include "Waypoint_t.hpp"

/**
 * Reads vehicle position and publishes autopilot instructions.
 */
int main(int argc, const char* argv[]) {

	// Spin up LCM.
	lcm::LCM lcm;
    if(!lcm.good()) {
        return 1;
    }

	// Subscribe to Navio GPS LCM message.
	MessageHandler handler;
	handler.setVehicle(myVehicle);
    lcm.subscribe("VehicleState", &Handler::handleMessage, &handler);

    // Read LCM messages until we recieve an error.
    while(lcm.handle() == 0) {
        // We keep handling messages until an error is thrown.
    }
	return 0;
}

/**
 * Classs for handling LCM messages and parsing GPS data.
 */
class MessageHandler {
public:

    /**
     * Constructor.
     */
    Handler() {

        // Instantiate test mission waypoints.
        Waypoint waypoint_1;
        Waypoint waypoint_2;
        Waypoint waypoint_3;

        // Set mission waypoints.


        // Feed waypoints into the waypoint manager.
        std::vector<Waypoint> waypoints;
        waypoints.push_back(waypoint_1);
        waypoints.push_back(waypoint_2);
        waypoints.push_back(waypoint_3);
        manager.setWaypoints(waypoints);
    }

    /**
     * Destructor.
     */
    ~Handler() {}

    /**
     * Parse GPS and IMU data to update the vehicle state.
     */
    void handleMessage(const lcm::ReceiveBuffer* rbuf,
                       const std::string& chan, 
                       const naviodata_t* msg)
    {

        // Update vehicle location.
        vehicle.setWaypoint(msg->gps[0], 
                            msg->gps[1], 
                            msg->gps[2]);    

        // Feed location to the waypoint manager.
        manager.setVehicleLocation(vehicle);

        // Publish the next waypoint to the autopilot.
        target = manager.getCurrentWaypoint();
        std::vector goal = target.getWaypoint();
        data.gps[0] = goal[0];
        data.gps[1] = goal[1];
        data.gps[2] = goal[2];
        
    }

private:

        /**
         * Stores the mission waypoints.
         */ 
        WaypointManager manager;

        /**
         * Stores the current target location.
         */
        Waypoint target;

        /**
         * Stores the vehicle's current location.
         */
        Waypoint vehicle;

        /**
         * LCM message storing the current target location.
         */
        Waypoint_t data;
};



