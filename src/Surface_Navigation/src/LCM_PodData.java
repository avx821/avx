import java.io.IOException;
import lcm.lcm.LCM;
import lcm.lcm.LCMDataInputStream;
import lcm.lcm.LCMSubscriber;

public class LCM_PodData implements LCMSubscriber, Runnable {
		 LCM GPSdata_LCM,Heading_LCM,windData_LCM;  

		public LCM_PodData() throws IOException {
			this.GPSdata_LCM = new LCM();
			this.Heading_LCM = new LCM();
			this.windData_LCM = new LCM();
	        this.GPSdata_LCM.subscribe("GPSData_pod", this);
	        this.Heading_LCM.subscribe("HeadingData_pod", this);
	        this.windData_LCM.subscribe("windData_pod", this);
		}
		
		 public void messageReceived(LCM lcm, String channel, LCMDataInputStream ins)
		    {
		        System.out.println("Received message on channel:  " + channel);
		        try {
		            if (channel.equals("GPSData_pod")) {
		            	logger_t GPS_msg=new logger_t(ins);
		                System.out.println("  timestamp    = " + GPS_msg.timestamp);
		                System.out.println(" [lat,lng,alt]     = [" + GPS_msg.latitude+","+GPS_msg.longitude+","+GPS_msg.altitude+" ]");
		                System.out.println("  Enabled      = '" + GPS_msg.enabled + "'");
		            }
		            if (channel.equals("windData_pod")) {
		            	logger_t wind_msg=new logger_t(ins);
		                System.out.println("  timestamp    = " + wind_msg.timestamp);
		                System.out.println(" Concatenated Filename     = " + (wind_msg.fileDir+wind_msg.filename));
		                System.out.println("  Enabled      = '" + wind_msg.enabled + "'");
		            }
		            if (channel.equals("HeadingData_pod")) {
		            	logger_t PodHeading_msg=new logger_t(ins);
		                System.out.println("  timestamp    = " + PodHeading_msg.timestamp);
		                System.out.println(" Concatenated Filename     = " + (PodHeading_msg.fileDir+PodHeading_msg.filename));
		                System.out.println("  Enabled      = '" + PodHeading_msg.enabled + "'");
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
