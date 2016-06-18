#!/usr/bin/env python  
import roslib
import rospy
import tf
import math
from cmd_mission.srv import *


def create_frame(x,y,z):
    rospy.wait_for_service('check_objs_relation')
    print "hier2"
    try:
        print "into client"
        check_objs_relations = rospy.ServiceProxy('check_objs_relation', check_objs_relation)
        print "into client3"
        resp1 = check_objs_relations(x,y,z)
        print "into client4"
        print resp1.result_check
        return resp1.result_check
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e
    
if __name__ == '__main__':
    create_frame("behind", "tree01", "tree02")
