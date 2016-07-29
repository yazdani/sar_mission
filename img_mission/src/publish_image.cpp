#include "ros/ros.h"
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


void callback(const sensor_msgs::Image& image)
{
  indexy = 0;
  //ROS_INFO_STREAM(image);
  cv_bridge::CvImagePtr cv_ptr = nullptr;
  cv::Mat images;
  ROS_INFO_STREAM("--------------------------------------1111");
  ROS_INFO_STREAM(indexy);
  ROS_INFO_STREAM("--------------------------------------2222");
  if(sensor_msgs::image_encodings::isColor(image.encoding)) {
  ROS_INFO_STREAM("--------------------------------------2sdfsf222");
    cv_ptr = cv_bridge::toCvCopy(image, sensor_msgs::image_encodings::BGR8);
  } 
    ROS_INFO_STREAM("--------------------------------------2ss222");
  if(cv_ptr) {
    cv::Mat imgMat = cv_ptr->image;
    images = imgMat;//imread(imgMat, CV_LOAD_IMAGE_COLOR); 
    std::cout << "Got an image!" << std::endl;
  } 
    ROS_INFO_STREAM("--------------------------------------22dsds22");

    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
    imshow( "Display window", images );               // Show our image inside it.
}



bool check(img_mission::returnString::Request  &req,
	   img_mission::returnString::Response &res)
{
  // sub = nh.subscribe("/camera/rgb/image_raw", 1000, callback);
  ROS_INFO_STREAM(indexy);  ROS_INFO_STREAM(indexy);
  indexy = 1;
  ROS_INFO_STREAM(indexy);
  res.result = "Fertig";
  return true;
}

int main(int argc, char **argv)
{
  // extern int indexy;
  ros::init(argc, argv, "GetThatString");
  ros::NodeHandle n;
ros::NodeHandle nh;
  ros::Rate loop_rate(2);

  indexy = 0;

      ros::ServiceServer service = n.advertiseService("store_name", check);
  while(ros::ok())
    {
      if(indexy == 1)
	{
	  ROS_INFO_STREAM("--------------------------------------");
	    sub = nh.subscribe("/camera/rgb/image_raw", 1000, callback);
	  ROS_INFO_STREAM("--------------------------------------");
	  ROS_INFO_STREAM(indexy);
	  // ROS_INFO_STREAM(indexy == 1);
	  std::cout << indexy << std::endl;
	}  
      ROS_INFO("Ready to store the strings.");
      ROS_INFO_STREAM(indexy);   ROS_INFO("Ready to store the strings."); ROS_INFO_STREAM(indexy);
      ros::spinOnce();
    ROS_INFO("Ready togdg store the strings.");
    loop_rate.sleep();
    }
  //ros::spin();

  return 0;
}
