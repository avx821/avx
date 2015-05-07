
#include "motor_control.hpp"
#include "PCA9685.cpp"
#include "gpio.cpp"
#include <iostream>

using namespace std;
PCA9685 pwm;

class MotorHandler {
public:
    MotorHandler() {
        left = 0;
        right = 0;
    }

	~MotorHandler(){}
		
    double getLeftPower() {
        return left;
    }

    double getRightPower() {
        return right;
    }

    void handleMessage(const lcm::ReceiveBuffer* rbug,
		               const std::string& chan,
		               const avionics::motor_command_t* message) {
		left = message->L_power;
        right = message->R_power;
	}

private:
    double left;
    double right;
};

class StateMachineHandler {
public:
    StateMachineHandler() {
	currentMode = 0;
}

    ~StateMachineHandler() {}

    int getCurrentMode() {
        return currentMode;
    }

    void handleMessage(const lcm::ReceiveBuffer* rbug,
                       const std::string& chan,
                       const avionics::state_machine_mode_t* message) {
        currentMode = message->Mode_ID;
    }

private:
    int currentMode;
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
    system("sudo echo 27 > /sys/class/gpio/export");
    system("sudo echo out > /sys/class/gpio/gpio27/direction");

    pwm.initialize();
    pwm.setFrequency(50);
    
    lcm::LCM lcm;
    if(!lcm.good()) return 2;
    
    avionics::motor_command_t command;
    
    //pwm.setPWM(NAVIO_RCOUTPUT_1, 205);       
    //pwm.setPWM(NAVIO_RCOUTPUT_2, 205);
    //sleep(1);
	

    MotorHandler mode1;    
    MotorHandler mode2;

    lcm.subscribe("mode_1",&MotorHandler::handleMessage,&mode1);
    lcm.subscribe("mode_2",&MotorHandler::handleMessage,&mode2);

    StateMachineHandler state;

    lcm.subscribe("state_machine_mode",&StateMachineHandler::handleMessage,&state);
    
    int mode = 0;

    while(0==lcm.handle()) {
        mode = state.getCurrentMode();

        if (mode == 1) {
            runMotors(mode1.getLeftPower(),mode1.getRightPower());
        }
        if (mode == 2) {
            runMotors(mode2.getLeftPower(),mode2.getRightPower());            
        }

        cout << "State Machine Mode: " << mode << endl;
        cout << "GPS Navigation Motor Commands: Left: " << mode1.getLeftPower() << " Right: " << mode1.getRightPower() << endl;
        cout << "Visual Navigation Motor Commands: Left: " << mode2.getLeftPower() << " Right: " << mode2.getRightPower() << endl << endl;

    }
    return 0;
}


int runMotors(double leftPercentInput, double rightPercentInput)
{
	
    int leftPercent=(int)45*sqrt((leftPercentInput/100));
    int rightPercent=(int)45*sqrt((rightPercentInput/100));

    // motor
    pwm.setPWM(NAVIO_RCOUTPUT_1, 205+205*leftPercent/100);
    pwm.setPWM(NAVIO_RCOUTPUT_2, 205+205*rightPercent/100);
    //rudder
    double rightBalance=rightPercentInput/(leftPercentInput+rightPercentInput);
    pwm.setPWMmS(NAVIO_RCOUTPUT_3, SERVO_MIN + (SERVO_MAX-SERVO_MIN)*rightBalance);

	// pwm.setPWM(NAVIO_RCOUTPUT_1, 250);       
//pwm.setPWM(NAVIO_RCOUTPUT_2, 250);


cout<<"called motor     "<<205+205*leftPercent/100<<"          "<<205+205*rightPercent/100<<endl;

    return 0;
}
