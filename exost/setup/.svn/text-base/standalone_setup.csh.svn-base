#!/bin/csh

echo ''
echo 'Setting up python, ROOT and PyROOT'
echo ''
echo 'I assume that ROOTSYS is set to the desired ROOT release'
echo ''

unset root

unset LD_LIBRARY_PATH
setenv LD_LIBRARY_PATH "$ROOTSYS/lib"
alias root "$ROOTSYS/bin/root"
setenv PATH "$ROOTSYS/bin:${PATH}:"

# setup PyROOT
setenv LD_LIBRARY_PATH $ROOTSYS/lib:$LD_LIBRARY_PATH
setenv PYTHONPATH $ROOTSYS/lib:$PYTHONPATH

setenv PATH ${PWD}:${PATH}
chmod u+x exost
