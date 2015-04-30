#include<linux/videodev2.h>
#include<errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "filter_v2.hpp"

double X_VALUE;
double Y_VALUE;
unsigned long long TIME;

#define RUN_MOTORS 1
#define OMP_WAIT_POLICY PASSIVE
#define OWN_CV 1
//TODO set the environment variable
// global variables
double WIDTH = 1920;	// frame width
double HEIGHT = 1080; // frame height
double L_power =0;
double R_power =0;
//double TAU=1;
double TARGET_AREA = 0;

//#include <gperftools/tcmalloc.h>
#include <pthread.h>
#include <lcm/lcm-cpp.hpp>
#include "opencv2/opencv.hpp"
#include "vision_core.hpp"
#include "motor_control.hpp"
#include "filter_v2.hpp"
#include <ctime>
#include <string>
#include <fstream>


#define SHOW_THRESHOLDED true
#define SHOW_DETECTED false

#define ROI_limit 200


using namespace cv;
using namespace std;


options_c::options_c()
{
    trackbar=false;
    equalization=false;
    morphological=false;
    roi=true;
    save_video=false;
    power=20;
    area=25;
    tau=1;
}
unsigned long long timeout_gvc=3000;

int main(int, char**)
{
/*
int fd;
fd = open("/dev/video0", O_RDWR | O_NONBLOCK,0);
if(-1==fd){
	cout<<"error opening"<<endl;
}
*/

//-----------------------------INITIALIZATIONS---------------
    system("OMP_WAIT_POLICY=PASSIVE");

    options_c options;
    options_listener options_l(&options);
    pthread_t thread1;
    pthread_create (&thread1,NULL,options_listener::listen,(void*)&options_l.lcm);


    lcm::LCM lcm;
    //if(!lcm.good())
    //    return 2;
    avionics::vision_detection_t detection;
    avionics::motor_command_t command;


// alternative to test more
//CCamera* cam = StartCamera(WIDTH, HEIGHT,15,1,false);

    VideoCapture inputVideo=Initialize_input_video();
    if(!inputVideo.isOpened()) return -10; // check if we succeeded
    VideoWriter outputVideo;
    if (options.save_video) outputVideo=Initialize_output_video(inputVideo,"tracking.avi");

    Mat frame,img, frameDetected, frameThresholded, img1,img2,img3,img4;
    HSV_filter filter;
    Point2f mc, center;
    double x_value=0;
    double y_value=0;

    clock_t start_time=clock();
    clock_t frame_time;
    unsigned long long last_detection_time;

    update_FPS(start_time);// sets the starting time to the FPS counter
    ROI_c ROI, ROI_1,ROI_2,ROI_3,ROI_4;
    bool thresholded_ready=false;
    cout<<"FPS: "<<inputVideo.get(CV_CAP_PROP_FPS)<<endl;

    inputVideo.set(CV_CAP_PROP_FRAME_WIDTH,640);
    inputVideo.set(CV_CAP_PROP_FRAME_HEIGHT,480);

    cout<<HEIGHT<<endl;
    WIDTH=inputVideo.get(CV_CAP_PROP_FRAME_WIDTH);
    HEIGHT=inputVideo.get(CV_CAP_PROP_FRAME_HEIGHT);
     Mat img_grey;
      uchar H_low,S_low,V_low,H_high,S_high,V_high;
      
      //TODO thats a quickfix
      inputVideo >> img; // get a new frame from camera
      img_grey=Mat::zeros(img.rows,img.cols,CV_8UC1);


//------------------------------------------------------
   //for(int rou=1;rou<100000;rou++)

        while(true)
        {

                inputVideo >> img; // get a new frame from camera		
		/*
		frame=img.clone();
		imshow("frame",frame);
		waitKey(3);
		*/
		
		
		img_grey=Mat::zeros(img.rows,img.cols,CV_8UC1);
                frame_time=clock(); // time the frame was taken
                detection.frame_timestamp=frame_time;
                update_FPS(frame_time);

// copies another instance of the frame so we retain the original one for storing purpose
                if(options.save_video||SHOW_DETECTED)frame=img.clone();

// selects only the image portion determined by the Region of interest
                if (options.roi&&ROI.use) 
		{
			img=ROI.crop(img);
			img_grey=ROI.crop(img_grey);
		}

                if(options.equalization) equalize(frame,img);
#if OWN_CV
		//frame=img.clone();
		Mat img_grey=threshold_HSV(img,filter);
#endif

#if !OWN_CV
		if(options.trackbar){
			filter.thresholding_dynamic(img,img_grey);
		}
		else{
			if(filter.update_table) filter.update_filter_table();
			filter.thresholding_table(img,img_grey);
			//imshow("thresh table",img_grey);
			//waitKey(2);
			
		}		 
               	            	
#endif      

          if(options.trackbar)
                {
                    filter.calibrate_window();
                    frameThresholded=img_grey.clone();
                    thresholded_ready=true;
                }


                if (options.morphological) morphological(img);


                // Contour analysis
                int largest_area = 0;
                int largest_contour_index = 0;
                float radius=0; // also used as a flag: if 0, it means we don't do cropping as we haven't found the target
                vector<vector<Point> > contours; // Vector for storing contour
                vector<Vec4i> hierarchy;
                findContours(img_grey, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
                for (int i = 0; i< contours.size(); i++)
                {
                    //  Find the area of contour
                    double a = contourArea(contours[i], false);
                    if (a>largest_area)
                    {
                        largest_area = a;
                        // Store the index of largest contour
                        largest_contour_index = i;
                    }
                }
                detection.number_of_objects=contours.size();
#if SHOW_DETECTED || SAVE_VIDEO
                frameDetected=frame.clone();
#endif

                if (largest_area>options.area) // minimal area
                {
                    detection.detected=true;
	            last_detection_time=get_timestamp();
                    // Find the bounding rectangle for biggest contour
                    minEnclosingCircle(contours[largest_contour_index], center, radius);

                    Moments M = moments(contours[largest_contour_index],false);
                    Point2f mc = Point2f(M.m10 / M.m00, M.m01 / M.m00);
#if SHOW_DETECTED
                    int r_eq = (int)(sqrt(largest_area/ 3.14) );
                    circle(frameDetected, center, (int)radius, Scalar(256, 0, 2),4);
                    circle(frameDetected, mc, 4,Scalar(0,0,0),6);
                    circle(frameDetected, mc,r_eq,Scalar(0,0,256),4 );
#endif
                    mc=ROI.origin+mc;
                    x_value=mc.x;
                    y_value=mc.y;
                    //cout<<ROI.origin<<"  "<<mc<<endl;
                }
                else
                {
                    detection.detected=false;
                    mc.x=1;
                    mc.y=0;
                    radius=0; // TODO check if necessary at the end
                    center= Point2f(0,0);
                    x_value=0;
                    y_value=0;


                }
                detection.x=x_value;
                detection.y=y_value;
		
                detection.area=largest_area;
                detection.timestamp=get_timestamp();
                lcm.publish("VISION",&detection);
                center=ROI.origin+center;
		X_VALUE=x_value;
		Y_VALUE=y_value;
		TIME=detection.timestamp;
                //TAU=options.tau;
		TARGET_AREA = (double)radius;
                //thrust_x(x_value, (frame_time-start_time)/1000);
                //cout<<"suplied x_value   "<< x_value<< "timechange"<<(frame_time-start_time)/1000<<endl;
		//cout<<largest_area<<endl;
#if RUN_MOTORS
  		run_filter(x_value,(unsigned long long)detection.timestamp);

		
		int L_power_int, R_power_int;
/*		
if(detection.detected||(detection.timestamp-last_detection_time)<timeout_gvc){
                L_power_int=options.power*L_power;
                R_power_int=options.power*R_power;
		}
		else{
		L_power_int=0;
                R_power_int=0;
	
		}

		command.L_power=L_power_int;
                command.R_power=R_power_int;
*/
                command.L_power=options.power*L_power;
                command.R_power=options.power*R_power;
                
		command.timestamp=get_timestamp();
		
                lcm.publish("MOTOR",&command);
		cout<<L_power<<"      "<<R_power<< "                  "<<largest_area<<endl;
		//runMotors(L_power_int,R_power_int);
#endif


#if SHOW_DETECTED
                Point2f filter_result = Point2f(x_value,y_value);
                int r_eq = (int)(sqrt(largest_area/ 3.14) );
                circle(frameDetected, center, (int)radius, Scalar(256, 0, 2),4);
                circle(frameDetected, mc, 4,Scalar(0,0,0),6);
                circle(frameDetected, mc,r_eq,Scalar(0,0,256),4 );
                circle(frameDetected,filter_result,4,Scalar(255,255,0),6);
                //imshow("Detected", frameDetected);
#endif

                if (options.trackbar&&thresholded_ready) imshow("frame thresholded", frameThresholded);

                if(options.save_video)outputVideo << frameDetected ;
                if(options.roi)ROI.update(&center,radius);

		
//------------------------------------------------------------------------------------------------------------------------------
                if(options.trackbar)	       
			if(waitKey(1) >= 0)
			{ 
				cvDestroyWindow("frame thresholded");
				cvDestroyWindow("trackbar");
				options.trackbar=false;
				filter.update_table=true;
			}	
	}
       
    //free options
    return 0;
}
uchar RGB2HSV2filter (uchar R,uchar G, uchar B, uchar H, uchar S, uchar V,HSV_filter filter){
		return R;

}
