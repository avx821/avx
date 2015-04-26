import javafx.scene.shape.Circle;


public class Aircraft {
	double[] position; 
	double heading; 
	PointGPS currentpos;
	
	
	public Aircraft(double lat, double lng, double radius) {
		this.position[0]=lat;
		this.position[1]=lng;
		this.position[2]=radius;
		this.currentpos=new PointGPS(position[0],position[1],0.0);
	}
	
	boolean updatePosition(double lat, double lng){
		this.position[0]=lat; 
		this.position[1]=lng; 
		currentpos.setLat(lat);
		currentpos.setLng(lng);
		return true;
	}

	double getRadius() {
		return position[2];
	}
	void setHeading(double heading){
		this.heading=heading;
	}
	boolean intersects(double lat, double lng){
		// checks to see if points intersects with aircraft area
		return false; 
	}
	PointGPS getCurrentPos(){
		return this.currentpos;
	}
	
}
