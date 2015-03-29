import java.io.IOException;
import lcm.lcm.*; 


public class GPS_Navigation implements Runnable {
	 LCM_PodData podComm; 
	 LCMData sysComm;
	 NavStateHandler stateHandler; 
	 PointGPS current, start, dest; 
	
	 
	public GPS_Navigation(LCM_PodData pComm, LCMData sComm){
	
		this.podComm=pComm;
		this.sysComm=sComm;
		
        this.stateHandler=new NavStateHandler(NavState.START);
	}
	
	@Override
	public void run() {
		if(sysComm!=null || sysComm.isConnected())
		{
			// update these to format in LCM data types
			this.current=new PointGPS(sysComm.getLatitude(),sysComm.getLongitude(),sysComm.getAltitude()); 
			this.current.setHeading(sysComm.getHeading());
		}
		else{
			// get old value of current location from memory
		}
		if(podComm!=null || podComm.isConnected()){	
		 this.dest=new PointGPS(podComm.getLatitude(),podComm.getLongitude(),podComm.getAltitude()); 
		}
		else{
			// get old value of pod location from memory
		}
		this.start=this.current; 
		LinearSpline spline=new LinearSpline(this.start,this.dest);
		
			while(stateHandler.state!=NavState.ARRIVED ) // or gave up but this will be implemented later
			{
				// create linear Spline
			}
		
		
		
		
		
	}
	 public static void main(String[] args){
		
		try {
			LCMData sComm = new LCMData();
			LCM_PodData pComm=new LCM_PodData();
			sComm.run(); 
			pComm.run();
			 GPS_Navigation WaypointNav=new GPS_Navigation(pComm,sComm);
			 WaypointNav.run();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
	 }
}

		  