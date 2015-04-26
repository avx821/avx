
enum NavState{
	READY,START,INTERRUPTED,TRACKING,STOPPED,ARRIVED
}

public class NavStateHandler {
	
	MotorCommand command;
	NavState state;
	

	public NavStateHandler(NavState navstate) {
		this.state=navstate; 
	}

	public NavState getState() {
		return state;
	}

	public void setState(NavState state) {
		this.state = state;
	}
	// handles state updates
	public void updateState(PointGPS target){
		if(target==null && state==NavState.ARRIVED)
		{
			System.out.println("Arrived at destinationi GPS point");
		}
		else if (state==NavState.START && target!=null){
			state=NavState.TRACKING;
		}
		else if (state==NavState.TRACKING){
			// get distance and heading from target
			// send rotational and translational velocity commands
		}
	}

}
