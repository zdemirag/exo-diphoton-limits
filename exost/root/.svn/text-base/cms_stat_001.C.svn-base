// cms_stat_001.C  version 0.9
//
// Standard routine for Bayesian MCMC limit calculation
 
/*
Adapted from Kyle Cranmer's
$ROOTSYS/tutorials/roostats/StandardBayesianMCMCDemo.C

Adapted for CMS by Gena Kukartsev
date: Feb. 2011

This is a standard routine that should be used for Bayesian
interval calculations. Use it with any input
prepared in the standard way.  You specify:
   - pointer to RooWorkspace that contains the model
   - pointer to ModelConfig that specifies details for calculator tools
     (may be inside the same workspace but does not have to)
   - pointer to the dataset 
 Optional parameters:
   - confidence level, from 0 to 1
   - left side tail fraction
     (if not between 0 and 1, the shortest interval is evaluated)
   - number of Metropolis-Hastings algorith iterations
   - number of burn-in steps in the Markov chain to discard
   - posterior plot file name (if 0, not created)
   - output verbosity. Set to 0 for silent

The routine neither copies nor owns the input objects: you are
responsible for their existence and clean up.

The user takes ownership of the returned MCMCInterval object.

The actual heart of the routine is only about 10 lines long.

The MCMCCalculator is a Bayesian tool that uses
the Metropolis-Hastings algorithm to efficiently integrate
in many dimensions.  It is not as accurate as the BayesianCalculator
for simple problems, but it scales to much more complicated cases.
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

using namespace RooFit;
using namespace RooStats;

RooStats::MCMCInterval * 
StandardBayesianMCMCInterval(RooWorkspace * ws_ptr,
			     RooStats::ModelConfig * mconf_ptr,
			     RooAbsData * data_ptr,
			     double confidence_level = 0.95,
			     double left_side_tail_fraction = -1.0,
			     int n_mcmc_iterations = 1000000,
			     int n_mcmc_burn_in_steps = 500,
			     const char * posterior_plot_file_name = 0,
			     int verbosity = 1){
  // Parameters:
  //   - pointer to RooWorkspace that contains the model
  //   - pointer to ModelConfig that specifies details for calculator tools
  //     (may be inside the same workspace but does not have to)
  //   - pointer to the dataset 
  // Optional parameters:
  //   - confidence level, from 0 to 1
  //   - left side tail fraction
  //     (if not between 0 and 1, the shortest interval is evaluated)
  //   - number of Metropolis-Hastings algorith iterations
  //   - number of burn-in steps in the Markov chain to discard
  //   - posterior plot file name (if 0, not created)
  //   - output verbosity. Set to 0 for silent


  /////////////////////////////////////////////////////////////
  //
  // Validate input first
  // 
  ////////////////////////////////////////////////////////////

  if(!ws_ptr){
    cout <<"workspace not found, exiting" << endl;
    return 0;
  }

  if(!mconf_ptr){
    cout << "ModelConfig was not found, exiting" << endl;
    return 0;
  }

  if(!data_ptr){
    cout << "data were not found, exiting" << endl;
    return 0;
  }

  if(confidence_level < 0.0 || confidence_level > 1.0){
    cout << "invalid confidence level requested, exiting" << endl;
    return 0;
  }

  if(n_mcmc_iterations < 0){
    cout << "negative number of MCMC iterations specified, exiting" <<endl;
    return 0;
  }

  if(n_mcmc_burn_in_steps < 0){
    cout << "negative number of MCMC burn-in steps, exiting" <<endl;
    return 0;
  }

  /////////////////////////////////////////////
  // create and use the MCMCCalculator
  // to find and plot the credible interval
  // on the parameter of interest as specified
  // in the model config
  MCMCCalculator mcmc(*data_ptr, *mconf_ptr);
  mcmc.SetConfidenceLevel(confidence_level);
  mcmc.SetNumIters(n_mcmc_iterations);          // Metropolis-Hastings algorithm iterations
  mcmc.SetNumBurnInSteps(n_mcmc_burn_in_steps); // first N steps to be ignored as burn-in

  // default is the shortest interval unless tail fraction between [0,1] specified
  if (left_side_tail_fraction >= 0.0 && left_side_tail_fraction <= 1.0){
    mcmc.SetLeftSideTailFraction(left_side_tail_fraction);
  }

  MCMCInterval * interval = mcmc.GetInterval();

  // make a plot
  if (posterior_plot_file_name){
    TCanvas c1("c1");
    MCMCIntervalPlot plot(*interval);
    plot.Draw();
    c1.SaveAs(posterior_plot_file_name);
  }
  
  // print out the interval on the first Parameter of Interest
  if (verbosity > 0){
    RooRealVar * firstPOI = (RooRealVar*) mconf_ptr->GetParametersOfInterest()->first();
    cout << "\n95% interval on " <<firstPOI->GetName()<<" is : ["<<
      interval->LowerLimit(*firstPOI) << ", "<<
      interval->UpperLimit(*firstPOI) <<"] "<<endl;
  }

  return interval;
}
