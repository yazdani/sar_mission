#!/usr/bin/env python

import rospy
import os, sys
import os.path
import string
import datetime
import math


path = "../map"
path2 = "../owl"
# outFile = "osm_map.osm"
# outFile2 = "osm_sem_map.owl"
outFile = sys.argv[1]
outFile2 = sys.argv[2]

alt_home=1580.0
lat_home=44.153278
lon_home=12.241426
lat_home_rad= lat_home*math.pi/180.0
lon_home_rad= lon_home*math.pi/180.0
ma_semi= 6378137;    
mi_semi= 6356752.3124;
euler = math.sqrt(1-pow(mi_semi,2)/pow(ma_semi,2));

if os.path.exists(path):
    print "directory already exist"
else:
    os.mkdir( path, 0755 )

if os.path.exists(path2):
    print "directory already exist"
else:
    os.mkdir( path2, 0755 )

with open(path2+"/"+outFile2,'w') as s:
    s.write("")

with open(path+"/"+outFile,'r') as i:
    lines = i.readlines()

with open(path2+"/"+outFile2,'a') as o:
    o.write("<?xml version=\"1.0\"?>\n\n")
    o.write("<!DOCTYPE rdf:RDF [\n")
    o.write("<!ENTITY owl \"http://www.w3.org/2002/07/owl#\" >\n")
    o.write("<!ENTITY xsd \"http://www.w3.org/2001/XMLSchema#\" >\n")
    o.write("<!ENTITY knowrob \"http://knowrob.org/kb/knowrob.owl#\" >\n") 
    o.write("<!ENTITY rdfs \"http://www.w3.org/2000/01/rdf-schema#\" >\n") 
    o.write("<!ENTITY rdf \"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" >\n") 
    o.write("]>\n\n")
    o.write("<rdf:RDF xmlns=\"http://knowrob.org/kb/IAI-alpine.owl#\"\n")
    o.write("xml:base=\"http://knowrob.org/kb/IAI-alpine.owl\"\n")
    o.write("xmlns:knowrob=\"http://knowrob.org/kb/knowrob.owl#\"\n")
    o.write("xmlns:rdfs=\"http://www.w3.org/2000/01/rdf-schema#\"\n")
    o.write("xmlns:xsd=\"http://www.w3.org/2001/XMLSchema#\"\n")
    o.write("xmlns:owl=\"http://www.w3.org/2002/07/owl#\"\n")
    o.write("xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">\n\n")
    o.write("<owl:Ontology rdf:about=\"http://knowrob.org/kb/IAI-alpine.owl\">\n")
    o.write("<owl:imports rdf:resource=\"package://env_mission/owl/mountain_ontology.owl\"/>\n")
    o.write("</owl:Ontology>\n\n")

    index = 0
    while index < len(lines):
        if "<node id=\"" in lines[index] and "<tag k=\"" in lines[index+1]:
            ids = str(int(lines[index].split("id=\"")[1].split("\"")[0]) % 1000)
            lat = float(lines[index].split("lat=\"")[1].split("\"")[0])
            lon = float(lines[index].split("lon=\"")[1].split("\"")[0])
            types = lines[index+1].split("v=\"")[1].split("\"")[0]
            lat_rad = lat*math.pi/180.0
            lon_rad = lon*math.pi/180.0
            radius = ma_semi/math.sqrt(1-pow(euler,2)*pow(math.sin(lat_home_rad),2))
            x = (lat_rad-lat_home_rad)*radius
            y = (lon_rad-lon_home_rad)*radius*math.cos(lat_home_rad)
            z = alt_home - alt_home
            o.write("<!--"+types+ids+"-->")
            o.write("\n")
            o.write("\n")
            o.write("<owl:NamedIndividual rdf:about=\"&knowrob;"+types+ids+"\">\n")
            o.write("<rdf:type rdf:resource=\"http://knowrob.org/kb/knowrob.owl#"+types+"\"/>\n")
            o.write("<knowrob:describedInMap rdf:resource=\"http://knowrob.org/kb/ias_semantic_map.owl#MountainMap\"/>\n")
            if types == "victim":
                o.write("<knowrob:hasColor rdf:datatype=\"&xsd;string\">"+"red"+"</knowrob:hasColor>\n")
                o.write("<knowrob:isAlive rdf:datatype=\"&xsd;string\">"+"animate"+"</knowrob:isAlive>\n")
            else:
                o.write("<knowrob:hasColor rdf:datatype=\"&xsd;string\">"+"darkgreen"+"</knowrob:hasColor>\n")
                o.write("<knowrob:isAlive rdf:datatype=\"&xsd;string\">"+"inanimate"+"</knowrob:isAlive>\n")
            o.write("</owl:NamedIndividual>\n")
            o.write("\n")
            o.write("<owl:NamedIndividual rdf:about=\"&knowrob;Transformation_"+types+ids+"\">\n")
            o.write("<rdf:type rdf:resource=\"&knowrob;Transformation\"/>\n")
            o.write("<knowrob:quaternion rdf:datatype=\"&xsd;string\">"+"1.0 0.0 0.0 0.0"+"</knowrob:quaternion>\n")
            o.write("<knowrob:translation rdf:datatype=\"&xsd;string\">"+str(round(x,2))+" "+str(round(y,2))+" "+str(z)+"</knowrob:translation>\n")
            o.write("</owl:NamedIndividual>\n")
            o.write("\n")
            o.write("<owl:NamedIndividual rdf:about=\"&knowrob;SemanticMapPerception"+types+ids+"\">\n")
            o.write("<rdf:type rdf:resource=\"&knowrob;SemanticMapPerception\"/>\n")
            o.write("<knowrob:startTime rdf:resource=\"http://knowrob.org/kb/ias_semantic_map.owl#timepoint_0\"/>\n")
            o.write("<knowrob:eventOccursAt rdf:resource=\"&knowrob;Transformation_"+types+ids+"\"/>\n")
            o.write("<knowrob:objectActedOn rdf:resource=\"&knowrob;"+types+ids+"\"/>\n")
            o.write("</owl:NamedIndividual>\n")
            o.write("\n")
            o.write("\n")
        index = index + 1
    o.write("</rdf:RDF>")
        
