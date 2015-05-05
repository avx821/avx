#ifndef MODE_STATUS_LISTENER_H
#define MODE_STATUS_LISTENER_H

#include "lcm/lcm-cpp.hpp"

#include "../lcm_headers/cpp/avionics/mode_status_t.hpp"

/**
 * Class for listening to surface navigation mode statuses.
 */
class ModeStatusListener {
public:

	/**
     * Constructor.
     */
    ModeStatusListener();

    /**
     * Destructor.
     */
    ~ModeStatusListener() {}

    /**
     * Parse surface navigation mode status messages.
     *
     * @param lcm::ReceiveBuffer* The LCM channel buffer.
     * @param std::string& The LCM channel.
     * @param mode_status_t* The LCM message struct.
     */
    void handleMessage(const lcm::ReceiveBuffer* rbuf,
                       const std::string& chan, 
                       const avionics::mode_status_t* message);

    /**
     * Returns the most recently reported relevant mode status.
     */
    int getCurrentStatus();

private:

    /**
     * The most recent reported active mode status.
     */
	int current_status;
};

#endif
