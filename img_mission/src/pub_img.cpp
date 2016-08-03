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
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
//#include <opencv2/highgui.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
//! [includes]
#include "opencv2/objdetect.hpp" 
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
//! [namespace]
using namespace cv;
//! [namespace]

using namespace std;

int main( int argc, char** argv )
{
    //! [load]
    String imageName( "/home/yazdani/work/ros/indigo/catkin_ws/src/sar_mission/img_mission/imgs/HappyFish.jpg" ); // by default
    if( argc > 1)
    {
        imageName = argv[1];
    }
    //! [load]
    cout <<  imageName << std::endl ;
    //! [mat]
    cv::Mat image;
    //! [mat]

    //! [imread]
    image = cv::imread( imageName, CV_LOAD_IMAGE_COLOR); // Read the file
    //! [imread]

    if( image.empty() )                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
     cout <<  "tetetetetest" << std::endl ;
    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    //! [window]

    //! [imshow]
    cout <<  "tetetetetessssst" << std::endl ;
     imshow( "Display window", image      );                // Show our image inside it.
    //! [imshow]

    //! [wait]
    waitKey(0); // Wait for a keystroke in the window
    //! [wait]
    return 0;
}
