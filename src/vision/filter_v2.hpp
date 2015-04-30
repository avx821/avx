#include<math.h>
#include<stdlib.h>
#include<assert.h>
#include<iostream>
#include <ctime>

unsigned long long get_timestamp_a();
double ema_x(double new_x, unsigned long long new_time);
void set_power(double L, double R);
void reset_PID();
void thrust_x(double new_x, unsigned long long new_time);
double ema_y(double new_y, double new_time);
void PID(double filter_x);
void run_filter(double X_VALUE, unsigned long long TIME);
