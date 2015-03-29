import java.io.IOException;

import lcm.lcm.*;

public class LCMData implements LCMSubscriber, Runnable {
		 LCM GPSdata_LCM,Heading_LCM;  
		 GPS_logger_t GPS_msg;
		 Heading_logger_t Heading_msg;
		
		 
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
			return GPS_msg.GPS[0];
		}
		public double getLatitude() {
			return GPS_msg.GPS[1];
		}
		public double getAltitude() {
			return GPS_msg.GPS[2];
		}
		public double getHeading() {
			return Heading_msg.heading;
		}

		public void messageReceived(LCM lcm, String channel, LCMDataInputStream ins)
		    {
		        System.out.println("Received message on channel:  " + channel);
		        try {
		            if (channel.equals("GPSData")) {  	
		            	 GPS_msg=new GPS_logger_t(ins);
		            	 System.out.println("  timestamp    = " + GPS_msg.timestamp);
			                System.out.println(" [lat,lng,alt]     = [" + GPS_msg.latitude+","+GPS_msg.longitude+","+GPS_msg.altitude+" ]");
			                System.out.println("  Enabled      = '" + GPS_msg.enabled + "'");
		            }   
		            if (channel.equals("HeadingData")) {
		            	Heading_msg=new Heading_logger_t(ins);
		                System.out.println("  timestamp    = " + Heading_msg.timestamp);
		                System.out.println(" Concatenated Filename     = " + (Heading_msg.fileDir+Heading_msg.filename));
		                System.out.println("  Enabled      = '" + Heading_msg.enabled + "'");
		            }
		        } catch (IOException ex) {
		            System.out.println("Exception: " + ex);
		        }
		        }
		
		protected boolean isConnected(){
			//get whether comm channel is working
			return true; 
		}
		@Override
		public void run() {
			// run this class to listen to messages
		}
}
