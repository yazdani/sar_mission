cmake_minimum_required(VERSION 2.8.3)
project(quadrotor_controller)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  actionlib
  actionlib_msgs
  geometry_msgs
  roscpp
  std_msgs
  message_generation
  img_mission
)
find_package(cmake_modules REQUIRED)
find_package(Eigen3 REQUIRED)


## Generate services in the 'srv' folder
add_service_files(
   FILES
   cmd_srv.srv
   scan_reg.srv
   cmd_points.srv
)


## Generate added messages and services with any dependencies listed here
generate_messages(
 DEPENDENCIES
   actionlib_msgs geometry_msgs std_msgs img_mission
 )

catkin_package( CATKIN_DEPENDS actionlib actionlib_msgs geometry_msgs roscpp std_msgs img_mission)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS})



## Declare a C++ executable
# add_executable(quadrotor_controller_node src/quadrotor_controller_node.cpp)
#add_executable(cmdVel_controller src/cmdVel_controller.cpp)
#add_executable(cmdVel_follower src/cmdVel_follower.cpp)
add_executable(cmd_executer src/cmd_executer.cpp)
add_executable(scan_region src/scan_region.cpp)
add_executable(robot_pose src/robot_pose.cpp)
add_executable(rotate_cam src/rotate_cam.cpp)
add_executable(cmd_points src/cmd_points.cpp)
add_executable(take_off src/take_off.cpp)
add_executable(go_short_distance src/go_short_distance.cpp)
#add_executable(laser_scan_data src/laser_scan_data.py)
## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(quadrotor_controller_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
#add_dependencies(cmdVel_controller ${quadrotor_controller_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
add_dependencies(take_off quadrotor_controller_gencpp)
add_dependencies(cmd_executer quadrotor_controller_gencpp)
add_dependencies(scan_region quadrotor_controller_gencpp)
#add_dependencies(cmd_pose quadrotor_controller_gencpp)
add_dependencies(robot_pose quadrotor_controller_gencpp)
add_dependencies(rotate_cam quadrotor_controller_gencpp)
add_dependencies(cmd_points quadrotor_controller_gencpp)
add_dependencies(go_short_distance quadrotor_controller_gencpp)
#add_dependencies(laser_scan_data quadrotor_controller_gencpp)

## Specify libraries to link a library or executable target against
# target_link_libraries(quadrotor_controller_node
#   ${catkin_LIBRARIES}
# )
#target_link_libraries(cmdVel_controller
#  ${catkin_LIBRARIES})

#target_link_libraries(cmdVel_follower
#  ${catkin_LIBRARIES})
target_link_libraries(take_off ${catkin_LIBRARIES})
target_link_libraries(cmd_executer ${catkin_LIBRARIES})
target_link_libraries(scan_region ${catkin_LIBRARIES})
#target_link_libraries(cmd_pose ${catkin_LIBRARIES})
target_link_libraries(robot_pose ${catkin_LIBRARIES})
target_link_libraries(rotate_cam ${catkin_LIBRARIES})
target_link_libraries(cmd_points ${catkin_LIBRARIES})
target_link_libraries(go_short_distance ${catkin_LIBRARIES})
