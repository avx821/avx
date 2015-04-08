import java.io.IOException;
import java.util.ArrayList;

import javafx.scene.shape.Circle;



public class GPS_Navigation implements Runnable {
	 LCM_PodData podComm; 
	 LCMData sysComm;
	 Aircraft dragonfly; 
	 NavStateHandler stateHandler; 
	 PointGPS current, start, dest; 
	 ArrayList<PointGPS> waypoints;
	 final double RADIUS=2; // 2 m radius
	 
	public GPS_Navigation(LCM_PodData pComm, LCMData sComm){
	
		this.podComm=pComm;
		this.sysComm=sComm;
		this.dragonfly=new Aircraft();
		// initialize aircraft start points and radius
        this.stateHandler=new NavStateHandler(NavState.READY);
	}
	
	
	public GPS_Navigation(PointGPS start, PointGPS goal){
		this.start=start;
		this.dest=goal;
		this.dragonfly.setParameters(this.start.getLat(),this.start.getLng(),RADIUS);
        this.stateHandler=new NavStateHandler(NavState.READY); // waits for spline update
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
		
		 if(sysComm!=null || sysComm.isConnected())
		{
			// update these to format in LCM data types
			this.current=new PointGPS(sysComm.getLatitude(),sysComm.getLongitude(),sysComm.getAltitude()); 
			this.current.setHeading(sysComm.getHeading());
		}
		else{
			// get old value of current location from memory
		}
/*
 * Uncomment when pod communication ability is available
 * 		if(podComm!=null || podComm.isConnected()){	
 
		 this.dest=new PointGPS(podComm.getLatitude(),podComm.getLongitude(),podComm.getAltitude()); 
		}
		else{
			// get old value of pod location from memory
		}*/
		 
		LinearSpline spline=new LinearSpline(this.current,this.dest);
		waypoints=spline.createWaypoints();
		// Plot waypoints here with plotter
		if(waypoints.isEmpty()|| waypoints==null){
			try {
				throw new InterruptedException("No waypoints generated...GPS Navigation mode fail");
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}	
		stateHandler.setState(NavState.START);
			while(stateHandler.state!=NavState.ARRIVED ) // or gave up but this will be implemented later
			{
				// update current position
				synchronized(sysComm){
					this.dragonfly.updatePosition(sysComm.getLatitude(), sysComm.getLongitude());
				}
				// might update too fast...we want to give time for controller to run
				Thread.sleep(200);
				// get furthest point
				PointGPS target=this.getFurthestPoint();
				stateHandler.updateState(target);
				
			}
		
		
		
		
	}
	public PointGPS getFurthestPoint(){
		double max_dist=0.0;
		PointGPS furthest=null;
		int listnum=0;
		for(PointGPS point:waypoints){
			if(dragonfly.intersects(point.getLat(), point.getLng())){
				double dist=this.calculateDistance(dragonfly.getCurrentPos(), point);
				if(dist>max_dist){
					furthest=point;
					listnum=waypoints.indexOf(point);
				}
			}
		}
	// removes up to n-1 of furthest point for robustness
		this.waypoints=(ArrayList) this.waypoints.subList(listnum-1, waypoints.size()-1);
		return furthest;
	}
double calculateDistance(PointGPS from, PointGPS to){
		double delta_x=(to.getLat()-from.getLat())*LinearSpline.LAT_TO_METERS; 
		double delta_y=(to.getLng()-from.getLng())*LinearSpline.LNG_TO_METERS;
		double delta_z=to.getAlt()-from.getAlt();
		return Math.sqrt(delta_x*delta_x+delta_y*delta_y);
	}
	
	 public static void main(String[] args){
		
	//	try {
			LCMData sComm;
			try {
				sComm = new LCMData();
				sComm.run();
				double Glat=Double.parseDouble(args[0]); 
				double Glng=Double.parseDouble(args[1]); 
				double Galt=Double.parseDouble(args[2]); 
				PointGPS goal=new PointGPS(Glat,Glng,Galt);
				PointGPS start=new PointGPS(sComm.getLatitude(), sComm.getLongitude(),sComm.getAltitude());
				start.setHeading(Math.toRadians(sComm.getHeading()));
				
				// set limits for heading here or in set method
				 GPS_Navigation WaypointNav=new GPS_Navigation(start, goal);
				 WaypointNav.run();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			/*Uncomment when pod comm ability is enabled
			 * LCM_PodData pComm=new LCM_PodData(); 
			pComm.run();*/
			
			
		//} catch (IOException e) {
			// TODO Auto-generated catch block
		//	e.printStackTrace();
		//}
		
		
	 }
}

		  