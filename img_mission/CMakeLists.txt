cmake_minimum_required(VERSION 2.8.3)
project(img_mission)
## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  geometry_msgs
  roscpp
  rospy
  std_msgs
  sensor_msgs
  message_generation
  roscpp
  image_transport
)

find_package(OpenCV REQUIRED)
 add_message_files(
   FILES
   Strings.msg
)

## Generate services in the 'srv' folder
add_service_files(
   FILES
   returnString.srv
)

generate_messages(
    DEPENDENCIES
    geometry_msgs std_msgs
)
catkin_package(
 #INCLUDE_DIRS include
 CATKIN_DEPENDS cv_bridge std_msgs 
 DEPENDS OpenCV
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  ${catkin_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)


add_executable(show_image src/show_image.cpp)
target_link_libraries(show_image ${catkin_LIBRARIES})
add_dependencies(show_image img_mission_generate_messages_cpp)
add_executable(publishing_storing src/publishing_storing.cpp)
target_link_libraries(publishing_storing ${catkin_LIBRARIES})
add_dependencies(publishing_storing img_mission_generate_messages_cpp)
add_executable(pub_img src/pub_img.cpp)
target_link_libraries(pub_img  ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_dependencies(pub_img img_mission_generate_messages_cpp)
add_executable(subscribing_storing src/subscribing_storing.cpp)
target_link_libraries(subscribing_storing ${catkin_LIBRARIES})
add_dependencies(subscribing_storing img_mission_generate_messages_cpp)
