// cms_test_001.C  version 0.9
//
// Test macro for cms_stat_001.C
 
/*

Author: Gena Kukartsev
date: Feb. 2011

Test standard Bayesian MCMC credible interval calculation

*/

#include "TFile.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "RooWorkspace.h"
#include "RooAbsData.h"

#include "RooStats/ModelConfig.h"
#include "RooStats/MCMCCalculator.h"
#include "RooStats/MCMCInterval.h"
#include "RooStats/MCMCIntervalPlot.h"

#include "../root/cms_stat_001.C"

using namespace RooFit;
using namespace RooStats;

void StandardBayesianMCMCInterval_test(const char* infile = "myWS.root",
				       const char* workspaceName = "myWS",
				       const char* modelConfigName = "counting_model_config",
				       const char* dataName = "data"){

  /////////////////////////////////////////////////////////////
  //
  // First part is just to access a user-defined file 
  //
  ////////////////////////////////////////////////////////////

  const char* filename = "";
  if (!strcmp(infile,""))
    filename = "../data/examples/counting_ws.root";
  else
    filename = infile;
  // Check if example input file exists
  TFile *file = TFile::Open(filename);

  // if input file was specified byt not found, quit
  if(!file && strcmp(infile,"")){
    cout <<"file not found, exiting" << endl;
    return;
  } 

  // get the workspace out of the file
  RooWorkspace* w = (RooWorkspace*) file->Get(workspaceName);
  if(!w){
    cout <<"workspace not found, exiting" << endl;
    return;
  }

  // get the modelConfig out of the file
  ModelConfig* mc = (ModelConfig*) w->obj(modelConfigName);
  mc->SetWorkspace(*w);

  // get the modelConfig out of the file
  RooAbsData* data = w->data(dataName);

  // make sure ingredients are found
  if(!data || !mc){
    w->Print();
    cout << "data or ModelConfig was not found, exiting" <<endl;
    return;
  }

  // print out the contents of the workspace
  w->Print();

  ////////////////////////////////////////////////////////////////
  //
  // call the standard Bayesian MCMC interval calculation routine
  //
  ////////////////////////////////////////////////////////////////
  StandardBayesianMCMCInterval(w, mc, data, 
			       0.95, 0.0, 10000, 500,
			       "mcmc_posterior.png",
			       1);
}
