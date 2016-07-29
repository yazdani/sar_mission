#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include "sensor_msgs/Image.h"
#include <cstdlib>
#include "ros/ros.h"
#include "std_msgs/String.h"


using namespace cv;
using namespace std;
  Mat images;
  cv::Mat imgMat;
void callback(const sensor_msgs::Image& image)
{
  std::cout << "Got an imagerrtrt!" << std::endl;
  cv_bridge::CvImagePtr cv_ptr = nullptr;

  std::cout << "Got an imagertrtytytyty!" << std::endl;
  if(sensor_msgs::image_encodings::isColor(image.encoding)) {
  std::cout << "Got ayjtytn image!" << std::endl;
    cv_ptr = cv_bridge::toCvCopy(image, sensor_msgs::image_encodings::RGB8);
  } 
    std::cout << "Gotuiuiouiouio an image!" << std::endl;
  if(cv_ptr) {
    imgMat = cv_ptr->image;
    // images = image; //imread(image, CV_LOAD_IMAGE_COLOR); 
    std::cout << "Got an image!" << std::endl;
  } 
  std::cout << "Got andfdfd image!" << std::endl;
   namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.'
  std::cout << "Got an imaasdsaasge!" << std::endl;
   imshow( "Display window", imgMat );               // Show our image inside it.
  // if(! images.data )                              // Check for invalid input
  //   {
  //     cout <<  "Could not open or find the image" << std::endl ;
  //     return;
  //   }

  //   namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
  //   imshow( "Display window", imgMat );               // Show our image inside it.
 std::cout << "Got an imassssssssssssssssssssgertrtytytyty!" << std::endl;
}


int main( int argc, char** argv )
{
  ros::init(argc, argv, "subscriber_spawn_image");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("/camera/rgb/image_raw", 1000, callback);
 std::cout << "Got an imagffffffffffffffffffffffffertrtytytyty!" << std::endl;
  ros::spin();
  return 0;
}
