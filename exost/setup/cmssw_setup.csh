#!/bin/tcsh

echo ''
echo 'Updating the path'
echo ''

setenv PATH ${PWD}:${PATH}
chmod u+x exost

set roofit_version = `scramv1 tool info roofit| perl -ne 'if (m|Version : \s*(\S+)|) {print "$1\n"}'`
set roofit_location = `echo $ROOTSYS | perl -F/ -lane 'print join ("/",@F[0..$#F-2])'`
set roofit_path = $roofit_location/roofit/$roofit_version

echo RooFit version used by this CMSSW release: $roofit_path

setenv ROOT_INCLUDE ${ROOTSYS}/include
setenv ROOFIT_INCLUDE $roofit_path/include

echo''
echo 'If you plan to use ROOT interactively,'
echo 'execute this line every time or add it to your rootlogon.C'
echo''
echo '     gSystem -> SetIncludePath( "-I$ROOT_INCLUDE -IROOFIT_INCLUDE" );'
echo''
