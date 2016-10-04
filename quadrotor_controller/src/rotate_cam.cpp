#include "ros/ros.h"
#include "quadrotor_controller/cmd_points.h"
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


bool executecallback(quadrotor_controller::cmd_points::Request &req,
         quadrotor_controller::cmd_points::Response &res)
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
  
  ROS_INFO("START HECTOR FOR ROTATION");
  publisher = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  gms_c = nh_.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
  getmodelstate.request.model_name="quadrotor";
  
  geometry_msgs::Twist tw;
  publisher.publish(tw);
  ros::Duration(2.0).sleep();
  
  gms_c.call(getmodelstate);
  double now_x =  getmodelstate.response.pose.position.x;
  double now_y =  getmodelstate.response.pose.position.y;
  double now_z =  getmodelstate.response.pose.position.z;
  double temp = getmodelstate.response.pose.orientation.z;
  double new_x = req.x;
  double new_y = req.y;
  double new_z = req.z;
  double vel_x = req.qx;
  double vel_y = req.qy;
  double new_qz = req.qz ;
  double new_qw = req.qw;
  double vel_qz = 0.0;

  ros::Rate r(1);
  bool success = true;
  publisher.publish(tw);
  ros::Duration(1.0).sleep();
  


  ROS_INFO("Start Rotation ");
  
  temp = getmodelstate.response.pose.orientation.z;
 
  gms_c.call(getmodelstate);
  if(new_qw >= 0)
    {
      if(getmodelstate.response.pose.orientation.w < 0.95 && getmodelstate.response.pose.orientation.w >= 0)
	{
	  while(getmodelstate.response.pose.orientation.w <= 0.95)
	    {
	      ROS_INFO_STREAM("Zet: ");
	      ROS_INFO_STREAM(getmodelstate.response.pose.orientation.z);
	      tw.angular.z = -0.5;
	      ROS_INFO_STREAM("We : ");
	      ROS_INFO_STREAM(getmodelstate.response.pose.orientation.w);
	      publisher.publish(tw);
	      ros::Duration(1.0).sleep();
	      gms_c.call(getmodelstate);
	    }
	  ros::Duration(2.0).sleep();
	  tw.angular.z = 0;
	  publisher.publish(tw);
	}
    }else{
   	while(getmodelstate.response.pose.orientation.w >= -0.95)
	  {
	    ROS_INFO_STREAM("Zet1: ");
	    ROS_INFO_STREAM(getmodelstate.response.pose.orientation.z);
	    tw.angular.z = 0.5;
	    ROS_INFO_STREAM("We : ");
	    ROS_INFO_STREAM(getmodelstate.response.pose.orientation.w);
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	  }
	ros::Duration(2.0).sleep();
	tw.angular.z = 0;
	publisher.publish(tw);
      }
  
  /*
  if(getmodelstate.response.pose.orientation.w >= 0)
    {
      if(getmodelstate.response.pose.orientation.z > new_qz)
	{
	  
	  while(getmodelstate.response.pose.orientation.z >= new_qz)
	    {
	      ROS_INFO_STREAM("Z-11: ");
	      ROS_INFO_STREAM(getmodelstate.response.pose.orientation.z);
	      ROS_INFO_STREAM(new_qz);
	      
	      tw.angular.z = -0.2;
	      publisher.publish(tw);
	      ros::Duration(1.0).sleep();
	      gms_c.call(getmodelstate);
	    }
	  
	  ros::Duration(2.0).sleep();
	  tw.angular.z = 0;
	  publisher.publish(tw);
	}else 
	{	  
	  while(getmodelstate.response.pose.orientation.z <= new_qz)
	    {
	      ROS_INFO_STREAM("Z-12: ");
	      ROS_INFO_STREAM(getmodelstate.response.pose.orientation.z);
	      ROS_INFO_STREAM(new_qz);
	      tw.angular.z = 0.2;
	      publisher.publish(tw);
	      ros::Duration(1.0).sleep();
	      gms_c.call(getmodelstate);
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
      gms_c.call(getmodelstate);
    }else
    {
            if(getmodelstate.response.pose.orientation.w < 0.95)
	      {
		while(getmodelstate.response.pose.orientation.w <= 0.95)
		  {
		    ROS_INFO_STREAM("Z-22: ");
		    tw.angular.z = -0.5;
		    publisher.publish(tw);
		    ros::Duration(1.0).sleep();
		    gms_c.call(getmodelstate);
		  }
		ros::Duration(1.0).sleep();
		tw.angular.z = 0;
		publisher.publish(tw);
	      }
	    else{
	      while(getmodelstate.response.pose.orientation.w >= -0.95)
		{
		  ROS_INFO_STREAM("Z-23: ");
		  tw.angular.z = 0.5;
		  publisher.publish(tw);
		  ros::Duration(1.0).sleep();
		  gms_c.call(getmodelstate);
		}
	      ros::Duration(1.0).sleep();
	      tw.angular.z = 0;
	      publisher.publish(tw);
	    }
    }
  */

  // if(getmodelstate.response.pose.orientation.w >= 0)
  //  {
      if(getmodelstate.response.pose.orientation.z > new_qz)
	{
	  
	  while(getmodelstate.response.pose.orientation.z >= new_qz)
	    {
	      ROS_INFO_STREAM("Z1: ");
	      ROS_INFO_STREAM(getmodelstate.response.pose.orientation.z);
	      ROS_INFO_STREAM(new_qz);
	      
	      tw.angular.z = 0.25;
	      publisher.publish(tw);
	      ros::Duration(1.0).sleep();
	      gms_c.call(getmodelstate);
	      ROS_INFO_STREAM("Z1: ");
	      ROS_INFO_STREAM(getmodelstate.response.pose.orientation.z);
	      ROS_INFO_STREAM(new_qz);
	      
	    }
	  
	  ros::Duration(1.0).sleep();
	  tw.angular.z = 0;
	  publisher.publish(tw);
	}else 
	{	  
	  while(getmodelstate.response.pose.orientation.z <= new_qz)
	    {
	      ROS_INFO_STREAM("Z2: ");
	      ROS_INFO_STREAM(getmodelstate.response.pose.orientation.z);
	      ROS_INFO_STREAM(new_qz);
	      tw.angular.z = -0.25;
	      publisher.publish(tw);
	      ros::Duration(1.0).sleep();
	      gms_c.call(getmodelstate);
	      ROS_INFO_STREAM("Z2: ");
	      ROS_INFO_STREAM(getmodelstate.response.pose.orientation.z);
	      ROS_INFO_STREAM(new_qz);
	    }
	  
	  ros::Duration(1.0).sleep();
	  tw.angular.z = 0;
	  publisher.publish(tw);
	}
      
     ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
      gms_c.call(getmodelstate);
      //  }

     

  res.repl = "Rotation completed";
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "rotate_command_server");
  ros::NodeHandle n;
  ros::ServiceServer service = n.advertiseService("setRobotRotation", executecallback);
  ROS_INFO("Ready to receive information based on rotation");
  ros::spin();
  
  return 0;
}
