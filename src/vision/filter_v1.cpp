


///////////////////////////////////////////////////////////////////////////////////////////////////
//// Algorithm for Seaplane Docking                                                           ////
//// Filters x-, y-coordinate input from camera with exponential moving average				  ////
//// Computes thrust with proportional control												  ////
//// Alice Zielinski																		  ////
//// VERSION: 1.1																		  	  ////
//////////////////////////////////////////////////////////////////////////////////////////////////



// THIS VERSION IS ON RPI AVIONICS

#include "filter_v1.hpp"
using namespace std;

//extern
extern double WIDTH;	// frame width
extern double HEIGHT;	// frame height
extern double L_power;
extern double R_power;
extern double TAU;

// constants
const double R_DEF = 0.5;	// default R power
const double L_DEF = 0.5;	// default L power
const double DEADBAND = 5;

// global variables
double last_x = 0;
double last_time_x = 0;

// EMA function filters x_coordinate
double ema_x(double new_x, double new_time) {
	// time difference between new target x and last target x
	double dt = new_time - last_time_x;
	last_time_x = new_time;
	// compute alpha and filtered target x value
	double alpha = 1 - exp(-dt/TAU);
	double filter_x = last_x + alpha * (new_x - last_x);
	last_x = filter_x;
	return filter_x;
}

// EMA(X, tau)
void thrust_x(double new_x, double new_time){

	// CONDITIONS
	// 1) No new observation or last obs same as new obs: maintain current throttle
	// 2) Last obs == 0, new obs != 0: throttle is P to new obs
	// 3) Obs within deadband: set default throttle
	// 4) EMA filter target x-coordinate


	// Error checking
  	assert(TAU > 0);
  	assert(new_time >= 0);
  	assert(new_x >= 0);

  	// L and R throttle internal to thrust_x
  	double L = 0;
	double R = 0;
  
  	/////////////// CONDITION 1 /////////////////////
	// if no new observation or same observation, then maintain current throttle
	if (new_x == last_time_x || new_x == 0) {
		last_time_x = new_time;
		return;
	}
	/////////////// CONDITION 2 /////////////////////
	else if (last_x == 0) {
		L = new_x/WIDTH;
		R = 1 - L;

		// check that L is within [0,1]
		if(L < 0) {
			L_power = 0;
			R_power = 1;
		}
		else if (L > 1) {
			L_power = 1;
			R_power = 0;
		}
		// otherwise set computed throttle
		else {
			L_power = L;
			R_power = R;
		}
		// save new x coord and time value
		last_x = new_x;
		last_time_x = new_time;
	}

	/*
	/////////////// CONDITION 3 /////////////////////
	// observation is within deadband: set default throttle
	else if ((fabs(WIDTH/2 - new_x)) <= DEADBAND){
		// set default throttle
		L_power = L_DEF;
		R_power = R_DEF;
		//cout << "DEFAULT" << "\n";
		// save new x coord and time value
		last_x = new_x;
		last_time_x = new_time;
	}
	*/

	/////////////// CONDITION 4 /////////////////////
	// EMA filter target x-coordinate
	else {
		// EMA filter target x-coordinate
		double filter_x = ema_x(new_x, new_time);
		// check bounds of filtered value: ensure within image width
		// filter_x == last_x at this time
		if (filter_x > 0 && filter_x < WIDTH) {
			/////////////// CONDITION 3 /////////////////////
			// observation is within deadband: set default throttle
			if ((fabs(WIDTH/2 - filter_x)) <= DEADBAND){
				// set default throttle
				L = L_DEF;
				R = R_DEF;
				//cout << "DEFAULT" << "\n";
				// save new x coord and time value
				last_x = new_x;
				last_time_x = new_time;
			}
			else {
				L = last_x/WIDTH;
        		R = 1 - L;
        	}
        	//cout << "L POWER: " << L << "\n";

        	// ensure L within range [0, 1]
			if(L < 0) {
				L_power = 0;
				R_power = 1;
			}
			else if (L > 1) {
				L_power = 1;
				R_power = 0;
			}
			// L is within range, set computed throttle
			else {
				L_power = L;
				R_power = R;
			}
        }
        // filtered value is not within bounds of image width
		else {
		// set default power
			L_power = L_DEF;
			R_power = R_DEF;
		}
	}
}

/*
// main test for development & debugging
int main(){
	thrust_x(60, 503);
	cout << "X: " << last_x << "\n";
	cout << "Time: " << last_time_x << "\n";
	cout << "L: " << L_power << "\n";
	cout << "R: " << R_power << "\n";
	thrust_x(302, 839);
	cout << "X: " << last_x << "\n";
	cout << "Time: " << last_time_x << "\n";
	cout << "L: " << L_power << "\n";
	cout << "R: " << R_power << "\n";
	thrust_x(320, 938);
	cout << "X: " << last_x << "\n";
	cout << "Time: " << last_time_x << "\n";
	cout << "L: " << L_power << "\n";
	cout << "R: " << R_power << "\n";
	thrust_x(500, 1093);
	cout << "X: " << last_x << "\n";
	cout << "Time: " << last_time_x << "\n";
	cout << "L: " << L_power << "\n";
	cout << "R: " << R_power << "\n";
	return 0;
}*/