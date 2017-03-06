#include "ros/ros.h"
#include "quadrotor_controller/scan_reg.h"
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <gazebo_msgs/GetModelState.h>
#include <gazebo_msgs/SetModelState.h>
#include <geometry_msgs/Twist.h>
#include <sstream>
#include <string>
#include <std_msgs/String.h>
#include <iostream>
#include <tf/LinearMath/Quaternion.h>
#include <stdio.h> 
#include <math.h>


bool executecallback(quadrotor_controller::scan_reg::Request &req,
         quadrotor_controller::scan_reg::Response &res)
{
  ros::NodeHandle nh;
  ros::NodeHandle nh_;
  ros::ServiceClient gms_c;  
  gazebo_msgs::SetModelState setmodelstate;
  gazebo_msgs::GetModelState getmodelstate; 
  ros::Publisher publisher;
  ros::ServiceClient smsl;
  geometry_msgs::Pose end_pose;
  geometry_msgs::Twist end_twist;
  
  ROS_INFO("START HECTOR FOR TASK EXECUTION");
  publisher = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  gms_c = nh_.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
  getmodelstate.request.model_name="red_wasp";
  
  geometry_msgs::Twist tw;
  publisher.publish(tw);
  ros::Duration(2.0).sleep();
  
  gms_c.call(getmodelstate);
  double now_x =  getmodelstate.response.pose.position.x;
  double now_y =  getmodelstate.response.pose.position.y;
  double now_z =  getmodelstate.response.pose.position.z;
  double temp = getmodelstate.response.pose.orientation.z;
  ros::Rate r(1);
  bool success = true;
  publisher.publish(tw);
  ros::Duration(1.0).sleep();
  ROS_INFO("Start task execution ");
 gms_c.call(getmodelstate);
  now_z =  getmodelstate.response.pose.position.z;
  ROS_INFO_STREAM(now_z);
  if(now_z <= 10)
    {
      while(now_z <= 10)
	{
	  tw.linear.z = 0.8;
	  publisher.publish(tw);
	  ros::Duration(1.0).sleep();
	  gms_c.call(getmodelstate);
	  now_z =  getmodelstate.response.pose.position.z;
	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
  
    {
      while(getmodelstate.response.pose.position.z > 3.0)
	{

	  ROS_INFO_STREAM(getmodelstate.response.pose.position.z);
	  
	  tw.linear.z = -0.3;
	  publisher.publish(tw);
	  ros::Duration(1.0).sleep();
	  gms_c.call(getmodelstate);
	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      publisher.publish(tw);
    }
  gms_c.call(getmodelstate);
  temp = getmodelstate.response.pose.orientation.z;
  if(temp <= 0.95)
    {
      while(temp <= 0.91)
	{
	  tw.angular.z = -0.3;
	  publisher.publish(tw);
	  ros::Duration(1.0).sleep();
	  gms_c.call(getmodelstate);
	  temp = getmodelstate.response.pose.orientation.z;
	}
      ros::Duration(2.0).sleep();
      tw.angular.z = 0;
      publisher.publish(tw);
    }
  ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);


  res.reply = "Task Execution completed";
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "execute_command_server");
  ros::NodeHandle n;
  ros::ServiceServer service = n.advertiseService("takeOff", executecallback);
  ROS_INFO("Ready to receive information where to fly");
  ros::spin();
  
  return 0;
}
