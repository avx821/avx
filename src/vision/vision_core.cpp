extern double WIDTH;
extern double HEIGHT;
//TODO choose where to put height and width
 
// 
#include "vision_core.hpp"

#include <ctime>
#include <string>
#include <fstream>

#define SHOW_THRESHOLDED false
#define SHOW_DETECTED false
#define ROI_limit 200

using namespace cv;
using namespace std;

unsigned long long get_timestamp(){
	struct timeval tv;
	gettimeofday(&tv,NULL);
	unsigned long long millisecondsSinceEpoch=
	(unsigned long long)(tv.tv_sec)*1000+
	(unsigned long long)(tv.tv_usec)/1000;
	return millisecondsSinceEpoch;
}


	void options_listener::handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan,  
                const avionics::vision_options_t* msg)
        {
        	cout<<"received msg"<<endl;
        	options->trackbar=msg->trackbar;
        	options->equalization=msg->equalization;
        	options->morphological=msg->morphological;
        	options->roi=msg->roi;
        	options->save_video=msg->save_video;
        	options->area=msg->area;
        	options->power=msg->power;
        	options->tau=msg->tau;
   
        };
        void* options_listener::listen(void* input_ptr){
        	
        	lcm::LCM* lcm_ptr=(lcm::LCM*)input_ptr;
        	cout<<"started listening"<<endl;
        	while(0 == lcm_ptr->handle());
        	return NULL;
        	cout<<"stoped listening"<<endl;
        };
        options_listener::options_listener(options_c* options_input){
	options=options_input;
    	if(!lcm.good());
        	//return 1; actually do something TODO
	lcm.subscribe("VISION_OPTIONS", &options_listener::handleMessage, this);
	
};


VideoCapture Initialize_input_video()
{
    // loads the camera model. Maybe have smarter thing in case it is not loaded
    system("sudo modprobe bcm2835-v4l2");
    VideoCapture cap(0); // open the default camera
    // gets the frame size from camera. If you change the hardware, check if it's properly defined
    HEIGHT=cap.get(CV_CAP_PROP_FRAME_HEIGHT);
    WIDTH=cap.get(CV_CAP_PROP_FRAME_WIDTH);
    return cap;

};

VideoWriter Initialize_output_video(VideoCapture inputVideo,string NAME )
{
    // Initializing video output file
    VideoWriter outputVideo;
    // TODO: get the right codec
    int ex = static_cast<int>(inputVideo.get(CV_CAP_PROP_FOURCC));
    Size S = Size((int) inputVideo.get(CV_CAP_PROP_FRAME_WIDTH),    // Acquire input size, maybe use the global variables
                  (int) inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT));

    // Open the output
    // TODO Get the right FPS
    outputVideo.open(NAME, ex=-1, 20, S, true);
    if (!outputVideo.isOpened())
    {
        std::cout  <<ex ;
        //return NULL; find a better way to return
    }
}


ROI_c::ROI_c()
{
    origin=Point2f(0,0);
    x_high=0;
    y_high=0;
    radius=0;
    use=false;
    counter=0;
}
void ROI_c::update(Point2f* center, int radius_new)
{
    radius=radius_new;

    if ( center->x==0||center->y==0||radius<=0||counter>=ROI_limit)  // no cropping found
    {
        use=false;
        origin=Point2f(0,0);
        counter=0;
    }
    else
    {
        use=true;
        counter=counter+1;
        radius=radius*1.4; // margin
        origin.x=center->x-radius;
        x_high=center->x+radius;
        origin.y=center->y-radius;
        y_high=center->y+radius;

        // Respecting the frame boundaries
        if(origin.x<=0) origin.x=0;
        if( x_high>WIDTH) x_high=WIDTH;
        if( origin.y<=0)  origin.y=0;
        if( y_high>HEIGHT)  y_high=HEIGHT;
    }
}
Mat ROI_c::crop(Mat img)
{
    return img(Rect(origin.x,origin.y, x_high-origin.x, y_high-origin.y));
}


void update_FPS(int frame_time)
{
    static int frame_count=-1;
    static int last_second=0;
    static int start_time=frame_time;

    frame_count=frame_count+1;

    if(last_second<(frame_time-start_time)/1000000) // new second
    {
        last_second=(frame_time-start_time)/1000000;
        cout<<"FPS: "<< frame_count<< "\n";
        frame_count=0;
    }
}





