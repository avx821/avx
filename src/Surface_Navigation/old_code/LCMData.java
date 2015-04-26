import java.io.IOException;

import lcm.lcm.*;
import camera_log.*;

public class LCMData implements LCMSubscriber, Runnable {
		 LCM GPSdata_LCM,Heading_LCM;  
		 GPS_logger_t GPS_msg;
		 Heading_logger_t Heading_msg;
		 double latitude; 
		 double longitude; 
		 double altitude;
		 double heading;
		
		 
		public LCMData() throws IOException {
			this.GPSdata_LCM = new LCM();
			this.Heading_LCM = new LCM();
	        this.GPSdata_LCM.subscribe("GPSData", this);
	        this.Heading_LCM.subscribe("HeadingData", this);
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
		public double getAltitude() {
			return altitude;
		}
		public double getHeading() {
			return heading;
		}

		public synchronized void messageReceived(LCM lcm, String channel, LCMDataInputStream ins)
		    {
		        System.out.println("Received message on channel:  " + channel);
		        try {
		            if (channel.equals("GPSData")) {  	
		            	 GPS_msg=new GPS_logger_t(ins);
		            	 System.out.println("  timestamp    = " + GPS_msg.timestamp+ "Got GPS data");
			             this.latitude=GPS_msg.gps[0]; 
			             this.longitude=GPS_msg.gps[1]; 
			             this.altitude=GPS_msg.gps[2];
		            }   
		            if (channel.equals("HeadingData")) {
		            	Heading_msg=new Heading_logger_t(ins);
		                System.out.println("  timestamp    = " + Heading_msg.timestamp);
		               this.heading=Heading_msg.heading;
		            }
		        } catch (IOException ex) {
		            System.out.println("Exception: " + ex);
		        }
		        }
		
		boolean IsConnected(){
			// checks to see if messages are coming in
			// if no messages in specified time period, then assume connection lost
		}
		@Override
		public void run() {
			// run this class to listen to messages
		}
}
