


///////////////////////////////////////////////////////////////////////////////////////////////////
//// Algorithm for Seaplane Docking                                                           ////
//// Filters x-, y-coordinate input from camera with exponential moving average				  ////
//// Computes thrust with PID control														  ////
//// Alice Zielinski																		  ////
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "filter_v2.hpp"
#include <sys/time.h>
#include <ctime>
#include <math.h>
#include <assert.h>
#include <iostream>
using namespace std;
 
//extern variables
extern double WIDTH;	// frame width
extern double HEIGHT;	// frame height
extern double L_power;
extern double R_power;


// Dev variables
/*
double WIDTH = 600;
double HEIGHT = 800;
double L_power = 0;
double R_power = 0;
*/


// constants
const double R_DEF = 0.5;	// default R power
const double L_DEF = 0.5;	// default L power
const double P_MAX = 0.5;
const double P_MIN = 0.1;	// min power to overcome torque
const double TAU = 1000;
const double Kp = 1/(WIDTH/2);
const double Ki = 0.000002;
const double Kd = 0.002;

// Global variables
// thrust_x vars
double last_x = 0;
double last_y = 0;
unsigned long long last_time_x = 0;
double last_time_y = 0;
unsigned long long now_time = 0;
unsigned long long timeout = 3;


// PID vars
double deadBand = 3.125/100*WIDTH;//20; // 3.125% of WIDTH
double error = 0;
double error_last = 0;
double integral = 0;
double derivative = 0;

// Time
unsigned long long get_timestamp_filter_a() {
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long long millisecondsSinceEpoch = 0;
	return millisecondsSinceEpoch=
	(unsigned long long)(tv.tv_sec)*1000+
	(unsigned long long)(tv.tv_usec)/1000;
}


// Function filters x_coordinate
double ema_x(double new_x, double new_time) {
	double dt = new_time - last_time_x;
	double alpha = 1 - exp(-dt/TAU);
	double filter_x = last_x + alpha * (new_x - last_x);
	last_x = filter_x;
	return filter_x;
}

// Sets throttle to specificed settings
void set_power(double L, double R) {
	// check: throttle settings within bounds
	if((L >= 0) && (R >= 0) && (L + R <= 1.2)) {
		L_power = L;
		R_power = R;
	}
}

// Reset PID memory
void reset_PID(){
	error = 0;
	error_last = 0;
	integral = 0;
	derivative = 0;
}

