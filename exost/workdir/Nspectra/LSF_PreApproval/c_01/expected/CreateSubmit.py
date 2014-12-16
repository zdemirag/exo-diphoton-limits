#!/usr/bin/python
#

import sys,string,time,os ,glob,re


from optparse import OptionParser
parser = OptionParser()
parser.add_option("-m", "--mass", dest = "mass", type="int")
parser.add_option("-t", "--type", dest = "type", type="string")
(options, args) = parser.parse_args()

mass = options.mass
type = options.type

### parameters ###

oFile1 = open('submit.sh','w')
oFile2 = open('kill.sh','w')

for i in range(0,1000000):

    #print glob.glob('lxplus*')
    #print "bsub -q 1nd -J LSF_*"+type+"_"+str(i)+" < lxplusBatch_"+str(mass)+"_"+type+"_"+str(i)+".csh\n"
    #print i
    name = glob.glob('lxplus*')[i]
    name2 = re.sub('.csh$', '', name)
    print "bsub -q 1nd -J "+ name2 + " < " + name #+ "\n"
    #print glob.glob('lxplus*')[i] - ".csh" + "<" + glob.glob('lxplus*')[i] + "\n"
    oFile1.write("bsub -q 1nd -J "+ name2 + " < " + name + "\n")
    #oFile1.write("bsub -q 1nd -J LSF_"+str(mass)+"_"+type+"_"+str(i)+" < lxplusBatch_"+str(mass)+"_"+type+"_"+str(i)+".csh\n")
    oFile2.write("bkill -J "+  name2+"\n")
