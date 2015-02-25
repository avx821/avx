import java.io.* ;
import lcm.lcm.*; 
public class SendMessage {

	public static void main(String args[])
    {
        try {
            LCM lcm = LCM.getSingleton();
            camera_log.cam_logger_t msg = new camera_log.cam_logger_t();
            // make this a static function that is called from camera
            msg.timestamp = System.nanoTime();
            msg.filename="~/avionics/src/lcm_types/camera_log/images/";
            msg.filename = "testsample_" + String.valueOf(msg.timestamp);
            msg.enabled = true;
            lcm.publish("CameraData", msg);
        } catch (IOException ex) {
            System.out.println("Exception: " + ex);
        }
    }
}