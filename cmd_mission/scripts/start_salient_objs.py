#!/usr/bin/env python

from cmd_mission.srv import *
import rospy
import tf
import roslib

listener=''

def get_gesture_relative_to_genius():
    global listener
    br = tf.TransformBroadcaster()
    rate = rospy.Rate(10.0)
    now = rospy.Time.now()
    var = 1
    listener.waitForTransform("/map", "/gesture", now, rospy.Duration(4.0))
    (trans,rot) = listener.lookupTransform('/map', '/gesture', now)
    print trans
    print "listener"

def get_salient_objs(req):
    get_gesture_relative_to_genius()
    # (trans,rot) = listener.lookupTransform('/map', '/human', rospy.Time(0))
    # print trans
    # print rot
    # print trans[0]
    # print "und nun"
    # print trans[1]
    return salient_objsResponse("test")

def start_salient_objs():
    global listener
    rospy.init_node('start_salient_objs')
    listener = tf.TransformListener()
    s = rospy.Service('salient_objs', salient_objs, get_salient_objs)
    print "Waiting for client in order to give list of salient objs."
    rospy.spin()

if __name__ == "__main__":
    start_salient_objs()
