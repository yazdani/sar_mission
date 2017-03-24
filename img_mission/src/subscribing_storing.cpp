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
static int value;

void callback(const sensor_msgs::ImageConstPtr& msg)
{
  if(value == 1)
    {
      std::string path = ros::package::getPath("img_mission") + "/imgs/";
      cv::imwrite(path+kette,cv_bridge::toCvShare(msg, "bgr8")->image);
      cv::waitKey(30);
      value = 0;
    }
}

void call(const img_mission::Strings& msg)
{

  ros::NodeHandle n_rot;
  ros::NodeHandle n_getrot;

  ros::ServiceClient client_getrot = n_getrot.serviceClient<cmd_mission::rotate>("get_rotation");
  cmd_mission::rotate srv_getrot;
  std::string str1 = msg.object;
  srv_getrot.request.goal = msg.object;
  if(str1.compare("picture") != 0)
    {
      if (client_getrot.call(srv_getrot))
	{
	  ROS_INFO("COOL JOB for ROTATION");
	}else
	{
	  ROS_ERROR("Failed to call service getRotation");
	} 
      
      ros::ServiceClient client_rot = n_rot.serviceClient<quadrotor_controller::cmd_points>("setRobotRotation");
      quadrotor_controller::cmd_points srv_rot;
      srv_rot.request.qx = srv_getrot.response.result.orientation.x;
      srv_rot.request.qy = srv_getrot.response.result.orientation.y;
      srv_rot.request.qz = srv_getrot.response.result.orientation.z;
      srv_rot.request.qw = srv_getrot.response.result.orientation.w;
      if (client_rot.call(srv_rot))
	{
	  ROS_INFO("COOL JOB");
	}else
	{
	  ROS_ERROR("Failed to call service setRobotRotation");
	}
    }
 kette = msg.image;
 value = 1;
 ros::NodeHandle n;
 ros::Subscriber  suby = n.subscribe("/camera/rgb/image_raw", 1, callback);
 ros::spin();
}



int main(int argc, char **argv)
{
  ros::init(argc, argv, "StoreThatImage");
  ros::NodeHandle nh;
  value = 0;
  ros::Subscriber sub = nh.subscribe("publishing_image", 1, call);
  ros::spin();
  return 0;
}
