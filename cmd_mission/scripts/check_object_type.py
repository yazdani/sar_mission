#!/usr/bin/env python  
import roslib
import rospy
import tf
import math
from cmd_mission.srv import *


def get_into_client(value):
    rospy.wait_for_service('check_objs_relation')
    try:
        check_objs_relations = rospy.ServiceProxy('check_objs_relation', check_objs_relation)
        prop = value.property
        fobj = value.obj1
        sobj = value.obj2
        resp1 = check_objs_relations(prop, fobj, sobj)
        print resp1.result_check
        return resp1.result_check
    except rospy.ServiceException, e:
        print "Service call failed: %s"%e

def get_salient_objs(req):
    x = get_into_client(req)
    return internal_relationResponse(x)
    
def check_object_type():
    rospy.init_node('start_checking_objs')
    s = rospy.Service('check_object_type',  internal_relation, get_salient_objs)
    rospy.spin()

if __name__ == '__main__':
    check_object_type()
