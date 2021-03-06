
import java.io.*;
import lcm.lcm.*;
import camera_log.*;
public class CameraSubscriber implements LCMSubscriber {
	LCM lcm; 
	public CameraSubscriber() throws IOException {
		
		        this.lcm = new LCM();
		        this.lcm.subscribe("CameraData", this);
		    }
		    public void messageReceived(LCM lcm, String channel, LCMDataInputStream ins)
		    {
		        try {
		            if (channel.equals("CameraData")) {
		            	camera_logger_t msg=new camera_logger_t(ins);
		                System.out.println("  timestamp    = " + msg.timestamp);
		                System.out.println(" Concatenated Filename     = " + (msg.fileDir+msg.filename));
		                System.out.println("  Enabled      = '" + msg.enabled + "'");
		            }
		        } catch (IOException ex) {
		            System.out.println("Exception: " + ex);
		        }
		    }
		    public static void main(String args[])
		    {
		        try {
		            CameraSubscriber cSub = new CameraSubscriber();
		            while(true) {
		            }
		        } catch (IOException ex) {
		            System.out.println("Exception: " + ex);
		        } //catch (InterruptedException ex) { }*/
		    }
		}
