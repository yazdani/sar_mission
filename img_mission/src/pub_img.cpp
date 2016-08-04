/*#include "ros/ros.h"
#include "std_msgs/String.h"

#include <sstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include "sensor_msgs/Image.h"
#include <cstdlib>
#include <string>
#include "img_mission/returnString.h"


using namespace std;
using namespace cv;


static int indexy;
ros::Subscriber sub;


void callback(const sensor_msgs::ImageConstPtr& msg)
{
  cv_bridge::CvImagePtr cv_ptr;
  cv_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::BGR8);
  std::cout << cv_ptr->image << std::endl;
  
  // imshow("output",  cv_bridge::toCvShare(msg, "rgb8")->image);   
  indexy = indexy + 1;// Show our image inside it.
  cv::waitKey(30);
  // destroyWindow("Display window");
}

int main(int argc, char **argv)
{
  // extern int indexy;
  ros::init(argc, argv, "GetThatString");
  ros::NodeHandle nh;
  ros::Rate loop_rate(1 );
  indexy = 0;
  while(indexy < 4)
    {
      ROS_INFO_STREAM("--------------------------------------");
      sub = nh.subscribe("/camera/rgb/image_raw", 1000, callback);
      //namedWindow( "Display window");
      ros::spinOnce();
      loop_rate.sleep();
    }
  
  ROS_INFO("Ready togdg store the strings.");
  
  return 0;
}
*/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
  std::cout<<"BLA HLA"<<std::endl;
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], CV_LOAD_IMAGE_COLOR);   // Read the file

    if(! image.data )                              // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    std::cerr<<image.rows<<std::endl;
    //    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow("asd", image );                   // Show our image inside it.

    waitKey(0);                                          // Wait for a keystroke in the window
    return 0;
}
