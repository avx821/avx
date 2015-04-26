import java.io.IOException;

public class realignment implements Runnable {
	LCM_PodData podComm; 
	LCMData sysComm;
	PointGPS current, start, dest; 
	waypointVector vec; 
	static double RADIUS=5; // assume 5 m radius, will be calculated later
	double heading=0.0; 
	double distance=0.0; 
	NavController controller; 
	final static double GPS_ACCURACY=1; // degree of accuracy of GPS in m
	final static double LNG_TO_METERS;				//Needs to be calculated based on latitude
	final static double LAT_TO_METERS=1.1132E5;		//Corrected value
	final static double _tolerance=1E-6; 
	double omega=0.0; 
	double speed=0.0;
	boolean Arrived=false;

	public realignment(LCM_PodData pComm, LCMData sComm){
		this.podComm=pComm;
		this.sysComm=sComm;
		// initialize aircraft start points and radius
	}
	waypointVector PointGPStoVector(PointGPS from, PointGPS to){
		double delta_x=(to.getLat()-from.getLat())*LAT_TO_METERS; 
		double delta_y=(to.getLng()-from.getLng())*LNG_TO_METERS;
		double delta_z=to.getAlt()-from.getAlt();
		return new waypointVector(delta_x,delta_y,delta_z);
	}
	public realignment(PointGPS start, PointGPS goal){
		this.start=start;
		this.dest=goal;
		this.controller=new NavController(this);
	}
	PointGPS calcWaypointfromPoint(double dist, PointGPS fp, double bearing){
		double delta_x=dist*Math.sin(bearing); 
		double delta_y=dist*Math.cos(bearing); 	
		double delta_phi=delta_y/LAT_TO_METERS; 
		double delta_lambda=delta_x/LNG_TO_METERS; 
		double new_x=fp.getLat()-delta_phi; 
		double new_y=fp.getLng()-delta_lambda; 
		return new PointGPS(new_x,new_y,0.0);  //0.0 isn't correct for altitude, but shouldn't matter
	}
	double getBearing(waypointVector u){
		return wrapAngle(Math.atan2(u.delta_y,u.delta_x));
	}
	// function that provides Angle wrap capability
	double wrapAngle(double angle){
		return angle%(2.0*Math.PI);
	}
	public PointGPS getCurrent() {
		return current;
	}
	public void setCurrent(PointGPS current) {
		this.current = current;
	}
	public PointGPS getStart() {
		return start;
	}
	public void setStart(PointGPS start) {
		this.start = start;
	}
	public PointGPS getDest() {
		return dest;
	}
	public void setDest(PointGPS dest) {
		this.dest = dest;
	}
	// calculate the relative bearing between two GPS points
	public double calcHaversineHeading(PointGPS to, PointGPS from){
		double delta_lat=to.getLat()-from.getLat();
		double delta_lon=to.getLng()-from.getLng(); 
		double a=(Math.sin(delta_lat/2.0)*Math.sin(delta_lat/2.0))+Math.cos(from.getLat())*Math.cos(to.getLat())*(Math.sin(delta_lon/2.0)*Math.sin(delta_lon/2.0));
		double c=2.0*Math.atan2(Math.sqrt(a),Math.sqrt(1-a));
		return c; 
	}
	@Override
	public void run() {
		this.current=this.start;
		synchronized(sysComm){

			if(sysComm!=null) //|| sysComm.isConnected())
			{
				// update these to format in LCM data types
				this.current=new PointGPS(sysComm.getLatitude(),sysComm.getLongitude(),sysComm.getAltitude()); 
				this.current.setHeading(sysComm.getHeading());
				System.out.println("Got GPS and heading data");
			}
			else{
				// get old value of current location from memory
			}
		}

		this.dest=this.calcWaypointfromPoint(RADIUS,this.dest,dest.heading);
		this.heading=calcHaversineHeading(this.current,dest);
		this.heading=wrapAngle(this.heading);
		this.vec=this.PointGPStoVector(current,dest);
		this.distance=Math.sqrt(this.vec.getDelta_x()*this.vec.getDelta_x() + this.vec.getDelta_y()*this.vec.getDelta_y());
		while(!controller.isDone()){
			long starttime=System.nanoTime();
			synchronized(sysComm){
				this.current.setHeading(sysComm.getHeading());
				this.current.setNewPoint(sysComm.getLatitude(),sysComm.getLongitude(),sysComm.getAltitude());
			}
			this.vec=this.PointGPStoVector(this.current,dest);
			this.distance=Math.sqrt(this.vec.getDelta_x()*this.vec.getDelta_x() + this.vec.getDelta_y()*this.vec.getDelta_y());
			this.heading=this.calcHaversineHeading(this.current,dest);
			double delta_heading=this.heading-dest.heading;
			delta_heading=wrapAngle(delta_heading);
			controller.updateState(distance,delta_heading);
			System.out.println("Cycle Time:"+(System.nanoTime()-starttime));
		}
		/*
		 **
		 * Uncomment when pod communication ability is available
		 * 		if(podComm!=null || podComm.isConnected()){	
		 this.dest=new PointGPS(podComm.getLatitude(),podComm.getLongitude(),podComm.getAltitude()); 
		}
		else{
			// get old value of pod location from memory
		}*/
	}

	public static void main(String[] args){

		LCMData sComm;
		try {
			sComm = new LCMData();
			sComm.run();
			try{
				Thread.sleep(500);
			}catch(Exception ex){
				System.out.println("SComm run error!");
			}
			// parse argument inputs for goal GPS point
			double Glat=Double.parseDouble(args[0]); 
			double Glng=Double.parseDouble(args[1]);
			LNG_TO_METERS = LAT_TO_METERS*Math.cos(Glat);		//Calculate the other GPS constant based on the latitude
			// initialize start and dest GPS points
			PointGPS goal=new PointGPS(Glat,Glng,0.0);
			PointGPS start=new PointGPS(sComm.getLatitude(), sComm.getLongitude(),sComm.getAltitude());
			
			//Calculate Middle target point
			double delta_lat = goal.getLat() - start.getLat();
			double delta_lng = goal.getLng() - start.getLng();
			double displacement = Math.sqrt(delta_lat*delta_lat - delta_lng*delta_lng);
			RADIUS = displacement;
			double anglediff = Math.atan2(delta_lat, delta_lng);
			
			double middle_lng = goal.getLng() + displacement*Math.sin(anglediff/2);
			double middle_lat = goal.getLat() + displacement*Math.cos(anglediff/2);
			
			double middle_to_end_lat = goal.getLat() - middle_lat ;
			double middle_to_end_lng = goal.getLng() - middle_lng;
			double middle_to_end_heading = Math.atan2(middle_to_end_lat, middle_to_end_lng);
			
			PointGPS middle = new PointGPS(middle_lat,middle_lng,sComm.getAltitude());
			// set heading for goal and start GPS points
			goal.setHeading(Math.toRadians(Double.parseDouble(args[3])));
			middle.setHeading(Math.toRadians(middle_to_end_heading));
			start.setHeading(Math.toRadians(sComm.getHeading()));
			// set limits for heading here or in set method
			
			//Run to the Middle point
			realignment WaypointNav=new realignment(start, middle);
			WaypointNav.run();
			
			//Run to the End Point 
			WaypointNav.dest = goal;
			WaypointNav.run();
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		/*Uncomment when pod comm ability is enabled
		 * LCM_PodData pComm=new LCM_PodData(); 
			pComm.run();*/
		//} catch (IOException e) {
		//	e.printStackTrace();
		//}
	}

}