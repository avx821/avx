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
		data.imu_pos[0] = (double) mx;
		data.imu_pos[1] = (double) my;
		data.imu_pos[2] = (double) mz;
		data.imu_vel[0] = (double) gx;
		data.imu_vel[1] = (double) gy;
		data.imu_vel[2] = (double) gz;
		data.imu_acc[0] = (double) ax;
		data.imu_acc[1] = (double) ay;
		data.imu_acc[2] = (double) az;
		double d = 1.2345;
		d = (double) data.imu_pos[0];
		cout << "imu_pos[] " << (double) data.imu_pos[0]<<" "<< d <<"\n";

	    //Publish it!
	    lcm.publish("imu", &data);
<<<<<<< HEAD
	    //Should be 0.005 for normal operation
	    sleep(0.5);
=======

	    //Default IMU sleep duration
	    //sleep(0.05);
>>>>>>> 8c4827ba2ba1d9163b7840768d44df62c3fa9428
	}

    return 0;
}
