/*
    This program is designed to collect data from the Navio
    and pubish it via lcm to the channel gps. It uses the
    navio_gps_t.lcm data type.
*/

//Inclusions for lcm
#include "lcm/lcm-cpp.hpp"
#include "navio_gps_t.hpp"

//Inclusions for time
#include <ctime>
#include <sys/time.h>

//Inclusions for GPS
#include "Navio/Ublox.h"

using namespace std;

unsigned long long get_timestamp(){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long long millisecondsSinceEpoch=
	(unsigned long long)(tv.tv_sec)*1000+
	(unsigned long long)(tv.tv_usec)/1000;
}

int main(int argc, char ** argv)
{
    //initialize lcm
    lcm::LCM lcm;
    if(!lcm.good())
        return 1;

    //initialize GPS
    Ublox gps;
    std::vector<double> pos_data;

    //Initialize data variable
    avionics::navio_gps_t data;

	//This will wait for good gps data. Note that it could fail and return
	//false
	if(gps.testConnection())
	{
		while (true)
		{
			//Store time
			unsigned long long timestamp;
			timestamp = get_timestamp();
			data.timestamp = timestamp;

			//Store GPS
			if(gps.decodeSingleMessage(Ublox::NAV_POSLLH, pos_data)==1)
			{
				data.lon = pos_data[1]; //Longitude
				data.lat = pos_data[2]; //Latitude
				data.alt = pos_data[3]; //Altitude
			}
			else
			{
			    printf("Not Captured");
			}

		    //Publish it!
		    lcm.publish("gps", &data);
		    sleep(0.5);
		}
	}

    return 0;
}
