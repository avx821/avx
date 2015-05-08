import java.io.IOException; 
import avionics.*;
 import lcm.lcm.*;

public class LCMData implements LCMSubscriber,Runnable {
		 LCM GPSdata_LCM,Heading_LCM;
		 navio_gps_t GPS_msg;
		 navio_imu_t Heading_msg;
		 double latitude; 
		 double longitude; 
		 double altitude;
		 double heading=Double.NaN;
		 boolean connected=false;
		 MovingFilter filterx,filtery;
		 GPS_Navigation gpsnav;
		final double _tolerance=1E-3;
		public LCMData() throws IOException {
			this.GPSdata_LCM = new LCM();
			this.Heading_LCM = new LCM();
	        this.GPSdata_LCM.subscribe("gps", this);
	        this.Heading_LCM.subscribe("imu", this);
	        this.filterx=new MovingFilter();
		this.filtery=new MovingFilter();
		this.connected=false;
	      /*
	       * Do here-write more code
	       *  // initialize heading and GPS message from last mode
	       */
		}
		public void setNav(GPS_Navigation nav){
			this.gpsnav=nav;
		}
		public double getLongitude() {
			return longitude;
		}
		public double getLatitude() {
			return latitude;
		}
		public double getAltitude() {
			return altitude;
		}
		public double getHeading() {
			return heading;
		}
		public double[] getIMU_pos(){
		double x=(double) (Heading_msg.imu_pos[0]/1.0);
		double y=(double) (Heading_msg.imu_pos[1]/1.0);
		double[] pos={x,y};
		return pos;
		}

		public synchronized void messageReceived(LCM lcm, String channel, LCMDataInputStream ins){
		    //  System.out.println("Received message on channel:  " + channel); 
		        try {
		            if (channel.contains("gps")) {
		            	 GPS_msg=new navio_gps_t(ins);
		        //    	 System.out.println("  timestamp    = " + GPS_msg.timestamp+ "Got GPS data");
			             this.latitude=GPS_msg.lat; 
			             this.longitude=GPS_msg.lon; 
			             this.altitude=GPS_msg.alt;
		       		}
			}catch(IOException ex){}
			try{
		            if (channel.contains("imu")) {
		            	Heading_msg=new navio_imu_t(ins);
		          //      System.out.println("  timestamp    = " + Heading_msg.timestamp);
		             this.heading=calculateHeading();
				//	System.out.println(" heading    = " +this.heading*180/Math.PI);
				//	Thread.sleep(50);
					if(this.heading!=Double.NaN){
						this.connected=true;
						}
			}	}catch (Exception ex) {
		            System.out.println("Here-->Exception: " + ex);
		        }
			synchronized(this){
			this.notifyAll();
			}
		}
		boolean isConnected(){
			return connected;
		}
		public double calculateHeading() {
			double magx=(double)(Heading_msg.imu_pos[0]/1000.0);
			double magy=(double)(Heading_msg.imu_pos[1]/1000.0);
			//double magnorm=Math.sqrt(magx*magx + magy*magy);
			magx=(magx/22.0)+0.7;
			magy=(magy/20)-5.8;
			magx=this.filterx.getAverage(magx);
			magy=this.filtery.getAverage(magy);
			double magheading= Math.atan2(-magy,magx);
			//System.out.println("[magx,magy]:["+(Heading_msg.imu_pos[0]/1000.0)+", "+(Heading_msg.imu_pos[1]/1000.0)+"]");
			//System.out.println("heading: "+magheading);*/
			return magheading;
		}
		@Override
	public void run() {
	//public static void main(String[] args){
	System.out.println("waiting for comm. startup");
	synchronized(gpsnav){
		while(!connected){
			try{
			gpsnav.wait();
			}catch(InterruptedException ex){
			System.out.println("Monitor exxception");
			}
		}
		System.out.println("Stop waiting");
	}
			 for(;;)
			 {
				 // check if receiving data from pi
			 }
		}
}


