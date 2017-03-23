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



## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
add_service_files(
   FILES
   cmd_srv.srv
   scan_reg.srv
   cmd_points.srv
)

## Generate actions in the 'action' folder
# add_action_files(
#   DIRECTORY action
#   FILES
#   cmdVel.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(
 DEPENDENCIES
   actionlib_msgs geometry_msgs std_msgs img_mission
 )

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package( CATKIN_DEPENDS actionlib actionlib_msgs geometry_msgs roscpp std_msgs img_mission)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS})


## Declare a C++ library
# add_library(quadrotor_controller
#   src/${PROJECT_NAME}/quadrotor_controller.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(quadrotor_controller ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

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
#target_link_libraries(laser_scan_data ${catkin_LIBRARIES})
#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS quadrotor_controller quadrotor_controller_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_quadrotor_controller.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
