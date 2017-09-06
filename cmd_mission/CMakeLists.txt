cmake_minimum_required(VERSION 2.8.3)
project(cmd_mission)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  cram_designators
  geometry_msgs
  message_generation
  message_runtime
  roscpp
  rospy
  std_msgs
  tf
)

add_message_files(
   FILES
   Subgoal.msg
)

## Generate services in the 'srv' folder
add_service_files(
   FILES
   all_objs.srv
   salient_objs.srv
   check_objs_relation.srv
   check_obj_property.srv
   get_obj_type.srv
   get_property_list.srv
   cmd_parser.srv
   vector_transform.srv
   internal_relation.srv
   get_obj_size.srv
   get_reason_pose.srv
   send_instruction.srv
   rotate.srv
 )


## Generate added messages and services with any dependencies listed here
generate_messages(
   DEPENDENCIES
   geometry_msgs std_msgs 
 )

catkin_package( CATKIN_DEPENDS geometry_msgs roscpp std_msgs)

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
)

