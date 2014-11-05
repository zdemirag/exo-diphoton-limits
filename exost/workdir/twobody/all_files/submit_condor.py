#!/usr/bin/env python

################################################
#
# Submit graviton combination to Condor
#
#
# Gena Kukartsev, April 2011
#
# March 2012:
#            - output to EOS
#            - ship ROOT with the job
#            - clean up input tar file
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

channel = 'dimuon'
#channel = 'dielectron'
#channel = 'dilepton'

#mode = 'observed'
mode = 'mass_cteq'
#mode = 'mass_graviton'
#mode = 'expected'

prefix = channel+'_'+mode

mass      = range(300,1050,25)
#mass      = range(300,2550,50)
#mass      = range(1450,2550,50)
#mass      = range(1900,2400,100)
#mass      = range(1700,2200,100)
#mass      = [320,340,360,380,420,440,460,480,520,540,560,580,620,640,660,680,720,740,760,780,825,875,925,975,1025,1075,1125,1175,1225,1275,1325,1375]
#mass      = [300,350,400,450,500,550,600,650,700,750,800,850,900,950,1000,1050,1100,1150,1200,1250,1300,1350,1400,1450,1500,1600,1700,1800,1900,2000,2100,2300,2500]
#mass      = [1000]
n_iter    =     10000
n_burn_in =       100

exp_ntoys_per_job = 50           # these params are now used for observed as well
exp_ntoys_per_mass_point = 50

# posterior and MCMC scatter plots
# leave empty to disable plots
plot_name_base = ''
#plot_name_base = 'mcmc.pdf'

output_base_dir = 'eos_zprime'

#root_tarball = '/uscms_data/d2/kukarzev/root/root_v5.32.00.Linux-slc5_amd64-gcc4.3.tar.gz'
root_tarball = '/uscms_data/d2/kukarzev/root/root_v5.30.02.Linux-slc5_amd64-gcc4.3.tar.gz'

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
#os.system('root -l -b -q -n twobody.C+')
os.system('rm ./root.tar.gz')
os.system('ln -s '+root_tarball+' ./root.tar.gz')
os.system('tar -czhvf '+_dir+'/tarfile.tgz cmslpc_batch_setup.sh run_limit.C twobody.C ws_dimuon_ratio.root ws_dielectron_ratio.root root.tar.gz')
_pwd = os.getcwd()
os.chdir(_dir)

prefix = prefix + '_' + mode

cmd_file_name = prefix + '.cmd'

cmd_file = open(cmd_file_name, "w")

stdout_save = sys.stdout
sys.stdout = cmd_file

print '''
# -*- sh -*- # for font lock mode
# variable definitions
- env = source ./tardir/cmslpc_batch_setup.sh
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
        os.system('echo '+_pwd+'/runManySections.py --submitCondor --noDeleteCondor '+cmd_file_name)
    else:
        os.system(_pwd+'/runManySections.py --submitCondor --noDeleteCondor '+cmd_file_name)

print
print legend, 'mode:                ', mode
print legend, 'mass points:         ', _npoints
if mode == 'observed':
    print legend, 'number of jobs:      ', _njobs
if mode == 'expected':
    print legend, 'toys per job:                  ', exp_ntoys_per_job
    print legend, 'toys per mass point requested: ', exp_ntoys_per_mass_point
    print legend, 'toys per mass point submitted: ', _nsubmit*exp_ntoys_per_job
    print legend, 'jobs per mass point submitted: ', _nsubmit
print legend, 'TOTAL jobs submitted:          ', _npoints*_nsubmit

print
_whoami = os.environ['USER']
print legend, 'on LPC, monitor job progress with condor_q -global |grep', _whoami
print    