HSV_filter::HSV_filter()
{
        for(int j=0;j<=255;j++)
	{
	for(int k=0;k<=255;k++)
	{
		if(k==0)
			saturation_table[k][j]=0;
		else{
			double	coef=255*(k-j)/k;
			uchar a=k;
			uchar b=j;			
			saturation_table[k][j]=coef;
		}

	}
	}

 

// Tonys green leds sticks
    H_low=30;
    S_low=150;
    V_low=153;

    H_high=61;
    S_high=255;
    V_high=250;
/*
    H_low=101;
    S_low=193;
    V_low=81;

    H_high=115;
    S_high=255;
    V_high=200;
    */
/*
  // purple light from pod
    H_low=111;
    S_low=2;
    V_low=2;

    H_high=136;
    S_high=119;
    V_high=255;
  */   
  
  // red light from pod
  /*
    H_low=102;
    S_low=115;
    V_low=153;

    H_high=121;
    S_high=200;
    V_high=255;
    */
      
    /*
    // red light default
    H_low=110;
    S_low=70;
    V_low=145;

    H_high=145;
    S_high=255;
    V_high=255;
*/
//yellow cap
//    int H_low=80;
//    int S_low=170;
//    int V_low=130;
//
//    int H_high=92;
//    int S_high=255;
//    int V_high=231;
 
//flashlight
//    int H_low=0;
//    int S_low=0;
//    int V_low=125;
//
//    int H_high=53;
//    int S_high=120;
//    int V_high=255;


//--
	//this is because opencv goes betwen 0 and 180 while we go between 0 and 255
	//#if OWN_CV
	H_low=256*H_low/180;
	H_high=256*H_high/180;
	cout<<(int)H_low<<endl;
	cout<<(int)H_high<<endl;
	//#endif
	RGB_filter_table=(uchar*)malloc(256*256*256*sizeof(uchar));
	//RGB_filter_table[256*256*20+256*3+49]=33;
	//cout<<(int)RGB_filter_table[256*256*20+256*3+49]<<endl;
        H_low=108;
        S_low=125;
        V_low=104;

        H_high=125;
        S_high=248;
        V_high=255;

//yelow dock hopefully
        H_low=88;
        S_low=107;
        V_low=176;

        H_high=102;
        S_high=255;
        V_high=255;

//red beacon

        H_low=119;
        S_low=136;
        V_low=176;

        H_high=137;
        S_high=255;
        V_high=255;
	
}

