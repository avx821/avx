import java.util.Timer;
import java.util.TimerTask;
import java.io.IOException;  
import java.util.logging.Level;  
import java.util.logging.Logger; 
		
public class TakePicture  extends TimerTask {
	Timer timer; 
	final int starttime=0;
	boolean isTaken;
	final int delay=1000;// in milliseconds
	public TakePicture() {
				this.isTaken=false; 
				timer=new Timer("data Log timer");
				timer.schedule(this,starttime,delay);
		}
	private void executeShellCommand(String pCommand)  
	   {  
	     try {  
	       Runtime run = Runtime.getRuntime() ;  
	       Process pr = run.exec(pCommand) ;  
	       pr.waitFor() ;  
	     } catch (IOException | InterruptedException ex) {  
	       Logger.getLogger(TakePicture.class.getName()).log(Level.SEVERE, null, ex);  
	     } } 
			@Override
			public void run() {
				isTaken=true;
				//take picture here
				long timestamp = System.nanoTime();
	           String fileDir="~/avionics/src/lcm_types/camera_log/images/";
	            String filename = "testsample_" + String.valueOf(timestamp);
	            filename=fileDir+filename;
				executeShellCommand("raspistill -o "+filename+".jpg");  
				//send message
				SendCameraData.SendData();
				isTaken=false;
			}
			
			public static void main(String[] args){
				TakePicture t=new TakePicture();
				t.run();
// TODO Auto-generated constructor stub
	}

}
