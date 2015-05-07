
import java.io.IOException;

public class GPS_Navigation implements Runnable {
	// PodLCMData podComm; 
	 LCMData sysComm;
	 PointGPS current, start, dest; 
	 waypointVector vec; 
	 final double RADIUS=5.0; // 10 m radius
	 double heading=0.0; 
	 double distance=0.0; 
	 NavController controller; 
	 final static double GPS_ACCURACY=1; // degree of accuracy of GPS in m
	 final static double LAT_TO_METERS=1.106E5;
	 final static double LNG_TO_METERS=LAT_TO_METERS;
	 final static double _tolerance=1E-6; 
	 double omega=0.0;
	 double speed=0.0;
	 double circle_heading=0.01;
	 boolean Arrived=false;
	
	public GPS_Navigation(LCMData sComm,PointGPS goal){
		this.sysComm=sComm;
		this.start=new PointGPS(sComm.getLatitude(), sComm.getLongitude(),0.0);
		start.setHeading(Math.toRadians(sComm.getHeading()));
		this.dest=goal;
		this.controller=new NavController(this);
		//this.podComm=null;
	}
	/*public GPS_Navigation(LCMData sComm,PodLCMData pComm){
		this.sysComm=sComm;
		this.podComm=pComm;
		this.start=new PointGPS(sComm.getLatitude(), sComm.getLongitude(),0.0);
		start.setHeading(Math.toRadians(sComm.getHeading()));
		this.dest=new PointGPS(pComm.getLatitude(), pComm.getLongitude(),0.0);
		dest.setHeading(Math.toRadians(pComm.getHeading()));
		this.controller=new NavController(this);
	}*/
	waypointVector PointGPStoVector(PointGPS from, PointGPS to){
		double delta_x=(to.getLat()-from.getLat())*LAT_TO_METERS; 
		double delta_y=(to.getLng()-from.getLng())*LNG_TO_METERS;
		double delta_z=to.getAlt()-from.getAlt();
		return new waypointVector(delta_x,delta_y,delta_z);
	}

	PointGPS calcWaypointfromPoint(double dist, PointGPS fp, double bearing){
		double delta_x=dist*Math.sin(bearing);
		double delta_y=dist*Math.cos(bearing);
		double delta_phi=delta_y/LAT_TO_METERS; 
		double delta_lambda=delta_x/LNG_TO_METERS; 
		double new_x=fp.getLat()-delta_phi; 
		double new_y=fp.getLng()-delta_lambda; 
		return new PointGPS(new_x,new_y,0.0); 
	}
	// function that provides Angle wrap capability
	double wrapAngle(double angle){
			angle=angle%(2.0*Math.PI); 
			if(angle>Math.PI){
				angle=(2.0*Math.PI)-angle; 
			}
			else if(angle<-Math.PI){
				angle=(2.0*Math.PI)+angle;
			}
		return angle; 
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
	@Override
	public void run() {
		this.current=this.start;	
		this.dest=this.calcWaypointfromPoint(RADIUS,this.dest,dest.heading);
		this.vec=this.PointGPStoVector(current,dest);
		this.heading=Math.atan2(this.vec.getDelta_x(), this.vec.getDelta_y());
		this.distance=Math.sqrt(this.vec.getDelta_x()*this.vec.getDelta_x() + this.vec.getDelta_y()*this.vec.getDelta_y());
		while(!controller.isDone()){
			long starttime=System.nanoTime();
			synchronized(sysComm){
				if(sysComm!=null){
				this.current.setHeading(sysComm.getHeading());
				this.current.setNewPoint(sysComm.getLatitude(),sysComm.getLongitude(),sysComm.getAltitude());
				}
			}
			this.vec=this.PointGPStoVector(this.current,dest);
			this.distance=Math.sqrt(this.vec.getDelta_x()*this.vec.getDelta_x() + this.vec.getDelta_y()*this.vec.getDelta_y());
			double delta_heading=this.heading-current.heading;
			delta_heading=wrapAngle(delta_heading);
		//	System.out.println("Distance:" +this.distance+", Heading: "+(this.current.getHeading()*180/Math.PI)+", delta_heading: "+(delta_heading*180/Math.PI));
			controller.updateState(distance,delta_heading);
		}
		/*while(controller.isDone()){
			driveCircle();
			try{
			Thread.sleep(10);
			}catch(InterruptedException ex){
				ex.printStackTrace();
			}
		}*/
	}
	
	public void driveCircle(){
			controller.updateState(0.0,circle_heading); 
			circle_heading++;
	}

			
	 public static void main(String[] args){
		 	try {
				if(args.length<4){
				// parse argument inputs for goal GPS point
				double Glat=Double.parseDouble(args[0]); 
				double Glng=Double.parseDouble(args[1]); 
				// initialize start and dest GPS points
				PointGPS goal=new PointGPS(Glat,Glng,0.0);
				// set heading for goal and start GPS points
				goal.setHeading(Math.toRadians(Double.parseDouble(args[2])));
				LCMData sComm= new LCMData();		
				// set limits for heading here or in set method
				Thread sysThread=new Thread(sComm);
				//HeadingCalibration cal=new HeadingCalibration(filter,sysComm);
				GPS_Navigation WaypointNav=new GPS_Navigation(sComm,goal);
				 Thread navThread=new Thread(WaypointNav);
				 sComm.setNav(WaypointNav);
				 sysThread.start();
				navThread.start();
			}
			/*else {
				LCMData sComm= new LCMData();	
				PodLCMData pComm=new PodLCMData();	
				// set limits for heading here or in set method
				Thread sysThread=new Thread(sComm);
				Thread podThread=new Thread(pComm);
				//HeadingCalibration cal=new HeadingCalibration(filter,sysComm);
				GPS_Navigation WaypointNav=new GPS_Navigation(sComm,pComm);
				 Thread navThread=new Thread(WaypointNav);
				 sComm.setNav(WaypointNav);
				 sysThread.start();
				 podThread.start();
				navThread.start();
				
			}*/
			/*Uncomment when pod comm ability is enabled
			 * LCM_PodData pComm=new LCM_PodData(); 
			pComm.run();*/
		 	} catch (IOException e) {
			// TODO Auto-generated catch block
		e.printStackTrace();
		}
		
	 }
}

		  
