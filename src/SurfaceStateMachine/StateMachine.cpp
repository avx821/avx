#include <iostream>

#include "lcm/lcm-cpp.hpp"

#include "../lcm_headers/cpp/avionics/mode_status_t.hpp"
#include "../lcm_headers/cpp/avionics/state_machine_mode_t.hpp"

#include "ModeStatusListener.hpp"

using namespace std;

/**
 * This main method runs a state machine for determining surface navigation 
 * sate transitions.
 */
int main(int argc, const char* argv[]) {

    // The software starts in the GPS navigation mode.
	int current_mode = 1;

	// Spin up LCM.
	lcm::LCM lcm;
    if(!lcm.good()) {
        return 1;
    }

    // Spin up mode status listeners.
    ModeStatusListener mode1;
    ModeStatusListener mode2;

    // Subscribe to appropriate LCM channels
    lcm.subscribe("mode1_status", &ModeStatusListener::handleMessage, &mode1);
    lcm.subscribe("mode2_status", &ModeStatusListener::handleMessage, &mode2);
    avionics::state_machine_mode_t modes;

    int mode1_status = 0;
    int mode2_status = 0;

    // Read LCM messages until we recieve an error.
    while(lcm.handle() == 0) {

	mode1_status = mode1.getCurrentStatus();
	mode2_status = mode2.getCurrentStatus();

    	// Can only transition from GPS navigation to visual navigation.
        if (current_mode == 1) {
        	if (mode1_status == 2 || mode2_status == 2) {
			current_mode = 2;
		}
        }

	cout << "State Machine Status: " << current_mode << endl;
	cout << "Mode 1 Status: " << mode1_status << endl;
	cout << "Mode 2 Status: " << mode2_status << endl << endl;

    	// Update and publish status message.
    	modes.Mode_ID = current_mode;
        lcm.publish("state_machine_mode", &modes);
        sleep(0.2);
    }
	return 0;
}

