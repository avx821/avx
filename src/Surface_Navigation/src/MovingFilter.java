public class MovingFilter{
	double[] sum;
	final int fnum=10;
	double[] data={0.0,0.0,0.0,0.0,0.0,
			0.0,0.0,0.0,0.0,0.0,
			};
	int counter=0;

	public MovingFilter(){
	}
	public double getAverage(double value){
	if(value!=Double.NaN){
		this.data[counter%fnum]=value;
		System.out.println("data["+(counter%fnum)+" ]: "+value);
		if(counter<fnum){
		counter++;
		//System.out.println("returning NaN");
		return Double.NaN;
			}
		else{
		//System.out.println("Counter>5; please return a value");
		double sum=0.0;
		double average=0.0;
		for(int i=0; i<fnum; i++){
		sum+=this.data[i];
		}
		average=sum/(double)fnum;
		counter++;
		return average;
			}
		}
	return Double.NaN;
	}
}
