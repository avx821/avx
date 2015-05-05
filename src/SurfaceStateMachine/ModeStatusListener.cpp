#include "ModeStatusListener.hpp"

ModeStatusListener::ModeStatusListener() {
	current_status = 0;
}

void ModeStatusListener::handleMessage(const lcm::ReceiveBuffer* rbuf,
                       			       const std::string& chan, 
                       			       const avionics::mode_status_t* message) {
	current_status = message->Status_ID;
}

int ModeStatusListener::getCurrentStatus() {
	return current_status;
}
