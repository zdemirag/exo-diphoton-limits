#!/usr/bin/env python

################################################
#
# Submit graviton combination to Condor
#
#
# Gena Kukartsev, April 2011
#
################################################

import sys
import os

#-------> parameters ----------------------------------------------------
#
# channel: dimuon, dielectron, dilepton,
#          mumu-ee-gg-xsec
#
#    mode: observed,
#          expected,
#          mass_cteq
#          mass_graviton
#

simulate = False

#channel = 'dimuon'
channel = 'dielectron'
#channel = 'dilepton'

mode = 'observed'
#mode = 'expected'
#mode = 'mass_cteq'
#mode = 'mass_graviton'

prefix = channel+'_'+mode

#mass      = range(300,2550,50)
#mass      = range(1500,2200,100)
#mass      = [300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1700,1800,1900,2000,2100,2200,2300,2400,2500]
#mass      = [300,500,1000,1500,2000,2500]
mass      = [1000]
n_iter    =     10000
n_burn_in =       100

exp_ntoys_per_job = 1           # these params are now used for observed as well
exp_ntoys_per_mass_point = 1

# posterior and MCMC scatter plots
# leave empty to disable plots
#plot_name_base = ''
plot_name_base = 'mcmc.pdf'

output_base_dir = 'eos_zprime'

#
#------------------------------------------------------------------------
                            
legend = '[submit_condor]:'

# create workdir
from datetime import datetime
d = datetime.now()
dt = d.strftime("%d-%b-%Y_%H-%M-%S")
_dir = output_base_dir.rstrip('/')+'/'+prefix+'_'+dt
os.system('rm *.so *.d')
os.system('mkdir '+_dir)

# create the tar file
os.system('root -l -b -q -n twobody.C+')
#os.system('tar -czhvf '+_dir+'/tarfile.tgz cmslpc_standalone_setup.sh twobody_C.so run_diphoton_comb.C ws_*.root data361cmssw_diphoton.root')
os.system('tar -czhvf '+_dir+'/tarfile.tgz cmslpc_standalone_setup.sh twobody_C.so run_limit.C ws_*.root data361cmssw_diphoton.root')
os.system('cp runMany.bash '+_dir+'/')
os.system('cp runManySections.py '+_dir+'/')
os.chdir(_dir)

prefix = prefix + '_' + mode

cmd_file_name = prefix + '.cmd'

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



#_npoints = int((mass_max-mass_min)/mass_inc)+1
#_njobs = _npoints

#_i = 0
#while _i < _njobs:
#    _peak = mass_min + _i*mass_inc

_npoints = 0
for _peak in mass:

    _npoints += 1
    
    #print legend,
    print prefix+'_$(JID).log',

    # commented out because ntoys is now used for observed too,
    # to run several times and everage
    #if mode == 'observed' or mode == 'mass':
    #    exp_ntoys_per_job = 1

#    print 'root -l -b -q -n tardir/run_diphoton_comb.C(' + \
    print 'root -l -b -q -n tardir/run_limit.C(' + \
          '"'+channel+'",' + \
          '"'+mode+'",' + \
          str(_peak)+',' + \
          '"'+prefix+'_$(JID)",' + \
          str(exp_ntoys_per_job)+',' + \
          str(n_iter)+',' + \
          str(n_burn_in)+',' + \
          '"tardir/"'+',' + \
          '"'+plot_name_base+'"' + \
          ')'
        
    print ''
    
#    _i += 1
#_npoints = int((mass_max-mass_min)/mass_inc)+1
_njobs = _npoints
    
cmd_file . close()
sys.stdout = stdout_save

if mode == 'expected':
    _nsubmit = int(exp_ntoys_per_mass_point/exp_ntoys_per_job)
    if exp_ntoys_per_mass_point % exp_ntoys_per_job != 0:
        _nsubmit +=1
else:
    #_nsubmit = 1
    _nsubmit = int(exp_ntoys_per_mass_point/exp_ntoys_per_job)
    if exp_ntoys_per_mass_point % exp_ntoys_per_job != 0:
        _nsubmit +=1


for i in range(0, _nsubmit):
    if simulate:
        #os.system('echo ../runManySections.py --submitCondor '+cmd_file_name)
        os.system('echo ./runManySections.py --submitCondor '+cmd_file_name)
    else:
        #os.system('../runManySections.py --submitCondor '+cmd_file_name)
        os.system('./runManySections.py --submitCondor '+cmd_file_name)

print
#print legend, 'data:                ', data
print legend, 'mode:                ', mode
#print legend, 'method:              ', method
print legend, 'mass points:         ', _npoints
if mode == 'observed':
    print legend, 'number of jobs:      ', _njobs
if mode == 'expected':
    #print legend, 'channel:                       ', exp_channel
    print legend, 'toys per job:                  ', exp_ntoys_per_job
    print legend, 'toys per mass point requested: ', exp_ntoys_per_mass_point
    print legend, 'toys per mass point submitted: ', _nsubmit*exp_ntoys_per_job
    print legend, 'jobs per mass point submitted: ', _nsubmit
print legend, 'TOTAL jobs submitted:          ', _npoints*_nsubmit

print
_whoami = os.environ['USER']
print legend, 'on LPC, monitor job progress with condor_q -global |grep', _whoami
print    

