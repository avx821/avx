#include "motor_control.hpp"
#include "PCA9685.cpp"
#include "I2Cdev.cpp"

PCA9685 pwm;

int runMotors(double leftPercentInput, double rightPercentInput)
{
    int leftPercent=(int)100*sqrt((leftPercentInput/100));
    int rightPercent=(int)100*sqrt((rightPercentInput/100));

    pwm.setPWM(NAVIO_RCOUTPUT_1, 205+205*leftPercent/100);
    pwm.setPWM(NAVIO_RCOUTPUT_2, 205+205*rightPercent/100);

    return 0;
}