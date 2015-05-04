import lcm.lcm.*;
import avionics.*;

public class motorSend{
int left; 
int right; 
boolean enabled; 
long time;
LCM lcm; 
motor_command_t comm; 
	public motorSend(){
	System.out.println("Sending motor commands");
	lcm=LCM.getSingleton();;
	comm=new motor_command_t();
	}
public void sendMotorCommand(int LM, int RM){
		System.out.println("running");
	comm.timestamp=System.nanoTime();
	comm.L_power=LM;
	comm.R_power=RM;
	for(;;){
	lcm.publish("MOTOR", comm);
		}
	}
public static void main(String[] args){
	motorSend motor=new motorSend();
	motor.sendMotorCommand(5,5);
	}
}
