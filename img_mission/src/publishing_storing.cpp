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
#include "img_mission/Strings.h"
#include <stdlib.h> 
#include <ros/package.h>
#include "cmd_mission/rotate.h"
#include "quadrotor_controller/cmd_points.h"
using namespace std;
using namespace cv;


static int indexy;
static int indexi;
static string kette;
static int indexer;
static std_msgs::String ts;
ros::Publisher pub;

bool check(img_mission::returnString::Request  &req,
	   img_mission::returnString::Response &res)
{  
  
  
   indexer = rand() % 10000;
   stringstream ss;
   ss << indexer;
   string str = ss.str();
   kette = "image_"+str+".jpg";
   res.result = "image_"+str+".jpg";
   img_mission::Strings value;
   value.object = req.goal;
   value.image="image_"+str+".jpg";
   pub.publish(value);
  return true;
}

int main(int argc, char **argv)
{
  // extern int indexy;
  ros::init(argc, argv, "GetThatString");
  ros::NodeHandle n;
 ros::NodeHandle nh;
 pub = nh.advertise<img_mission::Strings>("publishing_image",1000); 
  ros::ServiceServer service = n.advertiseService("store_image", check);
  ros::spin();
  return 0;
}
