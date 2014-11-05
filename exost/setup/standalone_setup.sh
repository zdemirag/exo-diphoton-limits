#!/bin/sh

echo ''
echo 'Setting up python, ROOT and PyROOT'
echo ''
echo 'I assume that ROOTSYS is set to the desired ROOT release'
echo ''

unset root

unset LD_LIBRARY_PATH
export LD_LIBRARY_PATH="$ROOTSYS/lib"
alias root="$ROOTSYS/bin/root"
export PATH="$ROOTSYS/bin:${PATH}:"

# setup PyROOT
export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$ROOTSYS/lib:$PYTHONPATH

export PATH=${PWD}:${PATH}
chmod u+x exost
