#!/usr/bin/env python

import rospy
import os, sys
import os.path
import string
import datetime
import math


path = "../osm"
path2 = "../owl"
#outFile = "osm_map.osm"
#outFile2 = "sem_map.owl"
outFile = sys.argv[2]
outFile2 = sys.argv[1]

now = datetime.datetime.now()

if os.path.exists(path):
    print "directory already exist"
else:
    os.mkdir( path, 0755 )

if os.path.exists(path2):
    print "directory already exist"
else:
    os.mkdir( path2, 0755 )

with open(path+"/"+outFile,'w') as s:
    s.write("")

with open(path2+"/"+outFile2,'r') as i:
    lines = i.readlines()

with open(path+"/"+outFile,'a') as o:
    o.write("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n")
    o.write("<osm version=\"0.6\" generator=\"OpenStreetMap server\">\n")
    o.write(" <bounds minlat=\"0.00\" minlon=\"0.00\" maxlat=\"0.00\" maxlon=\"0.0\"/>\n")
    index = 0
    jindex = 1
    while index < len(lines):
        if "<owl:NamedIndividual rdf:about=\"&knowrob;" in lines[index] and "<rdf:type rdf:resource=\"http://knowrob.org/kb/knowrob.owl#" in lines[index+1] and "<knowrob:describedInMap" in lines[index+2]:
            if  "<knowrob:quaternion" in lines[index+9]:
                x = lines[index+10].split(">")[1].split("<")[0].split(" ")[0]
                y = lines[index+10].split(">")[1].split("<")[0].split(" ")[1]
                z = lines[index+10].split(">")[1].split("<")[0].split(" ")[2]
                euler = math.sqrt(1-pow(6356752.3124,2)/pow(6378137,2))
                lat_home_rad = 44.153278*math.pi/180.0
                lon_home_rad = 12.241426*math.pi/180.0
                radius = 6378137/math.sqrt(1-pow(euler,2)*pow(math.sin(lat_home_rad),2))
                lat = float(x)/radius*180.0/math.pi+44.153278
                lon = float(y)/radius*180.0/math.pi/math.cos(lat_home_rad)+12.241426
                o.write(" <node id=\""+str(jindex+111111000)+"\" lat=\""+str(lat)+"\" lon=\""+str(lon)+"\" version=\"1\" changeset=\""+str(2000+index)+"\" user=\"me\" uid=\""+str(1000+index)+"\" visible=\"true\" timestamp=\""+now.isoformat()+"\">\n")

        
            if lines[index+1].split("#")[1].split("\"")[0]:
                if "victim" == lines[index+1].split("#")[1].split("\"")[0]:
                    o.write("    <tag k=\"person\" v=\""+ lines[index+1].split("#")[1].split("\"")[0]+"\"/>\n")
                else:
                    o.write("    <tag k=\"natural\" v=\""+ lines[index+1].split("#")[1].split("\"")[0]+"\"/>\n")
            elif lines[index+1].split(";")[1].split("\"")[0]:
                if "victim" == lines[index+1].split(";")[1].split("\"")[0]:
                    o.write("    <tag k=\"person\" v=\""+ lines[index+1].split(";")[1].split("\"")[0]+"\"/>\n")
                else:
                    o.write("    <tag k=\"natural\" v=\""+ lines[index+1].split(";")[1].split("\"")[0]+"\"/>\n")
            jindex = jindex + 1
            o.write(" </node>\n")
            o.write("\n")
            
        index = index +1
     
    o.write("</osm>")
        
