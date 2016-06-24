#include "ros/ros.h"
#include "quadrotor_controller/cmd_srv.h"
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




bool execute(quadrotor_controller::cmd_srv::Request &req,
         quadrotor_controller::cmd_srv::Response &res)
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
  ROS_INFO("IS EXECUTED");
  publisher = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 1);
   gms_c = nh_.serviceClient<gazebo_msgs::GetModelState>("/gazebo/get_model_state");
   getmodelstate.request.model_name="quadrotor";
    
    geometry_msgs::Twist tw;
    publisher.publish(tw);
    ros::Duration(5.0).sleep();
    
    gms_c.call(getmodelstate);
    double now_x =  getmodelstate.response.pose.position.x;
    double now_y =  getmodelstate.response.pose.position.y;
    double now_z =  getmodelstate.response.pose.position.z;
    
    double new_x = req.goal.position.x;
    double new_y = req.goal.position.y;
    double new_z = req.goal.position.z;

    ros::Rate r(1);
    bool success = true;
 publisher.publish(tw);
    ros::Duration(2.0).sleep();
       
       
    ROS_INFO(" Come Up Hector! ");

 if(now_z < new_z)
      {
	while(now_z < new_z)
	  {
	    tw.linear.z = 0.6;
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
      }// else if(now_z > new_z)
      // {
      // 	while(now_z > new_z)
      // 	  {
      // 	    std::cout << "hahaww" << std::endl;
      // 	    tw.linear.z = -0.2;
      // 	    publisher.publish(tw);
      // 	    ros::Duration(1.0).sleep();
      // 	    gms_c.call(getmodelstate);
      // 	    now_z =  getmodelstate.response.pose.position.z;
      // 	  }
	
      // 	ros::Duration(1.0).sleep();
      // 	tw.linear.z = 0;
      // 	tw.linear.x = 0;
      // 	tw.linear.y = 0;
      // 	publisher.publish(tw);
      // }

 if(now_x < new_x)
      {
	  ROS_INFO(" Move Hector! ");
	while(now_x < new_x)
	  {
	    tw.linear.x = 0.2;
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
		tw.linear.x = -0.1;
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

    if(now_y < new_y)
      {
	while(now_y < new_y)
	  {
	    tw.linear.y = 0.2;
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
	    tw.linear.y = -0.2;
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
  ROS_INFO(" Move down! ");


  std::cout << "Well it worked" << std::endl;
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "execute_instruction_server");
ros::NodeHandle n;
  ros::ServiceServer service = n.advertiseService("setPoseToVel", execute);
  ROS_INFO("Ready to receive information where to fly");
  ros::spin();

  return 0;
}
