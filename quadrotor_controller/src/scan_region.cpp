#include "ros/ros.h"
#include <ros/package.h>
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
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <tf/LinearMath/Quaternion.h>
#include <stdio.h> 
#include <math.h>
#include <cstdlib>
#include "img_mission/returnString.h"

static std_msgs::String ts;

bool execute(quadrotor_controller::scan_reg::Request &req,
	     quadrotor_controller::scan_reg::Response &res)
{
  ros::NodeHandle nh;
  ros::NodeHandle nh_;
  ros::NodeHandle nh_cam;
  ros::NodeHandle nh_takeOff;
  ros::ServiceClient gms_c;  
  gazebo_msgs::SetModelState setmodelstate;
  gazebo_msgs::GetModelState getmodelstate; 
  ros::Publisher publisher;
  ros::ServiceClient smsl;
  ros::ServiceClient cam;
  ros::ServiceClient takeOff;
  geometry_msgs::Pose end_pose;
  geometry_msgs::Twist end_twist;
  double new_x;
  double new_y;
  double new_z;
  ROS_INFO_STREAM("Start controller");
  publisher = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
  gms_c = nh_.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
  getmodelstate.request.model_name="quadrotor";
  cam = nh_cam.serviceClient<img_mission::returnString>("/store_image");
  takeOff = nh_takeOff.serviceClient<quadrotor_controller::scan_reg>("/takeOff");
  img_mission::returnString retsrv;
  quadrotor_controller::scan_reg retscanregion;
  geometry_msgs::Twist tw;
  publisher.publish(tw);
  ros::Duration(5.0).sleep();
  
  gms_c.call(getmodelstate);
  double safe_x =  getmodelstate.response.pose.position.x;
  double safe_y =  getmodelstate.response.pose.position.x;
  double safe_z =  getmodelstate.response.pose.position.x;
  double now_x =  getmodelstate.response.pose.position.x;
  double now_y =  getmodelstate.response.pose.position.y;
  double now_z =  getmodelstate.response.pose.position.z;
  double temp = 0.0;
  ros::Rate r(1);
  bool success = true;
  publisher.publish(tw);
  ros::Duration(2.0).sleep();
  
  ROS_INFO("Hector is starting its motors!");
  
  new_x = -7.55;
  new_y = -26;
  new_z = 10;
  
  retscanregion.request.start = "";
  if (takeOff.call(retscanregion))
    {
      ROS_INFO_STREAM(retscanregion.response.reply);
    }
  else
    {
      ROS_ERROR("Failed to call takeOff");
      return 1;
    }
  
  retsrv.request.goal = "";
  if (cam.call(retsrv))
    {
      ROS_INFO_STREAM(retsrv.response.result);
    }
  else
    {
      ROS_ERROR("Failed to call service store image");
      return 1;
    }
  gms_c.call(getmodelstate);
  now_x =  getmodelstate.response.pose.position.x;
  now_y =  getmodelstate.response.pose.position.y;
  now_z =  getmodelstate.response.pose.position.z;

  if(now_x <= new_x)
    {
      while(now_x <= new_x)
	{
	  tw.linear.x = - 0.6;
	  publisher.publish(tw);
	  ros::Duration(1.0).sleep();
	  gms_c.call(getmodelstate);
	  now_x =  getmodelstate.response.pose.position.x;
	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  else
    {     
      ros::Duration(2.0).sleep();
      tw.angular.z = 0;
      publisher.publish(tw);
      while(now_x > new_x)
	{
	  tw.linear.x = 0.6;
	  publisher.publish(tw);
	  ros::Duration(1.0).sleep();
	  gms_c.call(getmodelstate);
	  now_x =  getmodelstate.response.pose.position.x;
	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
 
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
 	{
 	  tw.linear.y = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
    {
      while(now_y > new_y)
 	{
 	  tw.linear.y = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	} 
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

    if(getmodelstate.response.pose.position.z > 5)
    {
      while(getmodelstate.response.pose.position.z > 5)
 	{
 	  tw.linear.z = -0.8;
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
    }

  ros::Duration(2.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

retsrv.request.goal = "";
  if (cam.call(retsrv))
  {
    ROS_INFO_STREAM(retsrv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service store image");
    return 1;
  }

    if(getmodelstate.response.pose.position.z < 10)
    {
      while(getmodelstate.response.pose.position.z < 10)
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
    }
 ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

  new_x = -17;
  new_y = -19;
   gms_c.call(getmodelstate);
   //second position
  now_x =  getmodelstate.response.pose.position.x;

  if(now_x <= new_x)
    {
      while(now_x <= new_x)
	{
	  tw.linear.x = - 0.6;
	  publisher.publish(tw);
	  ros::Duration(1.0).sleep();
	  gms_c.call(getmodelstate);
	  now_x =  getmodelstate.response.pose.position.x;
	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  else
    {
  ROS_INFO("first positionasdsa");
      while(now_x > new_x)
	{
	  tw.linear.x =  0.6;
	  publisher.publish(tw);
	  ros::Duration(1.0).sleep();
	  gms_c.call(getmodelstate);
	  now_x =  getmodelstate.response.pose.position.x;
	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);

  now_y =  getmodelstate.response.pose.position.y;
  if(now_y <= new_y && new_y <= 0)
    {
      while(now_y <= new_y)
	{
	  tw.linear.y = - 0.6;
	  publisher.publish(tw);
	  ros::Duration(1.0).sleep();
	  gms_c.call(getmodelstate);
	  now_y =  getmodelstate.response.pose.position.y;
	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else{
      while(now_y > new_y)
 	{
 	  tw.linear.y = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }

  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);

  
    if(getmodelstate.response.pose.position.z > 5)
    {
      while(getmodelstate.response.pose.position.z > 5)
 	{
 	  tw.linear.z = -0.8;
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
    }

  ros::Duration(2.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

  retsrv.request.goal = "";
  if (cam.call(retsrv))
  {
    ROS_INFO_STREAM(retsrv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service store image");
    return 1;
  }

    if(getmodelstate.response.pose.position.z < 10)
    {
      while(getmodelstate.response.pose.position.z < 10)
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
    }
 ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     


  gms_c.call(getmodelstate);

  new_x = -8;
  new_y = -10;
  now_x =  getmodelstate.response.pose.position.x;

  //third position
  if(now_x <= new_x)
    {
      while(now_x <= new_x)
 	{
 	  tw.linear.x = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  else
    {
      while(now_x > new_x)
 	{
 	  tw.linear.x = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  now_y =  getmodelstate.response.pose.position.y;
  
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
 	{
 	  tw.linear.y = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
    {
      while(now_y > new_y)
 	{
 	  tw.linear.y = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);

    if(getmodelstate.response.pose.position.z > 5)
    {
      while(getmodelstate.response.pose.position.z > 5)
 	{
 	  tw.linear.z = -0.8;
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
    }

  ros::Duration(2.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

retsrv.request.goal = "";
  if (cam.call(retsrv))
  {
    ROS_INFO_STREAM(retsrv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service store image");
    return 1;
  }

    if(getmodelstate.response.pose.position.z < 10)
    {
      while(getmodelstate.response.pose.position.z < 10)
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
    }
 ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

  gms_c.call(getmodelstate);

  new_x = 25;
  new_y = -10;
  //  fourth        
  now_x =  getmodelstate.response.pose.position.x;
	  
  if(now_x <= new_x)
    {
      while(now_x <= new_x)
  	{
  	  tw.linear.x = -0.6;
  	  publisher.publish(tw);
  	  ros::Duration(1.0).sleep();
  	  gms_c.call(getmodelstate);
  	  now_x =  getmodelstate.response.pose.position.x;
  	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  else
    {
      while(now_x > new_x)
  	{
  	  tw.linear.x = 0.6;
  	  publisher.publish(tw);
  	  ros::Duration(1.0).sleep();
  	  gms_c.call(getmodelstate);
  	  now_x =  getmodelstate.response.pose.position.x;
  	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  now_y =  getmodelstate.response.pose.position.y;
	  
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
  	{
  	  tw.linear.y = -0.6;
  	  publisher.publish(tw);
  	  ros::Duration(1.0).sleep();
  	  gms_c.call(getmodelstate);
  	  now_y =  getmodelstate.response.pose.position.y;
  	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else if(now_y > new_y)
    {
      while(now_y > new_y)
  	{
  	  tw.linear.y = 0.6;
  	  publisher.publish(tw);
  	  ros::Duration(1.0).sleep();
  	  gms_c.call(getmodelstate);
  	  now_y =  getmodelstate.response.pose.position.y;
  	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);


  retsrv.request.goal = "";
  if (cam.call(retsrv))
    {
      ROS_INFO_STREAM(retsrv.response.result);
    }
  else
    {
      ROS_ERROR("Failed to call service store image");
      return 1;
    }
  gms_c.call(getmodelstate);

  now_x =  getmodelstate.response.pose.position.x;
	  
  new_x = 29.99;
  new_y = 3.0;
  //fifth position        
  if(now_x <= new_x)
    {
      while(now_x <= new_x)
  	{
  	  tw.linear.x = -0.6;
  	  publisher.publish(tw);
  	  ros::Duration(1.0).sleep();
  	  gms_c.call(getmodelstate);
  	  now_x =  getmodelstate.response.pose.position.x;
  	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  else
    {
      while(now_x > new_x)
  	{
  	  tw.linear.x = 0.6;
  	  publisher.publish(tw);
  	  ros::Duration(1.0).sleep();
  	  gms_c.call(getmodelstate);
  	  now_x =  getmodelstate.response.pose.position.x;
  	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  now_y =  getmodelstate.response.pose.position.y;
	  
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
  	{
  	  tw.linear.y = -0.6;
  	  publisher.publish(tw);
  	  ros::Duration(1.0).sleep();
  	  gms_c.call(getmodelstate);
  	  now_y =  getmodelstate.response.pose.position.y;
  	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
    {
      while(now_y > new_y)
  	{
  	  tw.linear.y = 0.6;
  	  publisher.publish(tw);
  	  ros::Duration(1.0).sleep();
  	  gms_c.call(getmodelstate);
  	  now_y =  getmodelstate.response.pose.position.y;
  	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }   
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  
    if(getmodelstate.response.pose.position.z > 7)
    {
      while(getmodelstate.response.pose.position.z > 7)
 	{
 	  tw.linear.z = -0.8;
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
    }

  ros::Duration(2.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

retsrv.request.goal = "";
  if (cam.call(retsrv))
  {
    ROS_INFO_STREAM(retsrv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service store image");
    return 1;
  }
  gms_c.call(getmodelstate);
    if(getmodelstate.response.pose.position.z < 10)
    {
      while(getmodelstate.response.pose.position.z < 10)
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
    }
 ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

  new_x = 9.59;
  new_y = 1.95;
  
  
  gms_c.call(getmodelstate);

  now_x =  getmodelstate.response.pose.position.x;

  //sixth position        
  if(now_x <= new_x)
    {
      while(now_x <= new_x)
 	{
 	  tw.linear.x = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  else
    {
      while(now_x > new_x)
 	{
 	  tw.linear.x = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  now_y =  getmodelstate.response.pose.position.y;
  
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
 	{
 	  tw.linear.y = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
    {
      while(now_y > new_y)
 	{
 	  tw.linear.y = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  
    if(getmodelstate.response.pose.position.z > 5)
    {
      while(getmodelstate.response.pose.position.z > 5)
 	{
 	  tw.linear.z = -0.8;
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
    }

  ros::Duration(2.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

retsrv.request.goal = "";
  if (cam.call(retsrv))
  {
    ROS_INFO_STREAM(retsrv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service store image");
    return 1;
  }

    if(getmodelstate.response.pose.position.z < 10)
    {
      while(getmodelstate.response.pose.position.z < 10)
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
    }
 ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     
 gms_c.call(getmodelstate);


  new_x = -1.4;
  new_y = 5.0;
  //seventh position        
  now_x =  getmodelstate.response.pose.position.x;

  if(now_x <= new_x)
    {
      while(now_x <= new_x)
 	{
 	  tw.linear.x = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  else
    {
      while(now_x > new_x)
 	{
 	  tw.linear.x = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  now_y =  getmodelstate.response.pose.position.y;
  
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
 	{
 	  tw.linear.y = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
    {
      while(now_y > new_y)
 	{
 	  tw.linear.y = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  
    if(getmodelstate.response.pose.position.z > 5)
    {
      while(getmodelstate.response.pose.position.z > 5)
 	{
 	  tw.linear.z = -0.8;
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
    }

  ros::Duration(2.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     


retsrv.request.goal = "";
  if (cam.call(retsrv))
  {
    ROS_INFO_STREAM(retsrv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service store image");
    return 1;
  }

    if(getmodelstate.response.pose.position.z < 10)
    {
      while(getmodelstate.response.pose.position.z < 10)
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
    }
 ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     
  gms_c.call(getmodelstate);
 
  new_x = -16.45;
  new_y = -2.0;
  //seventh position
  now_x =  getmodelstate.response.pose.position.x;

  if(now_x <= new_x)
    {
      while(now_x <= new_x)
 	{
 	  tw.linear.x = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  else
    {
      while(now_x > new_x)
 	{
 	  tw.linear.x = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  now_y =  getmodelstate.response.pose.position.y;
  
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
 	{
 	  tw.linear.y = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
    {
      while(now_y > new_y)
 	{
 	  tw.linear.y = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  
    if(getmodelstate.response.pose.position.z > 5)
    {
      while(getmodelstate.response.pose.position.z > 5)
 	{
 	  tw.linear.z = -0.8;
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
    }

  ros::Duration(2.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

retsrv.request.goal = "";
  if (cam.call(retsrv))
  {
    ROS_INFO_STREAM(retsrv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service store image");
    return 1;
  }

    if(getmodelstate.response.pose.position.z < 10)
    {
      while(getmodelstate.response.pose.position.z < 10)
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
    }
 ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

 gms_c.call(getmodelstate);
  new_x = -32.99;
  new_y = 3.96;
  //eigth position
  now_x =  getmodelstate.response.pose.position.x;

  if(now_x <= new_x)
    {
      while(now_x <= new_x)
 	{
 	  tw.linear.x = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  else
    {
      while(now_x > new_x)
 	{
 	  tw.linear.x = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);

  now_y =  getmodelstate.response.pose.position.y;
	  
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
 	{
 	  tw.linear.y = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
    {
      while(now_y > new_y)
 	{
 	  tw.linear.y = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
 
  retsrv.request.goal = "";
  if (cam.call(retsrv))
  {
    ROS_INFO_STREAM(retsrv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service store image");
    return 1;
  }
  gms_c.call(getmodelstate);
  new_x = 7.49;
  new_y = 17;
  //nineth position        
  now_x =  getmodelstate.response.pose.position.x;

  if(now_x <= new_x)
    {
      while(now_x <= new_x)
 	{
 	  tw.linear.x = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  else
    {
      while(now_x > new_x)
 	{
 	  tw.linear.x = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  now_y =  getmodelstate.response.pose.position.y;
  
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
 	{
 	  tw.linear.y = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
    {
      while(now_y > new_y)
 	{
 	  tw.linear.y = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  
    if(getmodelstate.response.pose.position.z > 5)
    {
      while(getmodelstate.response.pose.position.z > 5)
 	{
 	  tw.linear.z = -0.8;
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
    }

  ros::Duration(2.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

retsrv.request.goal = "";
  if (cam.call(retsrv))
  {
    ROS_INFO_STREAM(retsrv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service store image");
    return 1;
  }

    if(getmodelstate.response.pose.position.z < 10)
    {
      while(getmodelstate.response.pose.position.z < 10)
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
    }
 ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

 gms_c.call(getmodelstate);
  new_x = -42.99;
  new_y = 2.99;
  //eleventh position        
  now_x =  getmodelstate.response.pose.position.x;

  if(now_x <= new_x)
    {
      while(now_x <= new_x)
 	{
 	  tw.linear.x = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  else
    {
      while(now_x > new_x)
 	{
 	  tw.linear.x = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  now_y =  getmodelstate.response.pose.position.y;
  
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
 	{
 	  tw.linear.y = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
    {
      while(now_y > new_y)
 	{
 	  tw.linear.y = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  
    if(getmodelstate.response.pose.position.z > 5)
    {
      while(getmodelstate.response.pose.position.z > 5)
 	{
 	  tw.linear.z = -0.8;
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
    }

  ros::Duration(2.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

retsrv.request.goal = "";
  if (cam.call(retsrv))
  {
    ROS_INFO_STREAM(retsrv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service store image");
    return 1;
  }

    if(getmodelstate.response.pose.position.z < 10)
    {
      while(getmodelstate.response.pose.position.z < 10)
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
    }
 ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     
 gms_c.call(getmodelstate);


  new_x = -23;
  new_y = 10;
  //twelveth position
  now_x =  getmodelstate.response.pose.position.x;

  if(now_x <= new_x)
    {
      while(now_x <= new_x)
 	{
 	  tw.linear.x = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  else
    {
      while(now_x > new_x)
 	{
 	  tw.linear.x = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  now_y =  getmodelstate.response.pose.position.y;
  
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
 	{
 	  tw.linear.y = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
    {
      while(now_y > new_y)
 	{
 	  tw.linear.y = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);

    if(getmodelstate.response.pose.position.z > 7)
    {
      while(getmodelstate.response.pose.position.z > 7)
 	{
 	  tw.linear.z = -0.8;
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
    }

  ros::Duration(2.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);     

retsrv.request.goal = "";
  if (cam.call(retsrv))
  {
    ROS_INFO_STREAM(retsrv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service store image");
    return 1;
  }

    if(getmodelstate.response.pose.position.z < 10)
    {
      while(getmodelstate.response.pose.position.z < 10)
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
    }
 ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);       
  gms_c.call(getmodelstate);

  new_x = -15;
  new_y = 18.99;
  //thirdtenth position
  now_x =  getmodelstate.response.pose.position.x;

  if(now_x <= new_x)
    {
      while(now_x <= new_x)
 	{
 	  tw.linear.x = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_x =  getmodelstate.response.pose.position.x;
 	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else{
    while(now_x > new_x)
      {
 	tw.linear.x = 0.6;
 	publisher.publish(tw);
 	ros::Duration(1.0).sleep();
 	gms_c.call(getmodelstate);
 	now_x =  getmodelstate.response.pose.position.x;
      }
    
    ros::Duration(1.0).sleep();
    tw.linear.z = 0;
    tw.linear.x = 0;
    tw.linear.y = 0;
    publisher.publish(tw);
  }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  now_y =  getmodelstate.response.pose.position.y;
  
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
 	{
 	  tw.linear.y = -0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
    {
      while(now_y > new_y)
 	{
 	  tw.linear.y = 0.6;
 	  publisher.publish(tw);
 	  ros::Duration(1.0).sleep();
 	  gms_c.call(getmodelstate);
 	  now_y =  getmodelstate.response.pose.position.y;
 	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
gms_c.call(getmodelstate);

 //    //next pose 
  new_x = 0;
  new_y = 21;
  //thirdtenth position
  now_x =  getmodelstate.response.pose.position.x;

  if(now_x <= new_x)
    {
      while(now_x <= new_x)
  	{
  	  tw.linear.x = -0.6;
  	  publisher.publish(tw);
  	  ros::Duration(1.0).sleep();
  	  gms_c.call(getmodelstate);
  	  now_x =  getmodelstate.response.pose.position.x;
  	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else{
    while(now_x > new_x)
      {
  	tw.linear.x = 0.6;
  	publisher.publish(tw);
  	ros::Duration(1.0).sleep();
  	gms_c.call(getmodelstate);
  	now_x =  getmodelstate.response.pose.position.x;
      }
    
    ros::Duration(1.0).sleep();
    tw.linear.z = 0;
    tw.linear.x = 0;
    tw.linear.y = 0;
    publisher.publish(tw);
  }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  now_y =  getmodelstate.response.pose.position.y;
  
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
  	{
  	  tw.linear.y = -0.6;
  	  publisher.publish(tw);
  	  ros::Duration(1.0).sleep();
  	  gms_c.call(getmodelstate);
  	  now_y =  getmodelstate.response.pose.position.y;
  	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
    {
      while(now_y > new_y)
  	{
  	  tw.linear.y = 0.6;
  	  publisher.publish(tw);
  	  ros::Duration(1.0).sleep();
  	  gms_c.call(getmodelstate);
  	  now_y =  getmodelstate.response.pose.position.y;
  	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  // //else
  retsrv.request.goal = "";
  if (cam.call(retsrv))
  {
    ROS_INFO_STREAM(retsrv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service store image");
    return 1;
  }
  //next pose 
  new_x = 2.5;
  new_y = 28.5;
  now_x =  getmodelstate.response.pose.position.x;

  //thirdtenth position
  if(now_x <= new_x)
    {
      while(now_x <= new_x)
  	{
  	  tw.linear.x = -0.6;
  	  publisher.publish(tw);
  	  ros::Duration(1.0).sleep();
  	  gms_c.call(getmodelstate);
  	  now_x =  getmodelstate.response.pose.position.x;
  	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else{
    while(now_x > new_x)
      {
  	tw.linear.x = 0.6;
  	publisher.publish(tw);
  	ros::Duration(1.0).sleep();
  	gms_c.call(getmodelstate);
  	now_x =  getmodelstate.response.pose.position.x;
      }
    
    ros::Duration(1.0).sleep();
    tw.linear.z = 0;
    tw.linear.x = 0;
    tw.linear.y = 0;
    publisher.publish(tw);
  }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  now_y =  getmodelstate.response.pose.position.y;
  
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
  	{
  	  tw.linear.y = -0.6;
  	  publisher.publish(tw);
  	  ros::Duration(1.0).sleep();
  	  gms_c.call(getmodelstate);
  	  now_y =  getmodelstate.response.pose.position.y;
  	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
    {
      while(now_y > new_y)
  	{
  	  tw.linear.y = 0.6;
  	  publisher.publish(tw);
  	  ros::Duration(1.0).sleep();
  	  gms_c.call(getmodelstate);
  	  now_y =  getmodelstate.response.pose.position.y;
  	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);

  retsrv.request.goal = "";
  if (cam.call(retsrv))
  {
    ROS_INFO_STREAM(retsrv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service store image");
    return 1;
  }
  
    if(getmodelstate.response.pose.position.z < 10)
    {
      while(getmodelstate.response.pose.position.z < 10)
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
    }
 ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  gms_c.call(getmodelstate);
//else

  new_x = 11;
  new_y = -2.0;
  now_x =  getmodelstate.response.pose.position.x;

  //thirdtenth position
  if(now_x <= new_x)
    {
      while(now_x <= new_x)
	{
	  tw.linear.x = -0.6;
	  publisher.publish(tw);
	  ros::Duration(1.0).sleep();
	  gms_c.call(getmodelstate);
	  now_x =  getmodelstate.response.pose.position.x;
	}
      
      ros::Duration(2.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else{
    while(now_x > new_x)
      {
	tw.linear.x = 0.6;
	publisher.publish(tw);
	ros::Duration(1.0).sleep();
	gms_c.call(getmodelstate);
	now_x =  getmodelstate.response.pose.position.x;
      }
    
    ros::Duration(1.0).sleep();
    tw.linear.z = 0;
    tw.linear.x = 0;
    tw.linear.y = 0;
    publisher.publish(tw);
  }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  now_y =  getmodelstate.response.pose.position.y;
  
  if(now_y <= new_y)
    {
      while(now_y <= new_y)
	{
	  tw.linear.y = -0.6;
	  publisher.publish(tw);
	  ros::Duration(1.0).sleep();
	  gms_c.call(getmodelstate);
	  now_y =  getmodelstate.response.pose.position.y;
	}
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw);
    }else
    {
      while(now_y > new_y)
	{
	  tw.linear.y = 0.6;
	  publisher.publish(tw);
	  ros::Duration(1.0).sleep();
	  gms_c.call(getmodelstate);
	  now_y =  getmodelstate.response.pose.position.y;
	}
      
      ros::Duration(1.0).sleep();
      tw.linear.z = 0;
      tw.linear.x = 0;
      tw.linear.y = 0;
      publisher.publish(tw); 
    }
  
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);

  now_z =  getmodelstate.response.pose.position.z;
  gms_c.call(getmodelstate);
  //next pose
  
  if(now_z >= 2)
    {
      while(now_z >= 2)
  	{
  	  tw.linear.z = -0.6;
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
    }
  ros::Duration(1.0).sleep();
  tw.linear.z = 0;
  tw.linear.x = 0;
  tw.linear.y = 0;
  publisher.publish(tw);
  
  retsrv.request.goal = "";
  if (cam.call(retsrv))
  {
    ROS_INFO_STREAM(retsrv.response.result);
  }
  else
  {
    ROS_ERROR("Failed to call service store image");
    return 1;
  }

  res.reply = "Task Execution completed";
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "start_scan_region_server");
ros::NodeHandle n;
  ros::ServiceServer service = n.advertiseService("scanRegion", execute);
  ROS_INFO_STREAM("Ready to receive information where to fly");
  ros::spin();

  return 0;
}
