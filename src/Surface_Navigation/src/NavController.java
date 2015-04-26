import lcm.lcm.*;

public class NavController {
	boolean done=false;
	int right; 
	int left;
	final double _tolerance=1E-6; 
	final double MAX_VELOCITY=75; 
	final double MIN_VELOCITY=30;
	final double FAR_DISTANCE=50.0; 
	final double NEAR_DISTANCE=20.0;
	GPS_Navigation gpsnav;
	realignment gpsrealignment;
	Motor_command_t motor_msg; // change when lcm message is known
	
	public NavController(GPS_Navigation gpsnav) {
		lcm=LCM.getSingleton();
		motor_msg=new Motor_command_t();
		this.gpsnav=gpsnav;
	}
	
	public NavController(realignment gpsrealignment) {
		lcm=LCM.getSingleton();
		motor_msg=new Motor_command_t();
		this.gpsrealignment=gpsrealignment;
	}

	void publishMotorCommand(){
		motor_msg.timestamp = System.nanoTime();
		motor_msg.LM=left; 
		motor_msg.RM=right;
		lcm.publish("Motor",motor_msg);	
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
		if(delta_dist<=_tolerance){
			if(delta_heading<=_tolerance){
			this.done=true;
			}
		}
		else{
			this.done=false;
			speed=this.calcTransvel(delta_dist);
			omega=this.calcRotvel(delta_heading);
		}
	generateMotorCommand(speed, omega);
	publishMotorCommand();
	}
	
	double calcTransvel(double delta_dist){
		double trans_vel=0.0;
		if((delta_dist-FAR_DISTANCE)>_tolerance){
			trans_vel=MAX_VELOCITY;
		}
		else if(((delta_dist-NEAR_DISTANCE)>_tolerance) && ((delta_dist-FAR_DISTANCE)<=_tolerance)){
			trans_vel=MAX_VELOCITY/2.0;
		}
		else if((delta_dist-NEAR_DISTANCE)<=_tolerance){
			trans_vel=MIN_VELOCITY;
		}
		return trans_vel;
	}
	
	double calcRotvel(double delta_heading){
		return delta_heading*(MIN_VELOCITY/Math.PI);
	}
	void generateMotorCommand(double speed, double omega){
			right=(int) (speed/2.0+(omega/2.0)); 
			left= (int) (speed/2.0);
		}
		
}

