
enum NavState{
	START,INTERRUPTED,ROTATING,STOPPED,ARRIVED
}

public class NavStateHandler {
	
	// Add arraylist of motor commands
	NavState state;
	 final static double LAT_TO_CM=1.106E7; 
	 final static double LNG_TO_CM=1.113E7; 

	public NavStateHandler(NavState navstate) {
		this.state=navstate; 
	}

	public NavState getState() {
		return state;
	}

	public void setState(NavState state) {
		this.state = state;
	}
	
	public void updateState(){
		
	}

}
