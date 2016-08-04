#!/usr/bin/env python  

import numpy as np
import cv2
import rospy
from img_mission.srv import *
import rospkg


def displayImage(req):
    rospack = rospkg.RosPack()
    test = rospack.get_path('img_mission')
    tmp = test+'/imgs/'+req.goal
    img = ""
    print img
    print tmp
    print 'imread'
    img = cv2.imread(tmp,1)
    print 'startWindow'
#    cv2.startWindowThread()
    print 'namedWindow'
    print req.goal
    #print img.height+"*"+img.width
   # cv2.namedWindow(req.goal,0)
    print 'test'
  #  cv2.resizeWindow('image',300,300)
   # cv2.imshow(req.goal,img)
    cv2.waitKey(5000)
    print 'done'
    #cv2.waitKey();
  #  cv2.destroyWindow(req.goal)
 #   res.result = "Spawn Image!"
    return returnStringResponse("Spawned Image!")
    

def show_image():
    rospy.init_node('display_image')
    s = rospy.Service('show_image', returnString, displayImage)

    rospy.spin()

if __name__ == "__main__":
    show_image()
