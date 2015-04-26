import java.io.IOException;

import lcm.lcm.*;
import camera_log.*;

public class LCMData implements LCMSubscriber,HeadingCalibration, Runnable {
		 LCM GPSdata_LCM,Heading_LCM;  
		 navio_gps_t GPS_msg;
		 navio_imu_t Heading_msg;
		 double latitude; 
		 double longitude; 
		 double altitude;
		 double heading;
		 boolean connected; 
		 
		public LCMData() throws IOException {
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
		public double getAltitude() {
			return altitude;
		}
		public double getHeading() {
			return heading;
		}
	

		public synchronized void messageReceived(LCM lcm, String channel, LCMDataInputStream ins)
		    {
		        System.out.println("Received message on channel:  " + channel);
		        this.connected=true;
		        try {
		            if (channel.equals("gps")) {  	
		            	 GPS_msg=new navio_gps_t(ins);
		            	 System.out.println("  timestamp    = " + GPS_msg.timestamp+ "Got GPS data");
			             this.latitude=GPS_msg.lat; 
			             this.longitude=GPS_msg.lon; 
			             this.altitude=GPS_msg.alt;
		            }   
		            if (channel.equals("imu")) {
		            	Heading_msg=new navio_imu_t(ins);
		                System.out.println("  timestamp    = " + Heading_msg.timestamp);
		               this.heading=calculateHeading();
		            }
		        } catch (IOException ex) {
		            System.out.println("Exception: " + ex);
		        }
		        }
		
		boolean isConnected(){
			// checks task manager status
			
		}
		@Override
		public void run() {
			System.out.println("Recieving System Data");
			 for(;;)
			 {
				 // check if receiving data from pi
			 }
		}

		@Override
		public double calculateHeading() {
			double[] imu_pos=Heading_msg.imu_pos; 
			double[] imu_vel=Heading_msg.imu_vel; 
			double[] imu_acc=Heading_msg.imu_acc;
			double mag_norm=Math.sqrt((imu_pos[0]*imu_pos[0])+(imu_pos[1]*imu_pos[1]+(imu_pos[2]*imu_pos[2])));
			double magx=imu_pos[0]/mag_norm; 
			double magy=imu_pos[1]/mag_norm;
			double magz=imu_pos[2]/mag_norm;
			double Pitch=Math.atan2(imu_acc[1],(Math.sqrt(imu_acc[0]*imu_acc[0] + imu_acc[2]*imu_acc[2])));
			double Roll=Math.atan2(-imu_acc[0],imu_acc[2]);
			double delta_my= -magy*Math.cos(Roll)+magz*Math.sin(Roll); 
			double delta_mx=magx*Math.cos(Pitch)+magy*Math.sin(Pitch)*Math.sin(Roll)+magz*Math.sin(Pitch)*Math.cos(Roll);
			double magheading=Math.atan2(delta_my,delta_mx);
			return magheading;
		}
}
