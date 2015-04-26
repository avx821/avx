import lcm.lcm.*;
public class MotorCommand {
	double omega; // rotational velocity
	double speed; // translational velocity
	LCM lcm;
	Motor_command_t motor_msg; // change when lcm message is known
	
	public MotorCommand(double omega, double speed) {
		lcm=LCM.getSingleton();
		motor_msg=new Motor_command_t();
		this.omega=omega; 
		this.speed=speed;
	}
	
	void publishMotorCommand(){
		motor_msg.timestamp = System.nanoTime();
		motor_msg.omega=omega; 
		motor_msg.speed=speed;
		lcm.publish("Motor",motor_msg);	
	}
}
