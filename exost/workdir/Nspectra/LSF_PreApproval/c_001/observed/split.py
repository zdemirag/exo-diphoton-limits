#!/usr/bin/python

import sys,string,time,os,glob,re
from array import array

fout = open("final","w")
flimit = open("limits","r")
fmass = open("mass","r")

mass = array("i",[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0])

i = 0
for line in fmass:
    #print line.strip()
    mass[i] = int(line.strip())
    i=i+1

#print mass[0]

j=0
flimit = open("limits","r")
for line2 in flimit:
    print mass[int(j/2)] , line2.strip()

    #fout.write(line.strip() + " " + line2.strip()+"\n")
    fout.write(str(mass[int(j/2)])+ " " + line2.strip()+"\n")
    j=j+1
