#include "ros/ros.h"
#include "std_msgs/String.h"
#include <opencv2/opencv.hpp>
#include <sstream>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include "sensor_msgs/Image.h"
#include <cstdlib>
#include <string>
#include "img_mission/returnString.h"
#include <stdlib.h> 
#include <ros/package.h>

using namespace std;
using namespace cv;


static int indexy;
static int indexi;
static string kette;
static int indexer;
ros::Subscriber sub;


bool check(img_mission::returnString::Request  &req,
	   img_mission::returnString::Response &res)
{  
  std::string path = ros::package::getPath("img_mission") + "/imgs/";
  ROS_INFO_STREAM(path);
  const string test = path+req.goal;
  cv::Mat image,  gray_image;
  ROS_INFO_STREAM(test);
  image = imread(test,CV_LOAD_IMAGE_COLOR);//imread(test,IMREAD_COLOR);  
  ROS_INFO_STREAM("test");
  // cv::namedWindow( "Display Image", WINDOW_AUTOSIZE );
  ROS_INFO_STREAM("test1");

  //  ROS_INFO_STREAM(image);
    if( !image.data)                      // Check for invalid input
    {
        cout <<  "Could not open or find the image" << std::endl ;
        return false;
    }
 
  ROS_INFO_STREAM("test2");

 
  ROS_INFO_STREAM("test3");
  imshow("Display Image",image);
  ROS_INFO_STREAM("test3");
  cv::waitKey();
  res.result = "Spawn Image!";
  indexy = 1;

  return true;
}

int main(int argc, char **argv)
{
  // extern int indexy;
  ros::init(argc, argv, "ShowThatString");
  ros::NodeHandle n;

  ros::Rate loop_rate(2);

  ros::ServiceServer service = n.advertiseService("show_image", check);

  ros::spin();

  return 0;
}
