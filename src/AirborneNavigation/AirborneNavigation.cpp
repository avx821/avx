#include <vector>
#include <iostream>

#include "lcm/lcm-cpp.hpp"

#include "../lcm_headers/cpp/avionics/navio_gps_t.hpp"
#include "../lcm_headers/cpp/avionics/waypoint_t.hpp"

#include "Waypoint.h"
#include "WaypointManager.h"

using namespace std;

/**
 * Classs for managing airborne navigation.
 */
class AirborneNavigation {
public:

    /**
     * Constructor.
     */
    AirborneNavigation() {}

    /**
     * Destructor.
     */
    ~AirborneNavigation() {}

    /**
     * Sets the mission waypoints.
     *
     * @param waypoints A vector of waypoint in the order that
     *        they should be hit by the aircraft.
     */
    void setWaypoints(std::vector<Waypoint> waypoints) {
        manager.setWaypoints(waypoints);
    }

    /**
     * Parse GPS and IMU data to update the vehicle state.
     *
     * @param rbuf The LCM message buffer.
     * @param chan The LCM channel to listen to.
     * @param message The LCM message to parse.
     */
    void handleMessage(const lcm::ReceiveBuffer* rbuf,
                       const std::string& chan, 
                       const avionics::navio_gps_t* message) {

        // Update vehicle location.
        vehicle.setWaypoint(message->lat, 
                            message->lon, 
                            message->alt);    

        // Feed location to the waypoint manager.
        manager.setVehicleLocation(vehicle);

        // Publish the next waypoint to the autopilot.
        target = manager.getCurrentWaypoint();
        std::vector<double> goal = target.getWaypoint();
        data.gps[0] = goal[0];
        data.gps[1] = goal[1];
        data.gps[2] = goal[2];

	cout << "Reported Location: Lat: " << message->lat << " Lon: " << message->lon << "Alt: " << message->alt << endl;
	cout << "Target Location: Lat: " << goal[0] << " Lon: " << goal[1] << " Alt: " << goal[2] << endl << endl;
    }

    /**
     * Accessor for the LCM message containing the current mission waypoint.
     */
    avionics::waypoint_t getData() {
        return data;
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
        avionics::waypoint_t data;
};

/**
 * Reads vehicle position and publishes autopilot instructions.
 */
int main(int argc, const char* argv[]) {

	// Spin up LCM.
	lcm::LCM lcm;
	if(!lcm.good()) {
        	return 1;
    }

    // Instantiate test mission waypoints.
    Waypoint waypoint_1;
    Waypoint waypoint_2;
    Waypoint waypoint_3;

    // Set mission waypoints. Currently set for testing.
    waypoint_1.setWaypoint(42.359053, -71.093507, 0);   
    waypoint_2.setWaypoint(42.356998, -71.092417, 0);
    waypoint_3.setWaypoint(42.360115, -71.094691, 0); 

    // Spin up a navigator and give it the waypoints.
    AirborneNavigation navigator;
    std::vector<Waypoint> waypoints;
    waypoints.push_back(waypoint_1);
    waypoints.push_back(waypoint_2);
    waypoints.push_back(waypoint_3);
    navigator.setWaypoints(waypoints);

    // Subscribe to Navio+ GPS publishing channel.
    lcm.subscribe("gps", &AirborneNavigation::handleMessage, &navigator);
		
    // Read LCM messages until we recieve an error.
    avionics::waypoint_t myData;
    while(lcm.handle() == 0) {
	myData = navigator.getData();
        lcm.publish("waypoint", &myData);
        sleep(0.2);
    }
	return 0;
}
