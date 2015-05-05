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
#include <iomanip> // for setw 
//#include "filter_v2.hpp"

double X_VALUE;
double Y_VALUE;
unsigned long long TIME;

#define RUN_MOTORS 1
#define OMP_WAIT_POLICY PASSIVE
#define OWN_CV 0
#define CAMERA_LOG 0
#define DISPLAY 1
#define SWITCHING 0
//TODO set the environment variable
// global variables
double WIDTH = 1920;	// frame width
double HEIGHT = 1080; // frame height
double L_power =0;
double R_power =0;
double TAU=1;
double TARGET_AREA = 0;

//#include <gperftools/tcmalloc.h>
#include <pthread.h>
#include <lcm/lcm-cpp.hpp>
#include "opencv2/opencv.hpp"
#include "vision_core.hpp"
#include "motor_control.hpp"
#include "filter_v1.hpp"
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
    roi=false;
    save_video=false;
    power=20;
    area=25;
    tau=1;
}
unsigned long long timeout_gvc=2000;

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
    avionics::camera_logger_t camera_log;
    avionics::mode_status_t mode_status; //1 looking for the pod and 2 when you see it
    mode_status.Mode_ID=2;



    Mat frame,img, frameDetected, frameThresholded;
    HSV_filter filter_dock=HSV_filter((Color)yellow);
    HSV_filter filter_beacon_red=HSV_filter((Color)red);
    HSV_filter filter_beacon_green=HSV_filter((Color)green);
    HSV_filter *filter;
#if OWN_CV
    if(!options.trackbar){
	filter_beacon_green.update_filter_table();
#if SWITCHING
	filter_dock.update_filter_table();
	filter_beacon_red.update_filter_table();
#endif

     }
#endif

    Point2f mc, center;
    double x_value=0;
    double y_value=0;
    float radius;
    int largest_area;


	// alternative to test more
//CCamera* cam = StartCamera(WIDTH, HEIGHT,15,1,false);

    VideoCapture inputVideo=Initialize_input_video();
    if(!inputVideo.isOpened()) return -10; // check if we succeeded
    VideoWriter outputVideo;
    if (options.save_video) outputVideo=Initialize_output_video(inputVideo,"tracking.h264");

    clock_t start_time=clock();
    clock_t frame_time;
    unsigned long long last_detection_time;

    update_FPS_and_status(false,start_time);// sets the starting time to the FPS counter
    search_status search;
    ROI_c ROI;
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
                frame_time=clock(); // time the frame was taken
		detection.frame_timestamp=get_timestamp();


#if CAMERA_LOG
		//camera logger
		char filename [200];
		char timestamp_string[50];
		
		camera_log.timestamp=detection.frame_timestamp;
		strcpy(filename,"/home/pi/avx/src/vision/camera_log/frame_");
		sprintf(timestamp_string,"%lld",camera_log.timestamp);
		strcat(filename,timestamp_string);
		strcat(filename,".jpg");
		imwrite(filename,img);
		camera_log.filename=filename;
		lcm.publish("CAMERA_LOG",&camera_log);
		//imwrite("/home/pi/avx/src/vision/camera_log/frame.jpg",img);
#endif
		
		//to show the image on the screen	
#if DISPLAY
		frame=img.clone();
		imshow("frame",frame);
		waitKey(3);
#endif
		
		
		
		

// copies another instance of the frame so we retain the original one for storing purpose
                if(options.save_video||SHOW_DETECTED)frame=img.clone();


Color target;
int number_of_scans;
if(search.secondary.color==none) number_of_scans=1;
else number_of_scans=2;


