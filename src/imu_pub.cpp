/*
    This program is designed to collect data from the Navio
    and pubish it via lcm to the channel imu. It uses the
    navio_imu_t.lcm data type.
*/

//Inclusions for lcm
#include "lcm/lcm-cpp.hpp"
#include "../types/avionics/navio_imu_t.hpp"

//Inclusions for time
#include <ctime>
#include <sys/time.h>

//Inclusions for IMU
#include "/home/pi/Navio/C++/Navio/MPU9250.h"
#include <iostream>

unsigned long long get_timestamp(){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long long millisecondsSinceEpoch=
	(unsigned long long)(tv.tv_sec)*1000+
	(unsigned long long)(tv.tv_usec)/1000;
}

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

    //Initialize data variable
    avionics::navio_imu_t data;

        while (true)
        {
		//Store time
		unsigned long long timestamp;
		timestamp = get_timestamp();
		data.timestamp = timestamp;

		//Store IMU data
		imu.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
		data.imu_pos[0] = (long) (mx*1000.0);
		data.imu_pos[1] = (long) (my*1000.0);
		data.imu_pos[2] = (long) (mz*1000.0);
		data.imu_vel[0] =  gx;
		data.imu_vel[1] =  gy;
		data.imu_vel[2] =  gz;
		data.imu_acc[0] =  ax;
		data.imu_acc[1] =  ay;
		data.imu_acc[2] =  az;
		//cout << "imu_pos[] " << (double) data.imu_pos[0] <<" "<< (double) data.imu_pos[1]<<"\n";

	    //Publish it!
	    lcm.publish("imu", &data);
	    //Should be 0.5 for normal operation
	    sleep(0.5);

	    //Default IMU sleep duration
	    //sleep(0.05);
	}

    return 0;
}
