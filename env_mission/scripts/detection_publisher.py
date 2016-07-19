#!/usr/bin/env python

import rospy
import os, sys
import os.path
from std_msgs.msg import String
from geometry_msgs.msg import Pose
from geometry_msgs.msg import Point
from geometry_msgs.msg import Quaternion
from geometry_msgs.msg import Vector3
import string
from env_mission.msg import detect_obj

if __name__ == '__main__':
    pub = rospy.Publisher('detect_obj', detect_obj, queue_size=10)
    rospy.init_node('dectection_publisher')
    tmp = detect_obj()
    pose = Pose()
    point = Point()
    point.x = 2.2
    point.y = 3.2
    point.z = 4.2
    quat = Quaternion()
    quat.x = 1.23
    quat.y = 24.56
    quat.z = 4.5
    quat.w = 1.0
    vec = Vector3()
    vec.x = 1.0
    vec.y = 2.0
    vec.z = 3.0
    pose.position = point
    pose.orientation = quat
    rate = rospy.Rate(0.5)

    while not rospy.is_shutdown():
        tmp.mission = 'start'
        tmp.objname = 'tree'
        tmp.objid = '2'
        tmp.objpose = pose 
        tmp.objbbox =  vec
        pub.publish(tmp)
        rate.sleep()
        tmp.mission = 'start'
        tmp.objname = 'tree'
        tmp.objid = '2'
        tmp.objpose = pose 
        tmp.objbbox =  vec
        pub.publish(tmp)
        rate.sleep()
        tmp.mission = 'end'
        tmp.objname = 'tree'
        tmp.objid = '2'
        tmp.objpose = pose 
        tmp.objbbox =  vec
        pub.publish(tmp)
        rate.sleep()
        

