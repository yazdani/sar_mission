#!/usr/bin/env python

import rospy
import os, sys
import os.path
import string
path = "../owl"
path2 = "../map"
outFile = "sem_map.owl"
outFile2 = "sem_map.yaml"

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
    o.write("<?xml version=\"1.0\"?> \n\n<!DOCTYPE rdf:RDF [\n")
    o.write("  <!ENTITY owl \"http://www.w3.org/2002/07/owl#\" > \n")
    o.write("  <!ENTITY xsd \"http://www.w3.org/2001/XMLSchema#\" > \n")
    o.write("  <!ENTITY knowrob \"http://knowrob.org/kb/knowrob.owl#\" > \n")
    o.write("  <!ENTITY rdfs \"http://www.w3.org/2000/01/rdf-schema#\" > \n")
    o.write("  <!ENTITY rdf \"http://www.w3.org/1999/02/22-rdf-syntax-ns#\" > \n")
    o.write("]>\n\n")
    o.write("<rdf:RDF xmlns=\"http://knowrob.org/kb/IAI-alpine.owl#\"\n")
    o.write("  xml:base=\"http://knowrob.org/kb/IAI-alpine.owl\"\n")
    o.write("  xmlns:knowrob=\"http://knowrob.org/kb/knowrob.owl#\"\n")
    o.write("  xmlns:rdfs=\"http://www.w3.org/2000/01/rdf-schema#\"\n")
    o.write("  xmlns:xsd=\"http://www.w3.org/2001/XMLSchema#\"\n")
    o.write("  xmlns:owl=\"http://www.w3.org/2002/07/owl#\"\n")
    o.write("  xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\">\n\n")
    o.write("\n")

    o.write("  <owl:Ontology rdf:about=\"http://knowrob.org/kb/IAI-alpine.owl\">\n")
    o.write("    <owl:imports rdf:resource=\"package://env_mission/owl/mountain_ontology.owl\"/>\n")
    o.write("  </owl:Ontology>\n\n")
    index = 0
    while index < len(lines):
        tmp = lines[index]
        if tmp == 'object\n':
            o.write("<!-- "+lines[index+4].split("\n")[0]+"-->\n")
            o.write("<owl:NamedIndividual rdf:about=\"&knowrob;"+lines[index+4].split("\n")[0]+"\">\n")
            o.write("     <rdf:type rdf:resource=\"http://knowrob.org/kb/knowrob.owl#"+lines[index+6].split("\n")[0]+"\"/>\n")
            o.write("     <knowrob:describedInMap rdf:resource=\"http://knowrob.org/kb/ias_semantic_map.owl#MountainMap\"/>\n")
            if lines[index+6] == "tree\n":
                o.write("     <knowrob:hasColor rdf:datatype=\"&xsd;string\">"+"darkgreen"+"</knowrob:hasColor>\n")
                o.write("     <knowrob:isAlive rdf:datatype=\"&xsd;string\">inanimate</knowrob:isAlive>\n")
            elif lines[index+6] == "victim\n":
                o.write("     <knowrob:hasColor rdf:datatype=\"&xsd;string\">"+"red"+"</knowrob:hasColor>\n")
                o.write("     <knowrob:isAlive rdf:datatype=\"&xsd;string\">animate</knowrob:isAlive>\n")
            elif lines[index+6] == "pylon\n":
                o.write("     <knowrob:hasColor rdf:datatype=\"&xsd;string\">"+"gray"+"</knowrob:hasColor>\n")
                o.write("     <knowrob:isAlive rdf:datatype=\"&xsd;string\">inanimate</knowrob:isAlive>\n")
            elif lines[index+6] == "rock\n":
                o.write("     <knowrob:hasColor rdf:datatype=\"&xsd;string\">"+"black"+"</knowrob:hasColor>\n")
                o.write("     <knowrob:isAlive rdf:datatype=\"&xsd;string\">inanimate</knowrob:isAlive>\n")
            o.write("</owl:NamedIndividual>\n\n")
            o.write("<owl:NamedIndividual rdf:about=\"&knowrob;Transformation_"+lines[index+4].split("\n")[0]+"\">\n")
            o.write("     <rdf:type rdf:resource=\"&knowrob;Transformation\"/>\n")
            pose_one = lines[index+8]
            ax,ay,az = pose_one.split(",")
            o.write("     <knowrob:quaternion rdf:datatype=\"&xsd;string\">"+ax+" "+ay+" "+az.split("\n")[0]+"</knowrob:quaternion>\n")
            pose_two = lines[index+9]
            qx,qy,qz,qw = pose_two.split(",")
            o.write("     <knowrob:translation rdf:datatype=\"&xsd;string\">"+qx+" "+qy+" "+qz+" "+qw.split("\n")[0]+"</knowrob:translation>\n")        
            o.write("</owl:NamedIndividual>\n\n")
            o.write("<owl:NamedIndividual rdf:about=\"&knowrob;SemanticMapPerception"+lines[index+4].split("\n")[0]+"\">\n")
            o.write("     <rdf:type rdf:resource=\"&knowrob;SemanticMapPerception\"/>\n")
            o.write("     <knowrob:startTime rdf:resource=\"http://knowrob.org/kb/ias_semantic_map.owl#timepoint_0\"/>\n")
            o.write("     <knowrob:eventOccursAt rdf:resource=\"&knowrob;Transformation_"+lines[index+4].split("\n")[0]+"\"/>\n")
            o.write("     <knowrob:objectActedOn rdf:resource=\"&knowrob;"+lines[index+4].split("\n")[0]+"\"/>\n")
            o.write("</owl:NamedIndividual>\n\n")
        index = index + 1
    o.write("</rdf:RDF>")
        


