import java.lang.IllegalArgumentException;  
import java.io.*;
import java.util.logging.Level;  
import org.opencv.core.Mat; 
import org.opencv.core.Core;
import org.opencv.imgcodecs.Imgcodecs;
import java.util.logging.Logger; 
import org.opencv.videoio.VideoCapture;
import java.util.*;
import lcm.lcm.*;
import camera_log.*;		
public class TakePicture_new {
	
	int counter=0;
	long time; 
	LCM lcm;
	VideoCapture camera;	
	camera_logger_t msg;
public TakePicture_new() {
	 System.loadLibrary(Core.NATIVE_LIBRARY_NAME);
		lcm=LCM.getSingleton();
		msg=new camera_logger_t(); 
		camera=new VideoCapture(0);
		camera.open(0);
	/*	try{
		Thread.sleep(1000);}
		catch (Exception e)
		{
		System.out.println("Sleep Exception");
		}*/	
		if(!camera.isOpened()){
		throw new NullPointerException("Camera is not opened");
		}
		time=System.currentTimeMillis();	
		}

	public void grabImage(String filename){

	Mat frame=new Mat(); 
	if(camera.read(frame))
	{
	counter++;
	Imgcodecs.imwrite(filename,frame);
	return; 
	} 
	else System.out.println("no image");
	}

	public void run() {
		//take picture here
		for(;;){
		msg.timestamp = System.nanoTime();
	        msg.fileDir="/home/pi/avionics/images/";
	        msg.filename = "testsample_" + String.valueOf(msg.timestamp);
		msg.enabled=true;	
	grabImage(msg.fileDir+msg.filename+".jpg");
		lcm.publish("CameraData",msg);	
			}		
	//	else { 
	//	System.out.println(System.currentTimeMillis()-time*10e-4);
//		System.out.println("counter:"+counter);
//		camera.release(); 
	//		}
	}
	public static void main(String [] args){
		TakePicture_new t=new TakePicture_new();
		t.run();
				}
}
