
import lcm.lcm.*;
import avionics.*;
public class NavController {
	boolean done=false;
	int right; 
	int left;
	final double _tolerance=1E-6; 
	final double MAX_VELOCITY=10; 
	final double MIN_VELOCITY=2;
	final double FAR_DISTANCE=50.0; 
	final double NEAR_DISTANCE=20.0;
	motor_command_t motor_msg; // change when lcm message is known
	final int mode_id=1;
	LCM lcm;
	mode_status_t mode_msg; 
	public NavController(GPS_Navigation gpsnav) {
		lcm=LCM.getSingleton();
		motor_msg=new motor_command_t();
		mode_msg=new mode_status_t();
		mode_msg.Mode_ID=mode_id;
	}

	void publishMotorCommand(){
		motor_msg.timestamp = System.nanoTime();
		motor_msg.L_power=left; 
		motor_msg.R_power=right;
		//System.out.println("Publishing motor commands [L,R]: ["+motor_msg.L_power+", "+motor_msg.R_power+"]");
		lcm.publish("mode_1",motor_msg);	
	}
	boolean isDone(){
		return done; //Needs an actual end condition?
	}
	void setCommand(int r, int l){
		this.right=0;
		this.left=0;
	}
	
	void updateState(double delta_dist, double delta_heading){
		double speed=0.0; 
		double omega=0.0;
		if(delta_dist<=1.5){
			if(delta_heading<=0.2){
				speed=0.0;
				omega=0.0;
			this.done=true;
			}
		mode_msg.Status_ID=2;
		lcm.publish("mode1_status", mode_msg);
		}
		else{
			this.done=false;
			speed=this.calcTransvel(delta_dist);
			omega=this.calcRotvel(delta_heading);
			mode_msg.Status_ID=1;
		lcm.publish("mode1_status", mode_msg);
		}
	generateMotorCommand(speed, omega);
	publishMotorCommand();
	}
	
	double calcTransvel(double delta_dist){
		double trans_vel=0.0;
		if((delta_dist-FAR_DISTANCE)>_tolerance){
			trans_vel=MAX_VELOCITY/2.0;
		}
		else if(((delta_dist-NEAR_DISTANCE)>_tolerance) && ((delta_dist-FAR_DISTANCE)<=_tolerance)){
			trans_vel=MAX_VELOCITY/4.0;
		}
		else if((delta_dist-NEAR_DISTANCE)<=_tolerance){
			trans_vel=MIN_VELOCITY/4.0;
		}
		return trans_vel;
	}

	double calcRotvel(double delta_heading){
		return delta_heading*(MIN_VELOCITY/Math.PI);
	}
	void generateMotorCommand(double speed, double omega){
			right=(int) (speed/2.0);
			left= (int) (speed/2.0+(omega));
		}
}

