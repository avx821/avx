/*
    Ground Station code for 16.821 testing purposes.
    @author Matt Cole (mcole@mit.edu)
*/

//Inclusions for lcm
#include "lcm/lcm-cpp.hpp"
#include "../lcm_headers/cpp/avionics/pod_data_t.hpp"

using namespace std;

bool haveData;
avionics::pod_data_t oldData;

int main(int argc, char ** argv)
{
    //initialize lcm
    lcm::LCM lcm;
    if(!lcm.good())
        return 1;

    haveData = false;

    while(true)
    {
	int entry = 0;
	cout << "Enter command type: [int]" << endl;
	    cin >> entry
	if(entry==0||entry>2) {
	    cout << "Command not recognized\n";
	    help();
	}
	switch(entry) {
	    case 1: modeOverride();
		    break;
	    case 2: sendPodData();
		    break;
	    default: cout << "input error\n";
	}
    }	
    return 0;
}

unsigned long long get_timestamp(){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long long millisecondsSinceEpoch=
	(unsigned long long)(tv.tv_sec)*1000+
	(unsigned long long)(tv.tv_usec)/1000;
}


void help() {
    cout << "Command types:\n" << "1: Mode Override\n" << "2: Send Pod Data\n";
}

void modeOverride() {
    int over = 0;
    cout << "Enter override: [int]" << endl;
    cin >> over;
    if(over==0||over>1) {
	cout << "Command not recognized\n";
	return;
    }
    lcm.publish("OVERRIDE", over);
}

void sendPodData() {
    avionics::pod_data_t podData;
    unsigned long long timestamp;
    timestamp = get_timestamp();
    double gpsLat, gpsLon, heading, wind1, wind2;
    bool wave, useOld;
    if(haveData) {
        cout << "Use data from last time? [boolean 0/1]" << endl;
        useOld = 0;
        cin >> useOld;
        if (useOld == 1) {
	    lcm.publish("pod_data",OldData);
	    return
	}
    }
    cout << "Enter pod data. Note that values are unchecked." << endl;
    cout << "Enter [gps lat], [SPACE], [gps lon]" << endl;
    cin >> gpsLat >> gpsLon;
    cout << "Enter pod heading [degrees]" << endl;
    cin >> heading;
    cout << "Enter [wind knots], [SPACE], [wind degrees]" << endl;
    cin >> wind1 >> wind2;
    cout << "Enter pod_wave_off: [boolean 0/1]" << endl;
    cin >> wave;

    podData.timestamp = timestamp;
    podData.gps[] = [gpsLat, gpsLon];
    podData.pod_heading = heading;
    podData.wind_data[] = [wind1, wind2];
    podData.pod_wave_off = wave == 1;
    oldData = podData;
    haveData = true;
    lcm.publish("pod_data",podData);
}

