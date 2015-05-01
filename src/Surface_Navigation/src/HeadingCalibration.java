// class that calibrates the magenetometer on startup 
// from 0 to 360

public class HeadingCalibration {
 boolean isCalibrated=false;
 boolean isNorthSet=false;
final  static int cnum=100;
 int checkpoint =5;
final static double _tolerance=1;
 double[] mag_north;
 LCMData sComm;
 MovingFilter filter;
	public HeadingCalibration(MovingFilter mf,LCMData sysComm){
	filter=mf;
	sComm=sysComm;
	}
public void startCalibrate(){
	if(!isNorthSet){
		setNorth();
		}
	calibrate();
	}
public boolean getCalibrated(){
	return isCalibrated;
	}
public boolean setNorth(){
	double[] sum={0.0,0.0};
	int counter=0;
	double[] pos=null;
	while(counter<cnum){
	synchronized(sComm){
	pos=filter.getAverage(sComm.getIMU_pos());
			}
	if(pos==null){
	sum[0]+=pos[0];
	sum[1]+=pos[1];
	counter++;
		}
	}
	mag_north[0]=sum[0]/ (double) cnum;
	mag_north[1]=sum[1]/ (double) cnum;
	isNorthSet=true;
	System.out.println("North Set!");
	return isNorthSet;
	}
public void calibrate(){
	System.out.println("Please move slowly in a clockwise direction");
	double x=0.0;
	double y=0.0;
	double[] pos=null;
	while(checkpoint>0){
		synchronized(sComm){
		pos=filter.getAverage(sComm.getIMU_pos());}
		// checkpoint 1 - 0 degrees
		double heading = 180*calcHeading(pos[0],pos[1])/Math.PI;
		heading=wrapAngle(heading);
		if(heading<=_tolerance){
		checkpoint--;
			}
		else if(Math.abs(heading-90)<=_tolerance){
			checkpoint--;
			}
		else if(Math.abs(heading-180)<=_tolerance){
			checkpoint--;
			}
		else if(Math.abs(heading-270)<=_tolerance){
			checkpoint--;
			}
		else if(Math.abs(heading-359)<=_tolerance){
			checkpoint--;
			}
		}
		isCalibrated=true;
	System.out.println("Calibrated!");
	}
public double calcHeading(double x, double y){
	double top=(x*mag_north[0]) + (y*mag_north[1]);
	double magXY= Math.sqrt(x*x + y*y);
	double magN= Math.sqrt(mag_north[0]*mag_north[0] + mag_north[1]*mag_north[1]);
	double mag=magXY*magN;
	if(mag==0.0){
	mag=_tolerance;
	}
	return Math.acos(top/(mag));
	}
public double wrapAngle(double theta){
	return theta%(2.0*Math.PI);
	}
}
