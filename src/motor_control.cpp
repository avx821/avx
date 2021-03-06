#include "motor_control.hpp"
#include "PCA9685.cpp"
#include "I2Cdev.cpp"
#include <iostream>

using namespace std;
PCA9685 pwm;
class Handler
{
	public:
		~Handler(){}
		
		void handleMessage(const lcm::ReceiveBuffer* rbug,
			const std::string& chan,
			const avionics::motor_command_t* msg)
		{
			runMotors(msg->L_power,msg->R_power);
			
		}
};
			

int main(int,char**)
{
    /*
    static const uint8_t outputEnablePin = RPI_GPIO_27;

    Pin pin(outputEnablePin);

    if (pin.init()) {
        pin.setMode(Pin::GpioModeOutput);
        pin.write(0); //drive Output Enable low 
    } else {
        fprintf(stderr, "Output Enable not set. Are you root?");
    }
    */
  //  system("sudo echo 27 > /sys/class/gpio/export");
//    system("sudo echo out > /sys/class/gpio/gpio27/direction");

    pwm.initialize();
    pwm.setFrequency(50);
    
    lcm::LCM lcm;
    if(!lcm.good()) return 2;
    
    avionics::motor_command_t command;
    
    //pwm.setPWM(NAVIO_RCOUTPUT_1, 205);       
    //pwm.setPWM(NAVIO_RCOUTPUT_2, 205);
    //sleep(1);
	

    Handler handlerObject;
    lcm.subscribe("MOTOR",&Handler::handleMessage,&handlerObject);
    
    while(0==lcm.handle());
    
    return 0;

}


int runMotors(double leftPercentInput, double rightPercentInput)
{
	
    int leftPercent=(int)100*sqrt((leftPercentInput/100));
    int rightPercent=(int)100*sqrt((rightPercentInput/100));

    pwm.setPWM(NAVIO_RCOUTPUT_1, 205+205*leftPercent/100);
    pwm.setPWM(NAVIO_RCOUTPUT_2, 205+205*rightPercent/100);

	// pwm.setPWM(NAVIO_RCOUTPUT_1, 250);       
//pwm.setPWM(NAVIO_RCOUTPUT_2, 250);


cout<<"called motor     "<<205+205*leftPercent/100<<"          "<<205+205*rightPercent/100<<endl;

    return 0;
}
