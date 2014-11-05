exost -a workspace -c diphoton_2013.cfg

if it says:
exost: Command not found.

do (wihtout any cmsenv in any other area):

source /afs/cern.ch/sw/lcg/external/gcc/4.3.2/x86_64-slc5/setup.csh
source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.05/x86_64-slc5-gcc43-opt/root/bin/thisroot.csh

setenv PYTHONDIR /afs/cern.ch/cms/slc5_amd64_gcc434/external/python/2.6.4-cms6

setenv PATH /afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc34-opt/bin:${PATH}
setenv PATH ${PYTHONDIR}/bin:${PATH}
setenv LD_LIBRARY_PATH /afs/cern.ch/sw/lcg/contrib/gcc/4.3.2/x86_64-slc5-gcc34-opt/lib64:${LD_LIBRARY_PATH}

setenv LD_LIBRARY_PATH ${ROOTSYS}/lib:${PYTHONDIR}/lib:${LD_LIBRARY_PATH}
setenv PYTHONPATH /afs/cern.ch/sw/lcg/app/releases/ROOT/5.34.05/x86_64-slc5-gcc43-opt/root/lib:${PYTHONPATH}
setenv PYTHONPATH ${ROOTSYS}/lib:${PYTHONPATH}

setenv PATH ${PWD}:${PATH}

Now you have: myWS.root

Then Go to the Nspectra Folder and do: 
make -f GNUmakefileLimitMoriond2013


        [1] type: int (0 -> false; 1,2,.. -> true), meaning: ebeb
        [2] type: int (0 -> false; 1,2,.. -> true), meaning: ebee
        [3] type: int (0 -> false; 1,2,.. -> true), meaning: dimuon
        [5] type: double, meaning: signal mass hypothesis
        [6] type: int, meaning: iterations/toy experiments
        [7] type: int, meaning: length of the Markov Chain
        [8] type: string, meaning: file name suffix for the output
        [9] type: string, meaning: mode of running: available options are 'observed' or 'expected'
        [10] type: string, meaning: path to the directory with the single channel workspaces
        [11] type: int (0 -> false; 1,2,.. -> true), meaning: write posterior plots?
        [12] type: int (0 -> false; 1,2,.. -> true), meaning: restrict mass range for the unbinned likelihood?
        [13] type: double, meaning: factor on estimated POI range (for convergence tests)
        [14] type: int (0 -> false; 1,2,.. -> true), meaning: apply systematic uncertainty on resonance mass hypothesis? 

 dielectron ebeb, dielectron ebee and dimuon
 ./LimitMoriond2013 1 0 0 1000 2 100000 testLim_allchannels_Moriond13 observed workspaces/ 0 1 1.5 1 

./LimitMoriond2013 0 1 0 1000 2 100000 c_01_DiphotonLimits_observed observed workspaces/ 0 1 1.5 1


grep -h "adjusting Z' mass hypothesis from 1000 to " LSFJOB_*/STDOUT > mass
grep -h "95% upper limit on ratio is : " LSFJOB_*/STDOUT > limits


bsub -q 1nd -J lxplusBatch_650_expected_15 < lxplusBatch_650_expected_15.csh


  graph->SetPoint(0,750,0.4125598073);
   graph->SetPoint(1,1000,0.466852814);
   graph->SetPoint(2,1250,0.5118468404);
   graph->SetPoint(3,1500,0.5472701788);
   graph->SetPoint(4,1750,0.589081347);
   graph->SetPoint(5,2000,0.6204102635);
   graph->SetPoint(6,2250,0.6408445239);
   graph->SetPoint(7,2500,0.6485947967);
   graph->SetPoint(8,3000,0.6592876315);




