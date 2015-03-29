
public class waypointVector {
	double delta_x; 
	double delta_y; 
	double delta_z; 
	public waypointVector(double dx,double dy,double dz) {
		delta_x=dx;
		delta_y=dy;
		delta_z=dz;
	}
	
	public double getDelta_x() {
		return delta_x;
	}
	public void setDelta_x(double delta_x) {
		this.delta_x = delta_x;
	}
	public double getDelta_y() {
		return delta_y;
	}
	public void setDelta_y(double delta_y) {
		this.delta_y = delta_y;
	}
	public double getDelta_z() {
		return delta_z;
	}
	public void setDelta_z(double delta_z) {
		this.delta_z = delta_z;
	}	
	
}