//makes x>y
//safe
#define SWAP(x,y,a,b) if (x <= y) { int tmp =x; x = y; y = tmp;a=b; }
// unsafe
//#define SWAP(x,y,a,b) {*(&x)^=*(&y) ;*(&y)^=*(&x) ;*(&x)^=*(&y);a=b;}
//#define S_TABLE(V,vmin) saturation_table[V][vmin]
void HSV_filter::update_filter_table(){
	for(int a=0;a<=255;++a)
		for(int b=0;b<=255;++b)
			for(int c=0;c<=255;++c)
			{
				RGB_filter_table[a*256*256+b*256+c]=RGB_filter(a,b,c);
			}
			update_table=false;
			//cout<<"created filter"<<endl;
	
}
void HSV_filter::thresholding_table(Mat img,Mat img_grey){
	        uchar channels = img.channels();
            int nRows = img.rows;
            int nCols = img.cols * channels;

        if (img.isContinuous())
        {
	//cout<<"not continuos"<<endl;
            nCols *= nRows;
            nRows = 1;
        }

  		
                for( int i = 0; i < nRows; ++i)
                {
                    uchar *p = img.ptr<uchar>(i);
                    uchar *p_grey=img_grey.ptr<uchar>(i);
		   #pragma omp parallel for num_threads(2)
                    for (int j = 0, j2 = 0; j < nCols; j+=3, ++j2)
                    {
		  	uchar R,G,B;
                        R=p[j];//B 2
                        G=p[j+1];//G 1
                        B=p[j+2];//R 0
/*
			cout<<(R<<16)<<" R   "<<R*256*256<<endl;
			cout<<(G<<8)<<" G   "<<G*256<<endl;
			cout<<(B)<<" B   "<<B<<endl;
			cout<<((R<<16)+(G<<8)+B)<<"  outro "<<R*256*256+256*G+B<<endl;
*/
                        p_grey[j2]=RGB_filter_table[R*256*256+256*G+B];
                    }
                }


}
void HSV_filter::thresholding_dynamic(Mat img,Mat img_grey){
	    uchar channels = img.channels();
            int nRows = img.rows;
            int nCols = img.cols * channels;

        if (img.isContinuous())
        {
            nCols *= nRows;
            nRows = 1;
        }
  		
                for( int i = 0; i < nRows; ++i)
                {
                    uchar *p = img.ptr<uchar>(i);
                    uchar *p_grey=img_grey.ptr<uchar>(i);

                    for (int j = 0,jg=0; j < nCols; j+=3, jg+=1)
                    {
			uchar R,G,B;
                        R=p[j];//B 2
                        G=p[j+1];//G 1
                        B=p[j+2];//R 0
                        //cout<<"sent"<<(int)R<<(int)G<<(int)B<<endl;
                        p_grey[jg]=RGB_filter(R,G,B);
                    }
                }

}
uchar HSV_filter::RGB_filter (uchar R, uchar G, uchar B)
{		
		
		static int counter=0;
		counter++;
		/*if(counter%10000000==0){
		        for(int j=0;j<=255;j++)
	{
	for(int k=0;k<=255;k++)
	{
		int S=saturation_table[k][j];
		cout<< " k "<< k<< " j " <<j<<" S " <<S<<endl;
	}
	}
		
		}*/
		uchar H,S,V,vmid,vmin,greater;
		V=B;
		vmid=G;
		vmin=R;
		H=S=0;
		SWAP(V,vmid,greater,1);
                SWAP(V,vmin,greater,0);
                SWAP(vmid,vmin,greater,greater);



		S=saturation_table[V][vmin];
		if(V==0)
			S=0;
		else{
			double	coef=255*(V-vmin)/V;
			S=(uchar)coef;
			if((coef-S)*(coef-S)>2)
				cout<<" S casting problm" << endl;
		}
		



		if(V!=0&&vmin>254){
			uchar other=(255*(V-vmin)/V);	
			//cout<<"V"<<(int)V<<" vmin"<<(int)vmin<<" S"<<(int)S<<" other"<<(int)other<<endl;
			//cout<<"RGB "<<(int)R<<" "<<(int)G<<" "<<(int)B<<"  S "<<(int)S<<endl;
		}

			if(S!=0){
			double H_float;
			switch(greater){
			
			case 0:
				H=42*(G-B)/(V-vmin);break; //-43(212) to 43
			case 1:
				H=84+42*(B-R)/(V-vmin); //43 to 129
				//cout<<"B "<<(int)B<<" R "<<(int)R<<" V "<<(int)V<<" vmin "<<(int)vmin<<" coef "<<(int)(43*(B-R)/(V-vmin)) <<endl;
				break;
			case 2:
				H=170+42*(R-G)/(V-vmin);// 127 to 213
				
				break;			
			}
			}

		        if((H>=H_low)&&(H<=H_high)&&(S>=S_low)&&(S<=S_high)&&(V>=V_low)&&(V<=V_high))
				return 255;
				else return 0;
}
void HSV_filter::calibrate_window()
{
    // trackbars for determining threshold
    namedWindow( "trackbar", WINDOW_NORMAL );

    createTrackbar( "H_low", "trackbar", &H_low, 255 );
    createTrackbar( "S_low", "trackbar", &S_low, 255 );
    createTrackbar( "V_low", "trackbar", &V_low, 255 );

    createTrackbar(" H_high", "trackbar", &H_high, 255 );
    createTrackbar( "S_high", "trackbar", &S_high, 255 );
    createTrackbar( "V_high", "trackbar", &V_high, 255 );
}


void equalize(Mat frame,Mat img)
{
    vector<Mat> channels; // should we set to static if its going to be used several times?
    cvtColor(frame, img, CV_RGB2YCrCb);
    split(img, channels);
    equalizeHist(channels[0],channels[0]);
    merge(channels, img);
    cvtColor(img, img, CV_YCrCb2RGB);
}

void morphological(Mat img)
{
    // morphological opening (remove small objects from the foreground)
    int size_ellipse= 3;
    erode(img, img, getStructuringElement(MORPH_ELLIPSE, Size(size_ellipse, size_ellipse)));
    dilate(img, img, getStructuringElement(MORPH_ELLIPSE, Size(size_ellipse, size_ellipse)));

    //morphological closing (fill small holes in the foreground)
    dilate(img, img, getStructuringElement(MORPH_ELLIPSE, Size(size_ellipse, size_ellipse)));
    erode(img, img, getStructuringElement(MORPH_ELLIPSE, Size(size_ellipse, size_ellipse)));

}

Mat threshold_HSV(Mat img,HSV_filter f)
{
    cvtColor(img, img, CV_RGB2HSV);
    inRange(img, Scalar( f.H_low, f.S_low, f.V_low ), Scalar(f.H_high, f.S_high, f.V_high), img);
    return img;
}
