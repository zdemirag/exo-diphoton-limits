#!/usr/bin/python

import sys,string,time,os


from optparse import OptionParser
parser = OptionParser()
parser.add_option("-m", "--mass", dest = "mass", type="int")
parser.add_option("-t", "--type", dest = "type", type="string")
(options, args) = parser.parse_args()

mass = options.mass
type = options.type

print "./LimitMoriond2013 0 1 0 " + str(mass) + " 50 100000 c_005_DiphotonLimits_"+type + " " +type+ " workspaces/ 0 1 1.5 1" + "\n"

for i in range(0, 20):
    outScript="lxplusBatch_"+str(mass)+"_"+type+"_"+str(i)+".csh"
    oFile = open(outScript,'w')
    oFile.write("#!/bin/csh" + "\n")
    oFile.write("cd /afs/cern.ch/work/z/zdemirag/work/Diphoton/CMSSW_5_3_2_patch4/src/" + "\n")
    oFile.write("eval `scram runtime -csh`" +  "\n")
    oFile.write("source /afs/cern.ch/sw/lcg/external/gcc/4.3.2/x86_64-slc5/setup.csh" + "\n")
    oFile.write("cd /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.05/x86_64-slc5-gcc43-opt/root/bin/" + "\n")
    oFile.write("source thisroot.csh" + "\n")
    oFile.write("setenv PYTHONDIR /afs/cern.ch/cms/slc5_amd64_gcc434/external/python/2.6.4-cms6" + "\n")
    oFile.write("setenv PATH /afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc34-opt/bin:${PATH}" + "\n")
    oFile.write("setenv PATH ${PYTHONDIR}/bin:${PATH}" + "\n")
    oFile.write("setenv LD_LIBRARY_PATH /afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc34-opt/lib64:${LD_LIBRARY_PATH}" + "\n")
    oFile.write("setenv LD_LIBRARY_PATH ${ROOTSYS}/lib:${PYTHONDIR}/lib:${LD_LIBRARY_PATH}" + "\n")
    oFile.write("setenv PYTHONPATH /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.05/x86_64-slc5-gcc43-opt/root/lib:${PYTHONPATH}" + "\n")
    oFile.write("setenv PYTHONPATH ${ROOTSYS}/lib:${PYTHONPATH}" + "\n")
    oFile.write("setenv PATH ${PWD}:${PATH}" + "\n")
    oFile.write("cd /afs/cern.ch/work/z/zdemirag/work/Diphoton/Oct24/exost/workdir/Nspectra" + "\n")
    oFile.write("./LimitMoriond2013 0 1 0 " + str(mass) + " 50 100000 c_005_DiphotonLimits_"+type + " " +type+ " workspaces/ 0 1 1.5 1" + "\n")    
    oFile.close()
