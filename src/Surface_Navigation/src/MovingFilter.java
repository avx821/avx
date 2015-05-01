public class MovingFilter{
	double[] sum;
	final int fnum=5;
	double[] data={0.0,0.0,0.0,0.0,0.0};
	int counter=0;

	public MovingFilter(){
	}
	public double getAverage(double value){
	if(value.length==null){
	throw new NullPointerException("Null value");
		}
	else if(value=Double.NaN){
	return 0.0;
	}
	else{
		this.data[counter%fnum]=value;
		if(counter<fnum){
		return Double.NaN;
			}
		else{
		double sum=0.0;
		double average=0.0;
		for(int i=0; i<fnum; i++){
		sum+=this.data[i];
				}
		average=sum/(double)fnum;
		return average;
			}
		}
	return Double.NaN;
	}
}
