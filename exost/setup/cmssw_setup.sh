#!/bin/sh

echo ''
echo 'Updating the path'
echo ''

export PATH=${PWD}:${PATH}
chmod u+x exost

roofit_version=`scramv1 tool info roofit| perl -ne 'if (m|Version : \s*(\S+)|) {print "$1\n"}'`
roofit_location=`echo $ROOTSYS | perl -F/ -lane 'print join ("/",@F[0..$#F-2])'`
roofit_path=$roofit_location/roofit/$roofit_version

echo RooFit version used by this CMSSW release: $roofit_path

export ROOT_INCLUDE=${ROOTSYS}/include
export ROOFIT_INCLUDE=$roofit_path/include

echo''
echo 'If you plan to use ROOT interactively,'
echo 'execute this line every time or add it to your rootlogon.C'
echo''
echo '     gSystem -> SetIncludePath( "-I$ROOT_INCLUDE -IROOFIT_INCLUDE" );'
echo''
