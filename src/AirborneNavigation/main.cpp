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
     */
    void setWaypoints(std::vector<Waypoint> waypoints) {
        manager.setWaypoints(waypoints);
    }

    /**
     * Parse GPS and IMU data to update the vehicle state.
     */
    void handleMessage(const lcm::ReceiveBuffer* rbuf,
                       const std::string& chan, 
                       const avionics::navio_gps_t* msg)
    {

        // Update vehicle location.
        vehicle.setWaypoint(msg->lat, 
                            msg->lon, 
                            msg->alt);    

        // Feed location to the waypoint manager.
        manager.setVehicleLocation(vehicle);

        // Publish the next waypoint to the autopilot.
        target = manager.getCurrentWaypoint();
        std::vector<double> goal = target.getWaypoint();
        data.gps[0] = goal[0];
        data.gps[1] = goal[1];
        data.gps[2] = goal[2];

        cout << "Current Location: Lat: " << msg->lat << " Lon: " << msg->lon << " Alt: " << msg->alt << endl;

    }

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
	cout << "Spinning up airborne navigation." << endl;
	// Spin up LCM.
	lcm::LCM lcm;
	if(!lcm.good()) {
        	return 1;
    }
	cout << "here1" << endl;
    // Instantiate test mission waypoints.
    Waypoint waypoint_1;
    Waypoint waypoint_2;
    Waypoint waypoint_3;

    // Set mission waypoints.
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
	cout << "here2" << endl;
    // Subscribe to Navio+ GPS publishing channel.
	lcm.subscribe("gps", &AirborneNavigation::handleMessage, &navigator);
	cout << "here2.5" << endl;
	cout << lcm.handle() << endl;	
	// Read LCM messages until we recieve an error.
	avionics::waypoint_t myData;
	while(lcm.handle() == 0) {
		cout << "here3" << endl;
		myData = navigator.getData();
        lcm.publish("waypoint", &myData);

        cout << "Published Location: Lat: " << myData.gps[0] << " Lon: " << myData.gps[1] << " Alt: " << myData.gps[2] << endl;
        sleep(0.2);
    	}
	return 0;
}

