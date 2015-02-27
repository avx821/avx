import java.io.* ;
import lcm.lcm.*; 
import camera_log.*;
public class SendMessage {

	public static void main(String args[])
    {
        
            LCM lcm = LCM.getSingleton();
            camera_logger_t msg = new camera_logger_t();
            // make this a static function that is called from camera
            msg.timestamp = System.nanoTime();
            msg.fileDir="";
            msg.filename = "testsample_" + String.valueOf(msg.timestamp);
            msg.enabled = true;
            lcm.publish("CameraData", msg);
        
    }
}
