#ifndef _MOTOR_CONTROL_AVIONICS
#define _MOTOR_CONTROL_AVIONICS 1

// Herbert declarations
#include <iostream>
#include "PCA9685.h"
#include <lcm/lcm-cpp.hpp>
#include "I2Cdev.cpp"
#include "../lcm_headers/cpp/avionics/motor_command_t.hpp"
#include "../lcm_headers/cpp/avionics/state_machine_mode_t.hpp"


#define NAVIO_RCOUTPUT_1 3
#define NAVIO_RCOUTPUT_2 4
#define NAVIO_RCOUTPUT_3 5

#define SERVO_MIN 1.250 /*mS*/
#define SERVO_MAX 1.750 /*mS*/
int runMotors(double, double);

extern PCA9685 pwm;
#endif
