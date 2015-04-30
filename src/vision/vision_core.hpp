#include "opencv2/opencv.hpp"
#include <lcm/lcm-cpp.hpp>
#include "../lcm_headers/cpp/all_cpp_lcm_types.hpp"
#include <sys/time.h> //for the epoch and gettimeofday

using namespace cv;
using namespace std;

class options_c
{
    //TODO use a more efficient way of storing bool, if necessary. I dont plan to use it much anyways
public:
    bool trackbar;
    bool equalization;
    bool morphological;
    bool roi;
    bool save_video;
    int area;
    int power;
    int tau;
    options_c();
};

unsigned long long get_timestamp();

class options_listener{
	public:
	options_c* options;
	lcm::LCM lcm;
	
	options_listener(options_c* options_input);

	void handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const avionics::vision_options_t* msg);
                
	static void* listen(void* input_ptr);
};


VideoCapture Initialize_input_video();

VideoWriter Initialize_output_video(VideoCapture inputVideo,string NAME );

class ROI_c
{
public:
    Point2f origin;
    int x_high;
    int y_high;
    int radius;
    bool use;
    int counter;
    void update(Point2f* center, int radius_new);
    ROI_c();
    Mat crop(Mat);
};

void update_FPS(int frame_time);

class HSV_filter
    // HSV upper and lower bounds for the filters
{
public:
    bool update_table;
    int H_low,S_low,V_low;
    int H_high,S_high,V_high;
    uchar saturation_table[256][256];
    uchar *RGB_filter_table;
    HSV_filter();
    void calibrate_window();
    void update_filter_table();
    uchar RGB_filter(uchar R, uchar G, uchar B);
   void thresholding_table(Mat img,Mat img_grey);
   void thresholding_dynamic(Mat img,Mat img_grey);
};


void equalize(Mat frame,Mat img);

void morphological(Mat img);



Mat threshold_HSV(Mat img,HSV_filter f);
