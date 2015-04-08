import java.util.ArrayList;
enum SegmentType{
	LINEAR,CUBIC}

public class LinearSpline {
	double distance; // distance between start and destination waypoints in m
	ArrayList<PointGPS> waypoints; // list of waypoints between start and destination
	final static double GPS_ACCURACY=1; // degree of accuracy of GPS in m
	final static double LAT_TO_METERS=1.106E5;
	final static double LNG_TO_METERS=1.113E5;
	final static double VISUAL_RADIUS=10;
	final static double SPLINE_LENGTH=0.1; // 10 cm
	final static double _tolerance=1E-6; 
	SegmentType seg;
	double xt,xt_sq;
	waypointVector spline_c,f1_prime, spline_b,spline_a,f0,f1, delta_vector,sd;
	PointGPS start,dest, target;
	public LinearSpline(PointGPS start, PointGPS dest) {
		
		if(start==null || dest==null){
			throw new NullPointerException("Invalid GPS location(s)");
			}
		
		this.start=start; 
		this.dest=dest;
	 sd=PointGPStoVector(this.start, this.dest);
		this.distance=Math.sqrt(sd.getDelta_x()*sd.getDelta_x()+sd.getDelta_y()*sd.getDelta_y());
		if(distance<=GPS_ACCURACY){
				throw new IllegalArgumentException("You are already within GPS range...switch to visual servo mode or to next pod");
			}
		
		}
	double calculateDistance(waypointVector vec){
		return Math.sqrt(vec.getDelta_x()*vec.getDelta_x()+vec.getDelta_y()*vec.getDelta_y());
	}
	
	double calculateHeight(){
		return (this.dest.getAlt()-this.start.getAlt()); 
	}
	// function that provides Angle wrap capability
	double wrapAngle(double angle){
		return angle%(2.0*Math.PI);
		
	}
	// function that creates spline from waypoints and pod heading
	void createSpline(PointGPS w0,PointGPS w1){
		// should I return points in spline or velocity to command?
		// for now, I will return points
		double heading=this.start.getHeading();
		// get the gradient of the line
		waypointVector spline=PointGPStoVector(w1,w0);
		double gradient=spline.delta_y/spline.delta_x;
		double num_steps=20.0; 
		// number of steps to create spline
		 double t=this.calculateDistance(spline)/num_steps;
		 for(int n=1; n<21;n++){
			  xt=(double)n*t;
			 System.out.println("xt:"+xt+", gradient:"+gradient*xt);
			 double x=this.start.getLat()+xt*gradient/LNG_TO_METERS;
			 double y=this.start.getLng()+(xt/LAT_TO_METERS);
			 System.out.println("x-lat:"+x+", y-long:"+y);
			 PointGPS nextpoint=new PointGPS(x,y,0.0);
			this.waypoints.add(nextpoint);
		 }
	}
	
	waypointVector AddVectors(ArrayList<waypointVector> vectors){
		waypointVector SumVector=new waypointVector(0,0,0); 
		for(waypointVector w:vectors)
		{
		SumVector.delta_x+=w.delta_x; 
		SumVector.delta_y+=w.delta_y;
		SumVector.delta_z+=w.delta_z;
		}
	return SumVector; 
		
	}
	void initializeSpline(PointGPS w0,PointGPS w2){
		waypointVector vec=PointGPStoVector(w0,w2);
		f1_prime=new waypointVector(vec.delta_x/SPLINE_LENGTH,vec.delta_y/SPLINE_LENGTH,0);
		ArrayList<waypointVector> vectors=new ArrayList<waypointVector>(); 
		vectors.add(spline_c); 
		vectors.add(f0.scaleVector(2.0)); 
		vectors.add(f1.scaleVector(-2.0)); 
		vectors.add(f1_prime);
		spline_a=AddVectors(vectors);
		vectors.clear();
		// re-use vector to add other vectors
		vectors.add(spline_c); 
		vectors.add(f0.scaleVector(-3.0)); 
		vectors.add(f1.scaleVector(3.0)); 
		vectors.add(f1_prime.scaleVector(-1.0));
		spline_b=AddVectors(vectors);
		}
		
	ArrayList<PointGPS> createWaypoints(){
		this.waypoints=new ArrayList<PointGPS>();
		PointGPS w0=this.start;
		this.waypoints.add(w0);
		PointGPS w1=null;
		System.out.println("Start: ["+w0.getLat()+","+w0.getLng()+"]");
		if(Math.abs(this.distance-100)>=_tolerance){
			// set the next waypoint 50m away and gun it to that waypoint
			System.out.println("does this work?");
			w0.setFlag(true,true,WaypointType.STRAIGHT);
			w1=calcWaypointfromPoint(50,this.dest,this.getBearing(this.sd)); // sets the next waypoint 50m away from dest
			System.out.println("Destination: ["+w1.getLat()+","+w1.getLng()+"]");
		}
		else{
			w0.setFlag(true,true,WaypointType.STRAIGHT);
			w1=this.dest;
		}
		
		
		createSpline(w0,w1);
		return this.waypoints;
	}
	double getBearing(waypointVector u){
		return Math.atan2(u.delta_y,u.delta_x);
	}
	
	PointGPS vectortoPointGPS(waypointVector v, PointGPS from){
		double delta_phi=v.getDelta_y()/LAT_TO_METERS;
		double delta_lambda=v.getDelta_x()/LNG_TO_METERS;
		double to_x=from.getLat()+delta_phi; 
		double to_y=from.getLng()+delta_lambda; 
		return new PointGPS(to_x,to_y,from.getAlt());
	}
	waypointVector PointGPStoVector(PointGPS from, PointGPS to){
		double delta_x=(to.getLat()-from.getLat())*LAT_TO_METERS; 
		double delta_y=(to.getLng()-from.getLng())*LNG_TO_METERS;
		double delta_z=to.getAlt()-from.getAlt();
		return new waypointVector(delta_x,delta_y,delta_z);
	}
	PointGPS calcWaypointfromPoint(double dist, PointGPS fp, double bearing){
		double delta_x=dist*Math.sin(bearing); 
		double delta_y=dist*Math.cos(bearing); 	
		double delta_phi=delta_y/LAT_TO_METERS; 
		double delta_lambda=delta_x/LNG_TO_METERS; 
		double new_x=fp.getLat()-delta_phi; 
		double new_y=fp.getLng()-delta_lambda; 
		return new PointGPS(new_x,new_y,0.0); 
		
	}
	void evaluate_spline(){
		ArrayList<waypointVector> vectors=new ArrayList<waypointVector>(); 
		if(this.seg==SegmentType.CUBIC){
		double xt_cubed=xt_sq*xt;
		vectors.add(spline_a.scaleVector(xt_cubed)); 
		vectors.add(spline_b.scaleVector(xt_sq)); 
		vectors.add(spline_c.scaleVector(xt)); 
		//vectors.add(this.f0);
		}
		else if(this.seg==SegmentType.LINEAR){
			vectors.add(spline_c.scaleVector(xt)); 
			///vectors.add(this.f0);
			}
		
		delta_vector=AddVectors(vectors);
	//System.out.println(delta_vector.getDelta_x()+", "+delta_vector.getDelta_y()+",");
		this.target=this.vectortoPointGPS(delta_vector, this.target);
//	System.out.println(target.getLat()+", "+target.getLng()+",");
	vectors.clear();
	}
	

}


