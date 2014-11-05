#!/usr/bin/env python

################################################
#
# Submit significance study to Condor
#
#
# Gena Kukartsev, May 2011
#
################################################

import sys
import os

#-------> parameters ----------------------------------------------------
#
#    mode: data,
#          pseudo,
#

add_fake_data = False  # always False except special checks

simulate = False

mode = 'pseudo'

#channel = 'dielectron'
channel = 'dilepton'

mass_low  =     600.0
mass_high =    2500.1
mass_step =       1.0

fit_range_low = 200.0
fit_range_high = 3000.0

ntoys_per_job =   20
ntoys         = 1000

#prefix = 'local'
prefix = 'lee'

output_base_dir = 'eos_zprime'

#
#------------------------------------------------------------------------
                            
legend = '[submit_significance_condor]:'

# create workdir
from datetime import datetime
d = datetime.now()
dt = d.strftime("%d-%b-%Y_%H-%M-%S")
_dir = output_base_dir.rstrip('/')+'/'+channel+'_'+mode+'_'+prefix+'_'+dt
os.system('rm *.so *.d')
os.system('mkdir '+_dir)

# create the tar file
os.system('root -l -b -q -n twobody.C+')
os.system('tar -czhvf '+_dir+'/tarfile.tgz cmslpc_standalone_setup.sh twobody_C.so run_significance.C ws_*.root data361cmssw_diphoton.root')
_pwd = os.getcwd()
os.chdir(_dir)

cmd_file_name = 'submit.cmd'

cmd_file = open(cmd_file_name, "w")

stdout_save = sys.stdout
sys.stdout = cmd_file

print '''
# -*- sh -*- # for font lock mode
# variable definitions
- env = source ./tardir/cmslpc_standalone_setup.sh
- tag = 
- output = outputFile=
- tagmode = none
- tarfile = tarfile.tgz
- untardir = tardir

'''



_njobs = int(ntoys/ntoys_per_job)

_i = 0
while _i < _njobs:
    
    #print legend,
    print 'zprime_significance_$(JID).log',
    
    if mode == 'data':
        ntoys_per_job = 1

    _fake = 'false'
    if add_fake_data:
        _fake = 'true'

    print 'root -l -b -q -n tardir/run_significance.C(' + \
          '"'+mode+'",' + \
          '"'+channel+'",' + \
          '"tardir/",' + \
          str(mass_low)+',' + \
          str(mass_high)+',' + \
          str(mass_step)+',' + \
          str(ntoys_per_job)+',' + \
          '"_$(JID)",' + \
          str(fit_range_low)+',' + \
          str(fit_range_high)+',' + \
          _fake + \
          ')'
        
    print ''
    
    _i += 1
    
cmd_file . close()
sys.stdout = stdout_save

if simulate:
    #os.system('echo ../runManySections.py --submitCondor '+cmd_file_name)
    os.system('echo '+_pwd+'/runManySections.py --submitCondor '+cmd_file_name)
else:
    #os.system('../runManySections.py --submitCondor '+cmd_file_name)
    os.system(_pwd+'/runManySections.py --submitCondor '+cmd_file_name)

print
print legend, 'mode:                 ', mode
print legend, 'number of jobs:       ', _njobs
print legend, 'toys per job:         ', ntoys_per_job
print legend, 'TOTAL toys:           ', ntoys
print legend, 'TOTAL jobs submitted: ', _njobs
print
_whoami = os.environ['USER']
print legend, 'on LPC, monitor job progress with condor_q -global |grep', _whoami
print    

