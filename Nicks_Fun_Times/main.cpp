#include <iostream>
#include <exception>
#include <vector>

#include "lcm/lcm-cpp.hpp"

#include "VehicleState.h"
#include "WaypointManager.h"

/**
 * This main method reads vehicle state updates and publishes
 * autoflight instructions for waypoint navigation. 
 */
int main( int argc, const char* argv[] )
{
	// We check to ensure LCM is funtioning properly.
	lcm::LCM lcm;
    if(!lcm.good()) {
        return 1;
    }

	// We instantiate an understanding of the vehicle's current state.
	VehicleState myVehicle;

	// We also store a list of our current waypoints for navigation.
	WaypointManager myWaypoints;
	myWaypoints.setVehicle(myVehicle);

	//
	// TODO: insert preselected waypoints in
	// waypoint manager for testing.
	//

	// We need to handle LCM messages to get GPS and IMU updates.
	MessageHandler handler;
	handler.setVehicle(myVehicle);

	//
	// TODO: loop publish autoflight instructions
	//
	
	return 0;
}

/**
 * Classs for handling LCM messages and parsing GPS and IMU data.
 */
class MessageHandler {
    public:

    	/**
    	 * Constructor.
    	 */
    	Handler() {}

    	/**
    	 * Destructor.
    	 */
        ~Handler() {}

		/**
	 	 * Sets the vehicle to update with new positions.  
	 	 *
	 	 * @param vehicle The vehicle to update.
	 	 */
        void setVehicle(VehicleState vehicle) {
        	toUpdate = vehicle;
        }

        /**
         * Parse GPS and IMU data to update the vehicle state.
         */
        void handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const avionics::naviodata_t* msg)
        {
            //parse gps and imu data here...
            
        }

    private:

    	/**
    	 * Stores the Vehicle State to update.
    	 */ 
    	VehicleState toUpdate;
};



