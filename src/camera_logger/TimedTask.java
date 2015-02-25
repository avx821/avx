

import java.util.Timer;
import java.util.TimerTask;

public class TimedTask extends TimerTask {
	Timer timer; 
	final int starttime=0; 
	final int delay=500; // in milliseconds
	public TimedTask() {
		timer=new Timer("data Log timer");
		timer.schedule(this,starttime,delay);
}

	@Override
	public void run() {
		System.out.println("Snapshot taken: ");
	}
	
	public static void main(String[] args){
		TimedTask t=new TimedTask();
		t.run();
	}
	}
