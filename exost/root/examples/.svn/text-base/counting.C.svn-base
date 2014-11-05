// This is an example of a RooStats interval calculation
// for a counting experiment
//
// ROOT macro approach

/*
RooStats exercise at CMSDAS-2011:
  Fermilab, January 25-29, 2011
  author: Gena Kukartsev
  inspired by official RooStats tutorials,
  see http://root.cern.ch/root/html/tutorials/roostats/

  date: January 24, 2011

*/

#include "TCanvas.h"
#include "RooWorkspace.h"
#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooStats/BayesianCalculator.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/SimpleInterval.h"

using namespace RooFit;
using namespace RooStats;

void counting( void ){
  //
  // this function implements the interval calculation for a counting experiment
  //
 
  // make model
  RooWorkspace * wspace = new RooWorkspace("myWS");
  wspace -> factory("Uniform::model_pdf_1(x[0,1])");
  wspace -> factory("Uniform::model_pdf_2(x)");
  wspace -> factory("SUM::model_pdf(s[0,15]*model_pdf_1,b[1,0,2]*model_pdf_2)");
  wspace -> factory("Lognormal::likelihood_b(b,1,3)");
  wspace -> factory("PROD::model_likelihood(model_pdf, likelihood_b)");
  wspace -> factory("Uniform::prior_pdf(s)");
  
  // define observables
  wspace -> defineSet("observables","x");

  // define parameters of interest
  wspace -> defineSet("poi","s");

  // define nuisance parameters
  wspace -> defineSet("nuisance_parameters","b");

  // load data
  RooArgList * observables = new RooArgList( *wspace->set("observables") );
  RooDataSet * data = RooDataSet::read("counting_data_3.ascii", *observables);
  data -> SetName("data");
  wspace -> import(*data);

  // model config
  ModelConfig modelConfig("counting_model_config");
  modelConfig . SetWorkspace(*wspace);
  modelConfig . SetPdf(*wspace->pdf("model_likelihood"));
  modelConfig . SetPriorPdf(*wspace->pdf("prior_pdf"));
  modelConfig . SetParametersOfInterest(*wspace->set("poi"));
  modelConfig . SetNuisanceParameters(*wspace->set("nuisance_parameters"));
  wspace -> import(modelConfig, "counting_model_config");

  // Bayesian Calculator
  BayesianCalculator bc(*data, modelConfig);
  bc.SetName("exostBayes");
  wspace -> import(bc);

  // inspect workspace
  wspace -> Print();

  // save workspace to file
  wspace -> writeToFile("myWS.root");

  // run Bayesian calculation
  bc.SetConfidenceLevel(0.95);
  SimpleInterval* bInt = 0;
  bInt = bc.GetInterval();

  // make plots
  TCanvas * c1 = new TCanvas("c1");
  RooPlot * bplot = bc.GetPosteriorPlot();
  bplot -> Draw();
  c1 -> SaveAs("posterior_pdf.png");

  // query interval
  std::cout << "Bayesian interval on s = [" << 
    bInt->LowerLimit( ) << ", " <<
    bInt->UpperLimit( ) << "]" << std::endl;
}
