
enum WaypointType{
	SPLINE,STRAIGHT
}
//nested class that defines other PointGPS types
class waypointflag{
	boolean reached_destination;  // true if we have reached waypoint
	boolean fast_waypoint;  // true if waypoint radius need not be considered
	WaypointType type;  // whether waypoint is straight or on the spline

	void setType(WaypointType pointType){
	this.type=pointType;
	}
	WaypointType getType(){
		return this.type;
	}
	public boolean isReached_destination() {
		return reached_destination;
	}
	public void setReached_destination(boolean reached_destination) {
		this.reached_destination = reached_destination;
	}
	public boolean isFast_waypoint() {
		return fast_waypoint;
	}
	public void setFast_waypoint(boolean fast_waypoint) {
		this.fast_waypoint = fast_waypoint;
	}
	
	
}

public class PointGPS {
	
	/*
	 * @param lat- GPS latitude co-ordinate in cm 
	 * @param lng- GPS longitude co-ordinate in cm 
	 * @param alt- GPS ellipsoid height in cm 
	 * 
	 */
	double lat; 
	double lng; 
	double alt; 
	double heading;
	waypointflag flag;
	
	
	public PointGPS(double lat, double lng, double alt) {
		this.lat=lat;
		this.lng=lng; 
		this.alt=alt;
		this.flag= new waypointflag();
		
	}

	public double getHeading() {
		return heading;
	}

	public void setHeading(double heading) {
		this.heading = heading;
	}

	public double getLat() {
		return lat;
	}

	public void setLat(double lat) {
		this.lat = lat;
	}

	public double getLng() {
		return lng;
	}

	public void setLng(double lng) {
		this.lng = lng;
	}

	public double getAlt() {
		return alt;
	}

	public void setAlt(double alt) {
		this.alt = alt;
	}

	public void setFlag(boolean reached, boolean fast, WaypointType t) {
		
		flag.reached_destination=reached; 
		flag.fast_waypoint=fast; 
		flag.type=t; 
		System.out.println("Waypoint flag set");
	}
	
	
	
}

