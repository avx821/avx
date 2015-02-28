import java.util.Timer;
import java.util.TimerTask;
import java.lang.IllegalArgumentException;  
import java.util.logging.Level;  
import org.opencv.core.Mat; 
import org.opencv.Highgui.Highgui;
import java.util.logging.Logger; 
import java.util.*;		
public class TakePicture  extends TimerTask {
	Timer timer; 
	final int starttime=0;
	boolean isTaken;
	final int delay=100;// in milliseconds
	public TakePicture() {
				this.isTaken=false; 
				timer=new Timer("data Log timer");
				timer.schedule(this,starttime,delay);
		}
	/*private void executeShellCommand(String pCommand)  
	   {
		Process pr=null;
	     try {
	       pr =Runtime.getRuntime().exec(pCommand);  
		 {
	      	System.out.println("error");
	     	}
try{			      pr.waitFor();  

}catch(InterruptedException e)
{
System.out.println("Waiting Exception");
}
	   }*/

public void grabImage(String filename){
System.loadLibrary("opencv-2.4.1"); 
VideoCapture camera=new VideoCapture(0);
try{
Thread.sleep(1000);
}
catch(Exception e){}camera.open(0); 
if(!camera.isOpened()){
	throw new IllegalArgumentException("Camera failed to open.");
}
Mat frame=new Mat(); 
camera.read(frame); 
Highgui.imwrite(filename,frame); 
				SendCameraData.SendData();

return; 

}
	@Override
	public void run() {
		isTaken=true;
		//take picture here
		long timestamp = System.nanoTime();
	        String fileDir="/home/pi/avionics/src/test_codes/camera_log/images/";
	        String filename = "testsample_" + String.valueOf(timestamp);
	        filename=fileDir+filename;
		//this.executeShellCommand("raspistill -o "+filename+".jpg");  
	this.grabImage(filename);
		//send message
//	SendMessage.Msgmain();	
	isTaken=false;
		}
			
			public static void main(String[] argv){
				TakePicture t=new TakePicture();
				t.run();
				}
				}
