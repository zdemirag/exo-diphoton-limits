#!/bin/sh

echo ''
echo 'Setting up python, ROOT and PyROOT'
echo ''

arch=slc5_ia32_gcc434
#roofit_release=5.27.04

export CMS_PATH=/uscmst1/prod/sw/cms

export PYTHONDIR=/uscmst1/prod/sw/cms/slc5_ia32_gcc434/external/python/2.6.4-cms8

export PATH=${PYTHONDIR}/bin:/uscms/home/kukarzev/nobackup/root/root_v5.28.00b/bin:$PATH
export ROOTSYS=/uscms/home/kukarzev/nobackup/root/root_v5.28.00b
export PYTHONPATH=${ROOTSYS}/lib:${PYTHONPATH}

export LD_LIBRARY_PATH=${PYTHONDIR}/lib:${CMS_PATH}/$arch/external/gcc/4.3.4/lib:${ROOTSYS}:${ROOTSYS}/lib:${LD_LIBRARY_PATH}

export ROOT_INCLUDE=${ROOTSYS}/include

export PATH=${PWD}:${PATH}
chmod u+x exost