// Exponential moving average
// EMA(X, tau)
void thrust_x(double new_x, double new_time){
	//cout << "ENTERED THRUST COMPUTATION \n";

	// CONDITIONS
	// 1) No new observation or last obs same as new obs: maintain current throttle
	// 2) Last obs == 0, new obs != 0: throttle is P to new obs
	// 3) Obs within deadband: set default throttle
	// 4) EMA filter target x-coordinate

	// Error checking
  	assert(TAU > 0);
  	assert(new_x >= 0);
  	assert(new_time >= 0);

  	double L = 0;
  	double R = 0;

  	double L_set = 0;
  	double R_set = 0;
  
  	/////////////// CONDITION 1 /////////////////////
	// if no new observation, then maintain prior power
	if (new_time == last_time_x || new_x == 0) {
		//cout << "CONDITION 1" << "\n";
		last_time_x = new_time;
		return;
	}
	/////////////// CONDITION 2 /////////////////////
	else if (last_x == 0) {
		//cout << "CONDITION 2" << "\n";
		// proportional throttle control
		L = new_x/WIDTH;
		R = 1 - L;

		// check that L is within [0,1]
		if(L < 0) {
			L_set = 0;
			R_set = 1;
		}
		else if (L > 1) {
			L_set = 1;
			R_set = 0;
		}
		// otherwise set computed throttle
		else {
			L_set = L;
			R_set = R;
		}
		last_x = new_x;
		last_time_x = new_time;
		set_power(L_set, R_set);
	}
	/////////////// CONDITION FILTER /////////////////////
	// EMA filter target x-coordinate
	else {
	//	cout << "FILTER" << "\n";
		// EMA filter target x-coordinate
		double filter_x = ema_x(new_x, new_time);
		// compute error between filtered x and desired heading (center of image)
		double error = filter_x - WIDTH/2;
		// check bounds of filtered value: ensure within image width
		// filter_x == last_x at this time
		if (filter_x > 0 && filter_x < WIDTH) {
			/////////////// CONDITION 3 /////////////////////
			// observation is within deadband: set default throttle
			if((fabs(error)) <= deadBand) {
			//	cout << "DEADBAND" << "\n";
				// reset PID memory
				reset_PID();
				// set default throttle
				L = L_DEF;
				R = R_DEF;
				set_power(L, R);
				// save new x-coord and time value
				last_x = new_time;
				last_time_x = new_time;
			}
			// otherwise PID control
			else {
				//cout << "PID CONTROL" << "\n";
				double dt = new_time - last_time_x;
				integral = integral + error * dt;
				//cout << "integral: " << integral << "\n";
				double de = error - error_last;
				if (fabs(de) > 20){
					derivative = de / dt;
					//cout << "de: " << de << "\n";
					//cout << "dt: " << dt << "\n";
					//cout << "derivative: " << derivative << "\n";
				}
				//cout << "Error: " << error << "\n";
				double response = ((Kp * error) + (Ki * integral) + (Kd * derivative));
				//cout << "response: " << response << "\n";

				// check: abs of response is less than max possible throttle
				if (fabs(response) <= 1.1) {
					double L_prelim = L_DEF + response;
					//cout << "L_prelim: " << L_prelim << "\n";
					double R_prelim = 1 - L_prelim;
					//cout << "R_prelim: " << R_prelim << "\n";
					//cout << "R_prelim + L_prelim = " << L_prelim + R_prelim << "\n";
					// ensure L and R within bounds [0, 1]
					if(L_prelim < 0.05){
					//	cout << "L_prelim < 0" << "\n";
						L = 0;
						R = 1;
					}
					else if(R_prelim < 0.05){
						L = 1;
						R = 0;
					}
					else if(L_prelim > 1){
						L = 1;
						R = 0;
					}
					else if(R_prelim > 1){
						L = 0;
						R = 1;
					}
					// ensure L & R throttle sum within bounds
					else if ((L_prelim + R_prelim <= 1.2)) {
						//cout << "RESPONSE WITHIN BOUND" << "\n";
						L = L_prelim;
						R = R_prelim;
					}
					// otherwise maintain current power
					else {
					//	cout << "THIS IS NOT GOOD" << "\n";
						L = L_power;
						R = R_power;
					}
					set_power(L,R);
				}
				// otherwise: rest PID and set default throttle
				else {
					reset_PID();
				/*
					L = L_DEF;
					R = R_DEF;
					set_power(L, R);
				*/
					// or, actually, maintain throttle
				}
			error_last = error;
			last_time_x = new_time;
			}
        }

        else {
        	// set default power
        	L = L_DEF;
        	R = R_DEF;
        	set_power(L, R);
        }
	}
}

void run_filter(double X_VALUE, unsigned long long  TIME) {
	now_time = get_timestamp_filter_a();
	//cout << (now_time - last_time_x) << "\n";
	if(now_time - last_time_x <= timeout){
		thrust_x(X_VALUE, TIME);
	}
	else{
		//cout << "TIME OUT \n";
		set_power(0,0);
		last_time_x = 0;
	}
}

// Dev main
/*
int main() {
	last_time_x = get_timestamp()-300;
	cout << "Running 1 \n";
	run_filter(300,get_timestamp()-200);
	cout << "Running 2 \n";
	run_filter(500,get_timestamp()-100);
	run_filter(400, get_timestamp()-50);
	run_filter(300, get_timestamp());

}
*/
















