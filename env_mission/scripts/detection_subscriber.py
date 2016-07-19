#!/usr/bin/env python

import rospy
import os, sys
import os.path
from std_msgs.msg import String
from geometry_msgs.msg import Pose
from geometry_msgs.msg import Vector3
from env_mission.msg import detect_obj
path = "../map"
outFile = "sem_map.yaml"

def cbDetector(msg):
    name = msg.objname
    pose = msg.objpose
    bbox = msg.objbbox
    objid = msg.objid
    mission = msg.mission
    pos = str(pose.position.x)+","+str(pose.position.y)+","+str(pose.position.z)
    ori = str(pose.orientation.x)+","+str(pose.orientation.y)+","+str(pose.orientation.z)+","+str(pose.orientation.w)
    vec = str(bbox.x)+","+str(bbox.y)+","+str(bbox.z)
    # name = "tree"
    # pose = "15"
    # bbox = "2 3 4"
    # id = "2"
    #wenn mission == start, dann ..
    if mission == 'start':
        with open(path+"/"+outFile,'a') as o:
            o.write('object')
            o.write('\n')
            o.write('id:\n')
            o.write(objid)
            o.write('\n')
            o.write('name:\n')
            o.write(name+objid)
            o.write('\n')
            o.write('type:\n')
            o.write(name)
            o.write('\n')
            o.write('pose:\n')
            o.write(pos)
            o.write('\n')
            o.write(ori)
            o.write('\n')
            o.write('bbox:\n')
            o.write(vec)
            o.write('\n')
            o.write('\n')
            

if __name__ == '__main__':
    if os.path.exists(path):
        print "directory already exist"
    else:
        os.mkdir(path, 0755 );
    with open(path+"/"+outFile,'w') as o:
        o.write("")
    rospy.init_node('detect_node')
    rate = rospy.Rate(10)
    rospy.Subscriber("detect_obj", detect_obj, cbDetector)
    rospy.spin()