for (int scan_number=number_of_scans;scan_number>=1;scan_number--){ // we start from 2 and decrease because we only crop in 1
	img_grey=Mat::zeros(img.rows,img.cols,CV_8UC1);

	switch (scan_number) {
		case 1: target=search.primary.color; break;
		case 2: target=search.secondary.color; break;
	}
	switch (target) {
		case red: filter=&filter_beacon_red; break;
		case green: filter=&filter_beacon_green; break;
		case yellow: filter=&filter_dock; break;
	}


// selects only the image portion determined by the Region of interest
                if (options.roi&&ROI.use&&scan_number==1) 
		{
			img=ROI.crop(img);
			img_grey=ROI.crop(img_grey);
		}

                if(options.equalization) equalize(frame,img);
#if !OWN_CV
		frame=img.clone();
		Mat img_grey=threshold_HSV(frame,*filter);
#endif

#if OWN_CV
		if(options.trackbar){
			(*filter).thresholding_dynamic(img,img_grey);
		}
		else{
			if((*filter).update_table) (*filter).update_filter_table();
			(*filter).thresholding_table(img,img_grey);
			//imshow("table",img_grey);
			//waitKey(2);
			
			
		}
		 
               	            	
#endif      
		thresholded_ready=false;
         	 if(options.trackbar&&scan_number==1)
                {
                    (*filter).calibrate_window();
                    frameThresholded=img_grey.clone();
                    thresholded_ready=true;
                }
		
                if (options.morphological) morphological(img);


                // Contour analysis
                largest_area = 0;
                int largest_contour_index = 0;
                radius=0; // also used as a flag: if 0, it means we don't do cropping as we haven't found the target
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

		switch (scan_number) {
			case 1: search.primary.found=detection.detected; break;
			case 2: 
				search.secondary.found=detection.detected; 
				//cout<<"searched for secondary result: "<<(int)detection.detected<<"area: "<< largest_area<<" target "<<target<<endl; 
				break;
		}
		
		
                detection.x=x_value;
                detection.y=y_value;
		
                detection.area=largest_area;
                detection.timestamp=get_timestamp();
		detection.type=scan_number;
		detection.color=target;
                lcm.publish("VISION",&detection);
		
}
		int mode_status_to_publish=update_FPS_and_status(detection.detected,frame_time);
		if(mode_status_to_publish){
			mode_status.Status_ID=mode_status_to_publish;
			lcm.publish("mode2_status",&mode_status);
		}
				// change of target
		/*
			start- primary green
			secondary- red, yellow, red, yellow
			
			found red- do nothing for now, but should tell task manager
			found yellow- switch to primary yellow and secondary none
			
		*/
		// report status for debugging
		cout<<" cycle: "<<setw(2)<<search.cycle<<" scans: "<<number_of_scans<<" prim: "<<search.primary.color<<" r:"<<search.primary.found\
					     <<" second: "<<search.secondary.color<<" r:"<<search.secondary.found<<"  ";
		//if(number_of_scans==2) cout<<cout<<" Also searched: "<< search.secondary.color<<" result: "<<search.secondary.found;
		//cout<<endl;
		
#if SWITCHING
		search.update();
#endif
		search.primary.found=search.secondary.found=false;
                center=ROI.origin+center;
		X_VALUE=x_value;
		Y_VALUE=y_value;
		TIME=detection.timestamp;
                //TAU=options.tau;
		TARGET_AREA = (double)radius;
                thrust_x(x_value, (frame_time-start_time)/1000);
                //cout<<"suplied x_value   "<< x_value<< "timechange"<<(frame_time-start_time)/1000<<endl;
		//cout<<largest_area<<endl;
#if RUN_MOTORS
  		//run_filter(x_value,(unsigned long long)detection.timestamp);
		
		
		int L_power_int, R_power_int;
		
		if(detection.detected||(detection.timestamp-last_detection_time)<timeout_gvc){
                L_power_int=options.power*L_power;
                R_power_int=options.power*R_power;
		}
		else{
		L_power_int=0;
                R_power_int=0;
			
		if(search.primary.color==yellow) search=search_status();
		}
		
		////////switched/////////
		command.L_power=R_power_int;
                command.R_power=L_power_int;

               // command.L_power=options.power*L_power;
               // command.R_power=options.power*R_power;
                
		command.timestamp=get_timestamp();
		
                lcm.publish("mode_2",&command);
		cout<<" cmd: "<<command.L_power<<"    "<<command.R_power<< "       "<<largest_area<<endl;
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
				(*filter).update_table=true;
			}	

	}
       
    //free options
    return 0;
}
/*
uchar RGB2HSV2filter (uchar R,uchar G, uchar B, uchar H, uchar S, uchar V,HSV_filter filter){
		return R;

}*/
