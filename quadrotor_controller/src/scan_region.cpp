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




bool execute(quadrotor_controller::scan_reg::Request &req,
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
 double new_x;
 double new_y;
 double new_z;

  ROS_INFO("IS EXECUTED");
  publisher = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
   gms_c = nh_.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
   getmodelstate.request.model_name="quadrotor";
    
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
    
    ros::Rate r(1);
    bool success = true;
 publisher.publish(tw);
    ros::Duration(2.0).sleep();
       
       
    ROS_INFO(" Come Up Hector! ");

    new_x = 14;
    new_y = 10;
    new_z = 10;

 if(now_z <= new_z)
      {
	while(now_z <= new_z)
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
 // First position
 if(now_x <= new_x)
      {
	while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
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

  // Second Position
    new_x = 28;
    new_y = 1;
if(now_x <= new_x)
      {
	while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
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

  // Third Position
    new_x = 23;
    new_y = -8;
if(now_x <= new_x)
      {
	while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
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

  //Fourth Position
  new_x = 5;
  new_y = -11;
  
  if(now_x <= new_x)
      {
	while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
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

  // Fifth
  new_x = -7;
  new_y = -10;

if(now_x <= new_x)
      {
	while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
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
  

   // Sixth
  new_x = -5.5;
  new_y = -24;
 
if(now_x <= new_x)
      {
	while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
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
	

// Seventh Position
    new_x = -15;
    new_y = -14;
if(now_x <= new_x)
      {
	while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
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

// Eightth Position
    new_x = -16;
    new_y = 0;
if(now_x <= new_x)
      {
	while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
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

// Nineth Position
	new_x = -36;
    new_y = -1;
if(now_x <= new_x)
      {
	while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
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

// Tenth Position
    new_x = -23;
    new_y = 10;
if(now_x <= new_x)
      {
	while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw); 
      }
  ROS_INFO(" Move down! ");

	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

// Eleventh Position
    new_x = -13;
    new_y = 17;
if(now_x <= new_x)
      {
	while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
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

// Twelveth Position
    new_x = -2;
    new_y = 19;
if(now_x <= new_x)
      {
	while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
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

// Thirdtenth Position
    new_x = 0.5;
    new_y = 26.5;
if(now_x <= new_x)
      {
	while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
	  ROS_INFO(" Perfect! ");
	    ROS_INFO_STREAM(now_x);
	    ROS_INFO_STREAM(new_x);
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
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

// Fourtenth Position
    new_x = -5;
    new_y = 1;
if(now_x <= new_x)
      {
	while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
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


  // Fifthtenth Position
  new_x = 13;
  new_y = 0;
  new_z = 2.5;

if(now_x <= new_x)
      {
	  while(now_x <= new_x)
	  {
	    tw.linear.x = 0.7;
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
	else if(now_x > new_x)
	  {
	    while(now_x > new_x)
	      {
		tw.linear.x = -0.7;
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
 
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);

    if(now_y <= new_y)
      {
	while(now_y <= new_y)
	  {
	    tw.linear.y = 0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
	tw.linear.z = 0;
	tw.linear.x = 0;
	tw.linear.y = 0;
	publisher.publish(tw);
      }else if(now_y > new_y)
      {
	while(now_y > new_y)
	  {
	    tw.linear.y = -0.7;
	    publisher.publish(tw);
	    ros::Duration(1.0).sleep();
	    gms_c.call(getmodelstate);
	    now_y =  getmodelstate.response.pose.position.y;
	  }
	ros::Duration(2.0).sleep();
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


 if(now_z <= new_z)
      {
	while(now_z <= new_z)
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

	while(now_z > new_z)
	  {
	    tw.linear.z = -0.5;
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


  res.reply = "Task Execution completed";
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "start_scan_region_server");
ros::NodeHandle n;
  ros::ServiceServer service = n.advertiseService("scanRegion", execute);
  ROS_INFO("Ready to receive information where to fly");
  ros::spin();

  return 0;
}
