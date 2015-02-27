
import camera_log.*; 
import java.io.* ;
import lcm.lcm.*; 
public class SendCameraData {

	public static boolean SendData()
	{
        try {
            LCM lcm = LCM.getSingleton();
            cam_logger_t msg = new cam_logger_t();
            // make this a static function that is called from camera
            msg.timestamp = System.nanoTime();
            msg.fileDir="~/avionics/src/lcm_types/camera_log/images/";
            msg.filename = "testsample_" + String.valueOf(msg.timestamp);
            msg.enabled = true;
            lcm.publish("CameraData", msg);
        } catch (IOException ex) {
            System.out.println("Exception: " + ex);
        }
        return true;
	}
}
