
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
	
	
	public PointGPS(double lat, double lng, double alt) {
		this.lat=lat;
		this.lng=lng; 
		this.alt=alt;	
	}
	public void setNewPoint(double lat, double lng, double alt){
		this.lat=lat;
		this.lng=lng; 
		this.alt=alt;
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

	
	
	
}

