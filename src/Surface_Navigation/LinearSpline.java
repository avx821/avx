import java.util.ArrayList;


public class LinearSpline {
	double distance; // distance between start and destination waypoints in m
	ArrayList<PointGPS> waypoints; // list of waypoints between start and destination
	final static double GPS_ACCURACY=1; // degree of accuracy of GPS in m
	final static double LAT_TO_METERS=1.106E5;
	final static double LNG_TO_METERS=1.113E5;
	final static double VISUAL_RADIUS=10;
	final static double SPLINE_LENGTH=0.1; // 10 cm
	final static double _tolerance=1E-6; 
	double xt,xt_sq;
	waypointVector spline_c, spline_b,spline_a;
	PointGPS start,dest,f0;
	public LinearSpline(PointGPS start, PointGPS dest) {
		
		if(start==null || dest==null){
			throw new NullPointerException("Invalid GPS location(s)");
			}
		
		this.start=start; 
		this.dest=dest;
		waypointVector sd=PointGPStoVector(this.start, this.dest);
		this.distance=Math.sqrt(sd.getDelta_x()*sd.getDelta_x()+sd.getDelta_y()*sd.getDelta_y());
		if(distance<=GPS_ACCURACY){
				throw new IllegalArgumentException("You are already within GPS range...switch to visual servo mode or to next pod");
			}
		
		}
	
	double calculateHeight(){
		return (this.dest.getAlt()-this.start.getAlt()); 
	}
	// function that creates spline from waypoints and pod heading
	ArrayList<PointGPS> createSpline(PointGPS w0,PointGPS w1){
		// should I return points in spline or velocity to command?
		// for now, I will return points
		ArrayList<PointGPS> wp_list=new ArrayList<PointGPS>(); 
		xt=xt_sq=0.0;
		f0=this.start;
		double heading=Math.toRadians(f0.getHeading());
		spline_c=new waypointVector(0.01*Math.sin(heading),0.01*Math.cos(heading),0);
		// function initializes the first spline segment
		initializeSpline(w0,w1); 
		double num_steps=10.0; 
		// number of steps to create spline
		 for(int n=0; n<10;n++){
			 xt=1.0/num_steps *(double)n;
			 xt_sq=xt*xt;
			 // function that checks if cubic spline is continuous
			 wp_list.add(evaluate()); 
			 
		 }
		
		return wp_list; 
	}
	
	void initializeSpline(PointGPS w0,PointGPS w1){
		waypointVector vec=PointGPStoVector(w0,w1);
		double[] f1_prime={vec.delta_x/SPLINE_LENGTH,vec.delta_y/SPLINE_LENGTH};
		calculate spline_a, spline_b
		}
	
	
	void createWaypoints(){
		this.waypoints=new ArrayList<PointGPS>();
		PointGPS w0=this.start;
		this.waypoints.add(w0);
		PointGPS w1=null;
		if(Math.abs(this.distance-100)>=_tolerance){
			// set the next waypoint 50m away and gun it to that waypoint
			w0.setFlag(true,true,WaypointType.STRAIGHT);
			w1=calcWaypointfromPoint(50,this.dest); // sets the next waypoint 50m away from dest
		}
		else{
			w0.setFlag(true,true,WaypointType.STRAIGHT);
			w1=calcWaypointfromPoint(VISUAL_RADIUS,this.dest);
		}
		this.waypoints.add(w1);
		createSpline(w0,w1);
	}
	double getBearing(waypointVector u){
		return Math.atan2(u.delta_y,u.delta_x);
	}
	
	PointGPS vectortoPointGPS(waypointVector v, PointGPS from){
		double delta_phi=v.getDelta_x()/LAT_TO_METERS;
		double delta_lambda=v.getDelta_y()/LNG_TO_METERS;
		double to_x=from.getLat()+delta_phi; 
		double to_y=from.getLng()+delta_lambda; 
		return new PointGPS(to_x,to_y,from.getAlt());
	}
	waypointVector PointGPStoVector(PointGPS from, PointGPS to){
		double delta_x=(to.getLat()-from.getLat())*LAT_TO_METERS; 
		double delta_y=(to.getLng()-from.getLng())*LNG_TO_METERS;
		double delta_z=to.getAlt()-from.getAlt();
		return new waypointVector(delta_x,delta_y,delta_z);
	}
	PointGPS calcWaypointfromPoint(double dist, PointGPS fp){
		waypointVector u=PointGPStoVector(this.start,this.dest);
		double bearing=this.getBearing(u); 
		double delta_x=dist*Math.sin(bearing); 
		double delta_y=dist*Math.cos(bearing); 
		double delta_phi=delta_x/LAT_TO_METERS; 
		double delta_lambda=delta_y/LNG_TO_METERS; 
		double new_x=fp.getLat()-delta_phi; 
		double new_y=fp.getLng()-delta_lambda; 
		
		return new PointGPS(new_x,new_y,0); 
		
	}
	void evaluate(){
		double xt_cubed=xt_sq*xt;
		 get the next spline target
	}

}


