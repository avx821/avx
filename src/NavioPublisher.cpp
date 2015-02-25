/*
    This program is designed to collect data from the Navio
    and pubish it via lcm to the channel Navio. It uses the
    naviodata_t.lcm data type.
*/

//Inclusions for lcm
#include <lcm/lcm-cpp.hpp>
#include "avionics/naviodata_t.hpp"

//Inclusions for GPS
#include "Navio/Ublox.h"

//Inclusions for IMU
#include "Navio/MPU9250.h"

//What is this? I don't know.
using namespace std;

int main(int argc, char ** argv)
{
    //initialize lcm
    lcm::LCM lcm;
    if(!lcm.good())
        return 1;

    //initialize IMU
    MPU9250 imu;
    imu.initialize();
    float ax, ay, az, gx, gy, gz, mx, my, mz;

    //initialize GPS
    Ublox gps;
    std::vector<double> pos_data;

    //Initialize data variable
    avionics::naviodata_t data;

    if(gps.testConnection())
    {
	//Someone said while loops are bad? This is here b/c it was in the example.
	//Feel free to fix it.
        while (true)
        {
	    //Perhaps we should clear the data variable here?

            if (gps.decodeSingleMessage(Ublox::NAV_POSLLH, pos_data) == 1)
            {
		//Store GPS Data
		data.timestamp = pos_data[0];
		data.gps[0] = pos_data[1]; //Longitude
		data.gps[1] = pos_data[2]; //Latitude
		data.gps[2] = pos_data[3]; //Altitude

		//Store IMU data
		imu.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
		data.imu_pos[] = {mx, my, mz};
		data.imu_vel[] = {gx, gy, gz};
		data.imu_acc[] = {ax, ay, az};
	    } else {
		//We failed to get GPS data. Handle it.
	    }

	    //Publish it!
	    lcm.publish("Navio", &data);

	    //Default IMU sleep duration
	    sleep(0.5);
	}
    } else {
	//GPS connection bad. Handle it.
    }

    return 0;
}
