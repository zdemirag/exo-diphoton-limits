#!/bin/csh

echo ''
echo 'Setting up python, ROOT and PyROOT'
echo ''

source /afs/cern.ch/sw/lcg/external/gcc/4.3/x86_64-slc5/setup.csh
source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.27.06/x86_64-slc5-gcc43-opt/root/bin/thisroot.csh

# setting up python and PyROOT
setenv PYTHONDIR /afs/cern.ch/cms/slc5_amd64_gcc434/external/python/2.6.4-cms6

setenv PATH /afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc34-opt/bin:${PATH}
setenv PATH ${PYTHONDIR}/bin:${PATH}
setenv LD_LIBRARY_PATH /afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc34-opt/lib64:${LD_LIBRARY_PATH}

setenv LD_LIBRARY_PATH ${ROOTSYS}/lib:${PYTHONDIR}/lib:${LD_LIBRARY_PATH}

setenv PYTHONPATH /afs/cern.ch/sw/lcg/app/releases/ROOT/5.27.06/x86_64-slc5-gcc43-opt/root/lib:${PYTHONPATH}
setenv PYTHONPATH /afs/cern.ch/cms/slc5_ia32_gcc434/cms/dbs-client/DBS_2_0_9_patch_4-cms2/lib/DBSAPI:${PYTHONPATH}
setenv PYTHONPATH /afs/cern.ch/cms/slc5_ia32_gcc434/cms/dbs-client/DBS_2_0_9_patch_4-cms2/lib:${PYTHONPATH}
setenv PYTHONPATH /afs/cern.ch/cms/slc5_ia32_gcc434/lcg/root/5.22.00d-cms22/lib:${PYTHONPATH}
setenv PYTHONPATH /afs/cern.ch/cms/slc5_ia32_gcc434/external/elementtree/1.2.6-cms8/share/lib/python2.6/site-packages:${PYTHONPATH}
setenv PYTHONPATH /afs/cern.ch/cms/slc5_ia32_gcc434/external/sip/4.8.2-cms8/lib/python2.6/site-packages:${PYTHONPATH}
setenv PYTHONPATH /afs/cern.ch/cms/slc5_ia32_gcc434/external/python-ldap/2.3.5-cms6/lib/python2.6/site-packages:${PYTHONPATH}
setenv PYTHONPATH /afs/cern.ch/cms/slc5_ia32_gcc434/external/pyqt/4.5.4-cms9/lib/python2.6/site-packages:${PYTHONPATH}
setenv PYTHONPATH ${ROOTSYS}/lib:${PYTHONPATH}

setenv PATH ${PWD}:${PATH}
chmod u+x exost
