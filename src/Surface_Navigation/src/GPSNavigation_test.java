import static org.junit.Assert.assertTrue;

import org.junit.Test;
import org.junit.runner.JUnitCore;

public class GPSNavigation_test {

	@Test
	public void constructor() {
    PointGPS s=new PointGPS(60.0,0.0,0.0);
    PointGPS d=new PointGPS(60.00143,0.0,1.0);
	GPS_Navigation nav = new GPS_Navigation(s,d);
	assertTrue(nav.getStart().equals(s));
	assertTrue(nav.getDest().equals(d));
    }
	@Test
	// normal case
	public void VerifyCommandLineParameters() {
	   GPS_Navigation.main(new String[]{"60.0", "0.0","0.0","60.10","0.0","0.0","20.0"});
	   
	    }

	
	
	public static void main(String[] args){
	    JUnitCore.main(GPSNavigation_test.class.getName());
	  }

}
