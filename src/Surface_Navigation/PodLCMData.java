
import java.io.IOException; 
import avionics.*; 
import lcm.lcm.*;

public class PodLCMData implements LCMSubscriber,Runnable {
		 LCM GPSdata_LCM,Heading_LCM;  
		 //TODO change message types
		 navio_gps_t GPS_msg;
		 navio_imu_t Heading_msg;
		 double latitude; 
		 double longitude; 
		 double altitude;
		 double heading=Double.NaN;
		 boolean connected=false;
		 GPS_Navigation gpsnav;
		final double _tolerance=1E-3;

		public PodLCMData() throws IOException {
			this.GPSdata_LCM = new LCM();
			this.Heading_LCM = new LCM();
	        this.GPSdata_LCM.subscribe("gps", this);
	        this.Heading_LCM.subscribe("imu", this);
		this.connected=false;
	      /*
	       * Do here-write more code
	       *  // initialize heading and GPS message from last mode
	       */
		}
		public double getLongitude() {
			return longitude;
		}
		public double getLatitude() {
			return latitude;
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
		    //TODO: changegd message channel names
		        try {
		            if (channel.contains("gps")) {
		            	 GPS_msg=new navio_gps_t(ins);
		        //    	 System.out.println("  timestamp    = " + GPS_msg.timestamp+ "Got GPS data");
			             this.latitude=GPS_msg.lat; 
			             this.longitude=GPS_msg.lon; 
		            	}
		            if (channel.contains("imu")) {
		            	Heading_msg=new navio_imu_t(ins);
		          //      System.out.println("  timestamp    = " + Heading_msg.timestamp);
		              this.heading=calculateHeading();
					System.out.println(" heading    = " +this.heading*180/Math.PI);
						if(this.heading!=Double.NaN){
						this.connected=true;
						}
		        	}
				}catch (Exception ex) {
		            System.out.println("Exception: " + ex);
		        }
			synchronized(this){
			this.notifyAll();   
			}
		}
		boolean isConnected(){
			return connected;
		}
		public double calculateHeading(navio_imu_t) {
			double magheading= Heading_msg.heading-Math.PI;
			return magheading;
		}
		@Override
	public void run() {
	
	System.out.println("waiting for pod comm. startup");
	synchronized(gpsnav){
		while(!connected){
			try{
			gpsnav.wait();
			}catch(InterruptedException ex){
			System.out.println("Monitor exception");
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
