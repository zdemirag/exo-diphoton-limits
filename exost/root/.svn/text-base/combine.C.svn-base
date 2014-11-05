//////////////////////////////////////////////////////////////////////////
//
// roostats_combine.C  version 0.2
//
// Standard c++ routine for statistical inference for the case
// of a combination of multiple analysis channels
//
// Uses simultaneous p.d.f.s to describe simultaneous fits to multiple
// datasets
//
// The procedure is model-independent
//
//
// Gena Kukartsev
//
// March 2011: first version
// 

/*
The code should be compiled in ROOT:

root -l

.L roostats_ccombine.C+

Usage:
       limit = roostats_combine(param, ...);

Inputs:
       param          - Parameter


Description goes here
*/

#include <iostream>

#include "TFile.h"
#include "RooGlobalFunc.h"
#include "RooWorkspace.h"
#include "RooAbsPdf.h"
#include "RooCategory.h"
#include "RooSimultaneous.h"
#include "RooDataSet.h"
#include "RooRealVar.h"

#include "RooStats/ModelConfig.h"
#include "RooStats/SimpleInterval.h"
#include "RooStats/BayesianCalculator.h"

// FIXME: remove namespaces
using namespace RooFit;
using namespace RooStats;
using namespace std;

Double_t roostats_combine( std::string filename1, std::string filename2 );


// ---> implementation below --------------------------------------------

class GreatCombinator{
  //
  // The class combines multiple channel analyses. A workspace is created
  // with the combined model, data, model config. The class can also call
  // interval calculation routines
  //

public:

  GreatCombinator();
  ~GreatCombinator();

  void prototype( std::string filename1, std::string filename2 );

  RooAbsPdf * AddModel(std::string channel, RooAbsPdf * pdf){return 0;};

  RooWorkspace * AddChannel(RooAbsData * in_data,
  			    RooAbsPdf * in_pdf,
  			    RooArgSet * in_poi,
  			    RooArgSet * in_nuis,
  			    RooAbsPdf * in_prior = 0);
  
  
  //void SaveWorkspace(std::string filename);


private:

  std::set<std::string> channels;
  std::map<std::string, RooAbsPdf *> models;
  

  RooCategory channel_cat;
  RooAbsData * data;
  RooAbsPdf * model;
  RooArgSet * poi;
  RooArgSet * nuis;
  RooAbsPdf * prior;
  RooStats::ModelConfig mc;
  RooWorkspace * ws;

};


GreatCombinator::GreatCombinator(){
  ws = new RooWorkspace("ws");
  mc.SetName("modelconfig");
  mc.SetTitle("ModelConfig for roostats_combine");

  data = 0;
  model = 0;
  poi = 0;
  nuis = 0;
  prior = 0;
}


GreatCombinator::~GreatCombinator(){
  delete ws;
  delete data;
  delete model;
  delete poi;
  delete nuis;
  delete prior;
}


RooAbsPdf * AddModel(std::string channel, RooAbsPdf * pdf);


void GreatCombinator::prototype( std::string filename1, std::string filename2 ){

  // input
  TFile file1(filename1.c_str(), "read");
  RooWorkspace * ws1 = (RooWorkspace *)file1.Get( "ws" );

  ws1->Print();

  TFile file2(filename2.c_str(), "read");
  RooWorkspace * ws2 = (RooWorkspace *)file2.Get( "ws" );


  // category
  RooCategory channels("channels", "channels");
  channels.defineType("channel001");
  channels.defineType("channel002");
  ws->import(channels);

  // models
  RooAbsPdf * model1 = ws1->pdf("model");
  RooAbsPdf * model2 = ws2->pdf("model");

  //RooWorkspace * ws = (RooWorkspace *)ws1->Clone();

  ws->import( *model1,
	      RooFit::RenameAllNodes("model1"),
	      RooFit::RenameAllVariablesExcept("model1", "xsec") );
  ws->import( *model2,
	      RooFit::RenameAllNodes("model2"),
	      RooFit::RenameAllVariablesExcept("model2", "xsec") );

  RooSimultaneous model("model", "model", channels);
  model.addPdf(*ws->pdf("model_model1"), "channel001");
  model.addPdf(*ws->pdf("model_model2"), "channel002");
  ws->import(model);
  //ws->factory("SIMUL::model(channels)");
  

  // data
  RooAbsData * data1 = ws1->data("observed_data");
  RooAbsData * data2 = ws2->data("observed_data");

  std::map<std::string, RooDataSet *> mData;
  mData["channel001"] = (RooDataSet *)data1;
  mData["channel002"] = (RooDataSet *)data2;

  RooRealVar * n1 = ws->var("n_model1");
  RooRealVar * n2 = ws->var("n_model2");

  RooDataSet data( "combdata", "combined data",
		   RooArgSet((const RooRealVar)(*n1), (const RooRealVar)(*n2) ),
		   Index(channels),
		   Import(mData) );

  ws->import( data );
  

  // POI
  RooArgSet sPoi( *(ws->var("xsec")) );

  // nuisance
  RooArgSet sNuis( *(ws->var("nsig_nuis_model1")),
		   *(ws->var("nbkg_nuis_model1")),
		   *(ws->var("nsig_nuis_model2")),
		   *(ws->var("nbkg_nuis_model2")) );

  // observables
  RooArgSet sObs( *(ws->var("n_model1")),
		  *(ws->var("n_model2")) );

  // prior
  ws->factory( "Uniform::prior(xsec)" );  

  // ModelConfig
  mc.SetWorkspace(*ws);
  mc.SetPdf(*(ws->pdf("model")));
  mc.SetParametersOfInterest( sPoi );
  mc.SetPriorPdf( *(ws->pdf("prior")) );
  mc.SetNuisanceParameters( sNuis );
  mc.SetObservables( sObs );

  ws->import(mc);

  file1.Close();
  file2.Close();

  ws->Print();

  // bayesian calculation
  BayesianCalculator * bcalc = new BayesianCalculator( *(ws->data("combdata")), mc);
  bcalc->SetName("bc");
  bcalc->SetConfidenceLevel(0.90);
  
  RooStats::SimpleInterval * sInt = bcalc->GetInterval();
  double upper_limit = sInt->UpperLimit();
  delete sInt;
  delete bcalc;

  std::cout << "Bayesian 95% C.L. upper limit: " << upper_limit << std::endl;

  return;
}


RooWorkspace * GreatCombinator::AddChannel(RooAbsData * in_data,
					   RooAbsPdf * in_pdf,
					   RooArgSet * in_poi,
					   RooArgSet * in_nuis,
					   RooAbsPdf * in_prior){
  return ws;
}



Double_t roostats_combine(std::string filename1,
			  std::string filename2){

  Double_t limit = -1.0;  

  GreatCombinator comb;
  comb.prototype( filename1, filename2 );

  return limit;
}

