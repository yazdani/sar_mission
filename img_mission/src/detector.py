#!/usr/bin/env python

import numpy as np
import argparse
import cv2
import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import random
from img_mission.srv import *

# Instantiate CvBridge
bridge = CvBridge()
value = 0
def image_callback(msg):
    global value
    print("Received an image!")
    try:
        cv2_img = bridge.imgmsg_to_cv2(msg, "bgr8")
        hsv = cv2.cvtColor(cv2_img, cv2.COLOR_BGR2HSV)
        lower_red = np.array([0,10,10], np.uint8)
        upper_red = np.array([50,255,255], np.uint8)
        # mask = cv2.inRange(hsv, lower_red, upper_red)
        # res = cv2.bitwise_and(cv2_img,cv2_img, mask= mask)
        dest = cv2.inRange(hsv, lower_red, upper_red)
        count_white = 0	
        count_black = 0
        red = cv2.countNonZero(dest)
        if red > 150 and value == 0:
            rad = random.randint(0, 10)
            value = 1
            tmp = "victim_"+str(rad)+".jpeg"
            rospy.wait_for_service('store_image')
            try:
                store_image = rospy.ServiceProxy('store_image', returnString)
                resp1 = store_image(tmp)
                return resp1.result
            except rospy.ServiceException, e:
                print "Service call failed: %s"%e
            #take picture
        elif red < 50 and value == 1:
            value = 0
    except CvBridgeError, e:
        print(e) 

    #cv2.imwrite('camera_image.jpeg', mask)
    #cv2.imwrite('camera_image_start.jpeg', cv2_img)
            
def listener():
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber('camera/rgb/image_raw', Image, image_callback)
    rospy.spin()

if __name__ == '__main__':
    listener()




# while not cap.isOpened():
#     print""
# while(1):    
#     # Take each frame
#     _, frame = cap.read()
    
#     # Convert BGR to HSV
#     hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
#     # define range of blue color in HSV
#     lower_blue = np.array([110,50,50])
#     upper_blue = np.array([130,255,255])
    
#     # Threshold the HSV image to get only blue colors
#     mask = cv2.inRange(hsv, lower_blue, upper_blue)
    
#     # Bitwise-AND mask and original image
#     res = cv2.bitwise_and(frame,frame, mask= mask)
    
#     cv2.imshow('frame',frame)
#     cv2.imshow('mask',mask)
#     cv2.imshow('res',res)
#     k = cv2.waitKey(5) & 0xFF
#     if k == 27:
#         break
            
#     cv2.destroyAllWindows()

     # #  define range of blue color in HSV
        # lower_blue = np.array([110,10,10])
        # upper_blue = np.array([130,255,255])

      #   #  green
      # #  green = np.uint8([[[0,255,0 ]]])
      #   lower_green = np.array([0,100,100], np.uint8)
      #   upper_green = np.array([20,255,255], np.uint8)
     #   hsv_green = cv2.cvtColor(green,cv2.COLOR_BGR2HSV)
     #   print hsv_green
        # Threshold the HSV image to get only blue colors
      
