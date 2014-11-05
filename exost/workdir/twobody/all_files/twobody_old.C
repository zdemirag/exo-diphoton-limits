//////////////////////////////////////////////////////////////////////////
//
// twobody.C
//
// Routine for statistical inference for the case
// of a combination of three two-body resonance searches:
// di-muon, di-electron and di-photon
//
// Uses simultaneous p.d.f.s to describe simultaneous fits to multiple
// datasets
//
//
// Gena Kukartsev
//
// April 2011: first version
// 

/*
The code should be compiled in ROOT:

root -l

.L twobody.C+

Usage:
       limit = limit(param, ...);

Inputs:
       param          - Parameter


Description goes here
*/

#include <iostream>
#include <fstream>
#include <stdexcept>

#include "TFile.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TUnixSystem.h"
#include "TStopwatch.h"
#include "TMath.h"

#include "RooGlobalFunc.h"
#include "RooRandom.h"
#include "RooWorkspace.h"
#include "RooAbsPdf.h"
#include "RooCategory.h"
#include "RooSimultaneous.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooFitResult.h"
#include "RooPlot.h"
#include "RooHistPdf.h"

#include "RooStats/ModelConfig.h"
#include "RooStats/SimpleInterval.h"
#include "RooStats/BayesianCalculator.h"
#include "RooStats/MCMCCalculator.h"
#include "RooStats/MCMCInterval.h"
#include "RooStats/MCMCIntervalPlot.h"
#include "RooStats/SequentialProposal.h"
#include "RooStats/ProposalHelper.h"
#include "RooStats/ProfileLikelihoodCalculator.h"
#include "RooStats/LikelihoodInterval.h"
#include "RooStats/HypoTestResult.h"

#include "diphoton.C"

// FIXME: remove namespaces
using namespace RooFit;
using namespace RooStats;
using namespace std;

Double_t limit( std::string channel,           // dimuon, dielectron...
		std::string mode,              // obsereved, expected, mass limit (extra k-factor uncertainty)
		Float_t peak,
		std::string suffix = "",       // suffix for output file names
		Int_t ntoys = 1,               // number of pseudoexperiments for expected limit
		Int_t mcmc_iter = 100000,      // number of MCMC iterations
		Int_t mcmc_burnin = 100,       // number of MCMC burn in steps to be discarded
		std::string inputdir = "");    // directory with workspace files

Double_t significance( std::string mode,
		       std::string channel = "dilepton",
		       std::string inputdir = "", // directory with workspace files
		       Double_t mass_low = 250.0,
		       Double_t mass_high = 1450.0,
		       Double_t mass_step = 3.0,
		       Int_t ntoys = 1,
		       std::string suffix = "",
		       Double_t fit_range_low = 200.0,
		       Double_t fit_range_high = 2000.0,
		       bool add_fake_data = false,
		       Double_t width_factor = 1.0);

Double_t robustness( std::string inputdir, // directory with workspace files
		     std::string channel,
		     Double_t mass_low = 250.0,
		     Double_t mass_high = 1450.0,
		     Double_t mass_step = 3.0,
		     std::string option = "low_range" );

Double_t dimuon_ratio_limit( Float_t peak,
			     std::string mode = "observed", // obsereved, expected, mass limit (extra k-factor uncertainty)
			     std::string suffix = "",       // suffix for output file names
			     Int_t ntoys = 1,               // number of pseudoexperiments for expected limit
			     Int_t mcmc_iter = 100000,      // number of MCMC iterations
			     Int_t mcmc_burnin = 100,       // number of MCMC burn in steps to be discarded
			     std::string inputdir = "");    // directory with workspace files

Double_t dilepton_diphoton_combine( Float_t peak,
				    std::string mode = "observed", // obsereved, expected, mass limit (extra k-factor uncertainty)
				    std::string suffix = "",       // suffix for output file names
				    Int_t ntoys = 1,               // number of pseudoexperiments for expected limit
				    Int_t mcmc_iter = 100000,      // number of MCMC iterations
				    Int_t mcmc_burnin = 100,       // number of MCMC burn in steps to be discarded
				    std::string inputdir = "");    // directory with workspace files

void twobody(void){} // dummy

// ---> implementation below --------------------------------------------

class TwoBody{
  //
  // The class combines multiple channel analyses. A workspace is created
  // with the combined model, data, model config. The class can also call
  // interval calculation routines
  //

public:

  TwoBody();
  ~TwoBody();
  
  void DimuonRatioLimit( Float_t peak,
			 std::string mode = "observed", // obsereved, expected, mass limit (extra k-factor uncertainty)
			 std::string suffix = "",  // suffix for output file names
			 Int_t nruns = 1,          // number of pseudoexperiments for expected limit
			 Int_t mcmc_iter = 100000, // number of MCMC iterations
			 Int_t mcmc_burnin = 100,  // number of MCMC burn in steps to be discarded
			 std::string inputdir = ""); // directory with workspace files
  
  void DielectronRatioLimit( Float_t peak,
			     std::string mode = "observed", // obsereved, expected, mass limit (extra k-factor uncertainty)
			     std::string suffix = "",  // suffix for output file names
			     Int_t nruns = 1,          // number of pseudoexperiments for expected limit
			     Int_t mcmc_iter = 100000, // number of MCMC iterations
			     Int_t mcmc_burnin = 100,  // number of MCMC burn in steps to be discarded
			     std::string inputdir = ""); // directory with workspace files
  
  void DileptonRatioLimit( Float_t peak,
			   std::string mode = "observed", // obsereved, expected, mass limit (extra k-factor uncertainty)
			   std::string suffix = "",  // suffix for output file names
			   Int_t nruns = 1,          // number of pseudoexperiments for expected limit
			   Int_t mcmc_iter = 100000, // number of MCMC iterations
			   Int_t mcmc_burnin = 100,  // number of MCMC burn in steps to be discarded
			   std::string inputdir = ""); // directory with workspace files
  
  void ThreeChannelLimit( Float_t peak,
			  std::string mode = "observed", // obsereved, expected, mass limit (extra k-factor uncertainty)
			  std::string suffix = "",  // suffix for output file names
			  Int_t nruns = 1,          // number of pseudoexperiments for expected limit
			  Int_t mcmc_iter = 100000, // number of MCMC iterations
			  Int_t mcmc_burnin = 100,  // number of MCMC burn in steps to be discarded
			  std::string inputdir = ""); // directory with workspace files

  Double_t DileptonRatioSignificance( std::string mode,
				      std::string channel,
				      std::string inputdir, // directory with workspace files
				      Double_t mass_low,
				      Double_t mass_high,
				      Double_t mass_step,
				      Int_t ntoys,
				      std::string suffix,
				      Double_t fit_range_low,
				      Double_t fit_range_high,
				      bool add_fake_data,
				      Double_t width_factor );
  
  Double_t DileptonRatioSignificanceRobustness( std::string inputdir, // directory with workspace files
						std::string channel,
						Double_t mass_low,
						Double_t mass_high,
						Double_t mass_step,
						std::string option = "low_range" );
  
  Int_t FixVariables( std::set<std::string> par);

  Int_t AddWorkspace(std::string filename,
		     std::string ws_name,
		     std::string channel_name,
		     std::string shared_vars);

  Int_t SetChannelData(std::string channel_name,
		       RooAbsData * dataset);

  Int_t SetSharedParameters( Float_t peak );
  Int_t SetDimuonRatioParameters( Float_t peak );
  Int_t SetDielectronRatioParameters( Float_t peak );
  Int_t SetDileptonRatioParameters( std::string channel,
				    std::string option = "" );
  Int_t SetDimuonParameters( void );
  Int_t SetDielectronParameters( void );
  Int_t SetDiphotonParameters( Float_t mass, Float_t kmpl );

  Int_t SetDiphotonData( Float_t mass, Float_t kmpl );
  RooAbsData * createThreeChannelObservedData( void );
  RooAbsData * createDileptonObservedData( void );

  Int_t CreateDiphotonToyMc( Float_t peak, Float_t kmpl );
  Int_t CreateDimuonToyMc( void );
  Int_t CreateDielectronToyMc( void );
  RooAbsData * CreateDileptonToyMc( void );
  RooAbsData * CreateThreeChannelToyMc( void );

  Int_t CreateDimuonPeData( Int_t nev );
  Int_t CreateDielectronPeData( Int_t nev );
  RooAbsData * CreateDileptonPeData( Int_t n_ee, Int_t n_mumu );

  RooAbsData * CreateScaledData( std::string channel,
				 Double_t cutoff,
				 Double_t scale,
				 std::vector<Double_t> events);

  Int_t Diagnostics( void );

  ModelConfig prepareDimuonRatioModel( std::string inputdir );
  ModelConfig prepareDileptonRatioModel_old( std::string inputdir );
  ModelConfig prepareDileptonRatioModel( std::string inputdir,
					 std::string channel );
  ModelConfig prepareThreeChannelModel( std::string inputdir );

  MCMCInterval * GetMcmcInterval(ModelConfig mc,
				 double conf_level,
				 int n_iter,
				 int n_burn,
				 double left_side_tail_fraction,
				 int n_bins);

  void makeMcmcPosteriorPlot( std::string filename );

  LikelihoodInterval * GetPlrInterval( double conf_level );

  double printMcmcUpperLimit( double peak, std::string filename = "" );
  void printDileptonRatioSignificance( std::string filename,
				       Double_t peak,
				       Double_t pllr);

  Double_t GetKfactorUncertainty( Float_t peak, std::string model = "cteq" );

  RooWorkspace * GetWorkspace( void ){ return ws; }
  Double_t GetRandom( std::string pdf, std::string var );

  Int_t PrintToFile( std::string filename,
		     std::vector<Double_t> entries,
		     std::string outputLegend = "Some numbers");

  void PlotData( std::string channel );


private:

  Int_t AddPseudoData( std::string channel,
		       std::string pdfName,
		       double nev,
		       double peak,
		       bool poisson );
  
  Double_t GetPoiUpper(std::string channel, Double_t peak);

  Double_t get_pllr( std::string null_parameters_name );
  std::pair<Double_t, Double_t> get_pllr_max( Double_t mass_low,
					      Double_t mass_high,
					      Double_t mass_step,
					      std::string fit_plot_file_name = "",
					      std::string null_parameters_name = "NullParameters");

  std::set<std::string> channels;
  std::map<std::string, std::string> mFiles; // input workspace files
  std::map<std::string, std::string> mShared; // names of variables shared in multiple channels
  std::map<std::string, RooDataSet *> mData;
  //std::map<std::string, Double_t> mLumi;
  std::map<std::string, RooAbsPdf *> models;

  std::ofstream logfile;

  RooCategory * channel_cat;
  RooAbsData * data;
  RooAbsPdf * model;
  RooArgSet * poi;
  RooArgSet * nuis;
  RooAbsPdf * prior;
  RooStats::ModelConfig mc;
  RooStats::ModelConfig * mpMc;
  RooWorkspace * ws;

  // roostats calculators results
  LikelihoodInterval * mpPlrInt;
  MCMCInterval * mcInt;

  // random numbers
  TRandom3 r;
};


TwoBody::TwoBody(){

  std::string legend = "[TwoBody::TwoBody]: ";

  ws = new RooWorkspace("ws");
  mc.SetName("modelconfig");
  mc.SetTitle("model_config");
  channel_cat = new RooCategory("channels", "channels");

  data = 0;
  model = 0;
  poi = 0;
  nuis = 0;
  prior = 0;

  mcInt = 0;
  mpPlrInt = 0;
  mpMc = 0;

  // set random seed
  r.SetSeed();
  UInt_t _seed = r.GetSeed();
  UInt_t _pid = gSystem->GetPid();
  std::cout << legend << "Random seed: " << _seed << std::endl;
  std::cout << legend << "PID: " << _pid << std::endl;
  _seed = 31*_seed+_pid;
  std::cout << legend << "New random seed (31*seed+pid): " << _seed << std::endl;
  r.SetSeed(_seed);

  // set RooFit random seed (it has a private copy)
  RooRandom::randomGenerator()->SetSeed(_seed);

  // open log file
  logfile.open("twobody.log");
}


TwoBody::~TwoBody(){
  
  logfile.close();

  delete channel_cat;
  delete ws;
  delete data;
  delete model;
  delete poi;
  delete nuis;
  delete prior;
  delete mcInt;
  delete mpPlrInt;
  delete mpMc;
}



Int_t TwoBody::PrintToFile( std::string filename,
			    std::vector<Double_t> entries,
			    std::string outputLegend){
  //
  // print out numbers from the vector to file, append if the file exists
  //

  if (filename.size()!=0){
    
    std::ofstream aFile;

    // append to file if exists
    aFile.open(filename.c_str(), std::ios_base::app);

    if (aFile.tellp() < 1){
      aFile << outputLegend << std::endl;
      aFile << "#------------------------------------------------------------" << std::endl;
    }

    char buf[1024];
    std::string str_buf = "";
    for (std::vector<Double_t>::const_iterator num = entries.begin();
	 num != entries.end();
	 ++num){
      sprintf(buf, "%15.6f", *num);
      str_buf.append(buf);
    }

    
    aFile << str_buf << std::endl;

    // close outfile here so it is safe even if subsequent iterations crash
    aFile.close();

  }

  return 0;
}



LikelihoodInterval * 
TwoBody::GetPlrInterval( double conf_level ){
  //
  // Profile likelihood ratio interval calculations
  //

  delete mpPlrInt;
  
  mpMc->Print();

  ProfileLikelihoodCalculator plc(*data, *mpMc);
  plc.SetConfidenceLevel(conf_level);
  mpPlrInt = plc.GetInterval();

  return mpPlrInt;
}



Double_t TwoBody::GetPoiUpper(std::string channel, Double_t peak){
  //
  // Estimate a good value for the upper boundary of the range of POI
  //

  Double_t result = 0.1;

  // default - old crude estimate
  //result = -0.00006*peak + 0.14
  result = 195.235*pow(peak,-1.2375);

  // estimate limit with profile likelihood ratio and
  // set the range to 5 times the limit
  GetPlrInterval(0.95);
  
  // query intervals
  RooFit::MsgLevel msglevel = RooMsgService::instance().globalKillBelow();
  RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);
  RooRealVar * _poi = (RooRealVar *)mpMc->GetParametersOfInterest()->first();
  Double_t upper_limit = mpPlrInt->UpperLimit( *_poi );
  RooMsgService::instance().setGlobalKillBelow(msglevel);
  
  result = 5.0*upper_limit;

  if (channel.find("dimuon")!=std::string::npos){
    return result;
  }

  else{
    return result;
  }
}



Int_t TwoBody::SetChannelData(std::string channel_name,
			      RooAbsData * dataset){
  //
  // delete a dataset that is pointed to by mData for this channel,
  // make a copy of the dataset that is pointed to by the argument,
  // replace (or add if new) the pointer to the copied dataset in mData
  //

  if ( mData.find(channel_name) != mData.end() ){
    delete mData[channel_name];
  }

  RooDataSet * _data = new RooDataSet( *(RooDataSet *)dataset );
  mData[channel_name.c_str()] = _data;

  return 0;
}



Int_t TwoBody::AddWorkspace(std::string filename,
			    std::string ws_name,
			    std::string channel_name,
			    std::string shared_vars){
  //
  // Load a single channel model and data from a workspace
  //

  channels.insert(channel_name);
  channel_cat->defineType(channel_name.c_str());
  mFiles[channel_name] = filename;
  mShared[channel_name] = shared_vars;

  // load workspace from a file
  TFile _file(filename.c_str(), "read");
  RooWorkspace * _ws = (RooWorkspace *)_file.Get( ws_name.c_str() );

  // get the signle channel model PDF
  RooAbsPdf * _model = _ws->pdf("model");

  // import the channel model PDF into the combined workspace
  // add prefix channel_name to all nodes except shared_vars
  ws->import( *_model,
  	      RooFit::RenameAllNodes( channel_name.c_str() ),
  	      RooFit::RenameAllVariablesExcept(channel_name.c_str(), shared_vars.c_str()) );

  // load the channel data from the single-channel workspace
  RooAbsData * _absdata = _ws->data("data");
  SetChannelData(channel_name, _absdata);
  _file.Close();

  return 0;
}



Int_t TwoBody::Diagnostics( void ){
  //
  // Print out some workspace content info
  //

  RooArgSet _vars = ws->allVars();

  TIterator * iter = _vars.createIterator();

  std::cout << std::endl;
  std::cout << "Important model parameters" << std::endl;
  std::cout << "---------------------------" << std::endl;
  ws->var("peak")->Print();
  ws->var("lumi_dimuon")->Print();
  ws->var("lumi_dielectron")->Print();
  ws->var("lumi_diphoton")->Print();
  ws->var("nbkg_est_dimuon")->Print();
  ws->var("nbkg_est_dielectron")->Print();
  ws->var("bkg_est_diphoton")->Print();
  ws->function("width_dimuon")->Print();
  ws->function("width_dielectron")->Print();
  ws->function("sigma_dimuon")->Print();
  ws->function("sigma_dielectron")->Print();


  std::cout << std::endl;
  std::cout << "Non-constant variables in the workspace" << std::endl;
  std::cout << "----------------------------------------" << std::endl;
  for(TObject * _obj = iter->Next(); _obj; _obj = iter->Next() ){
    std::string _name = _obj->GetName();
    RooRealVar * _var = (RooRealVar *)( _vars.find(_name.c_str()) );
    if (_var->isConstant() == kFALSE) _var->Print();
  }
  std::cout << std::endl;

  delete iter;

  return 0;
}



Int_t TwoBody::FixVariables( std::set<std::string> par ){
  //
  // Set all RooRealVars except <par> to be constants 
  //

  Int_t _fixed = 0;

  RooArgSet _vars = ws->allVars();

  TIterator * iter = _vars.createIterator();

  for(TObject * _obj = iter->Next(); _obj; _obj = iter->Next() ){
    std::string _name = _obj->GetName();
    if (par.find(_name) == par.end()){
      RooRealVar * _var = (RooRealVar *)( _vars.find(_name.c_str()) );
      _var->setConstant(kTRUE);
      ++_fixed;
    }
  }
  delete iter;

  return _fixed;
}



Int_t TwoBody::SetSharedParameters( Float_t peak ){
  //
  // set all parameter values and ranges
  // that are shared across the channels
  //

  ws->var("peak")->setVal(peak);

  return 0;
}



Int_t TwoBody::SetDimuonRatioParameters( Float_t peak ){
  //
  // set parameter values and ranges
  // for the dimuon ratio search
  //

  ws->var("peak")->setVal(peak);
  Float_t _sigma = ws->var("nsig_kappa_dimuon")->getVal() - 1.0;
  Float_t _low = std::max(0.0, 1.0-5.0*_sigma);
  Float_t _high = 1.0+5.0*_sigma;
  ws->var("nsig_nuis_dimuon")->setRange(_low, _high);

  // FIXME: deal with nbkg prior
  //_sigma = ws->var("nbkg_kappa_dimuon")->getVal() - 1.0;
  //_low = std::max(0.0, 1.0-5.0*_sigma);
  //_high = 1.0+5.0*_sigma;
  //ws->var("nbkg_nuis_dimuon")->setRange(_low, _high);

  return 0;
}



Int_t TwoBody::SetDielectronRatioParameters( Float_t peak ){
  //
  // set parameter values and ranges
  // for the dimuon ratio search
  //

  ws->var("peak")->setVal(peak);
  Float_t _sigma = ws->var("nsig_kappa_dielectron")->getVal() - 1.0;
  Float_t _low = std::max(0.0, 1.0-5.0*_sigma);
  Float_t _high = 1.0+5.0*_sigma;
  ws->var("nsig_nuis_dielectron")->setRange(_low, _high);

  // FIXME: deal with nbkg prior
  //_sigma = ws->var("nbkg_kappa_dielectron")->getVal() - 1.0;
  //_low = std::max(0.0, 1.0-5.0*_sigma);
  //_high = 1.0+5.0*_sigma;
  //ws->var("nbkg_nuis_dielectron")->setRange(_low, _high);

  _sigma = ws->var("mass_kappa_dielectron")->getVal() - 1.0;
  _low = std::max(0.0, 1.0-5.0*_sigma);
  _high = 1.0+5.0*_sigma;
  ws->var("mass_nuis_dielectron")->setRange(_low, _high);

  return 0;
}



Int_t TwoBody::SetDimuonParameters( void ){
  //
  // set all di-muon channel parameter values and ranges
  //

  Float_t _sigma = ws->var("nsig_kappa_dimuon")->getVal() - 1.0;
  Float_t _low = std::max(0.0, 1.0-5.0*_sigma);
  Float_t _high = 1.0+5.0*_sigma;
  ws->var("nsig_nuis_dimuon")->setRange(_low, _high);

  _sigma = ws->var("nbkg_kappa_dimuon")->getVal() - 1.0;
  _low = std::max(0.0, 1.0-5.0*_sigma);
  _high = 1.0+5.0*_sigma;
  ws->var("nbkg_nuis_dimuon")->setRange(_low, _high);

  return 0;
}



Int_t TwoBody::SetDielectronParameters( void ){
  //
  // set all di-electron channel parameter values and ranges
  //

  Float_t _sigma = ws->var("nsig_kappa_dielectron")->getVal() - 1.0;
  Float_t _low = std::max(0.0, 1.0-5.0*_sigma);
  Float_t _high = 1.0+5.0*_sigma;
  ws->var("nsig_nuis_dielectron")->setRange(_low, _high);

  _sigma = ws->var("nbkg_kappa_dielectron")->getVal() - 1.0;
  _low = std::max(0.0, 1.0-5.0*_sigma);
  _high = 1.0+5.0*_sigma;
  ws->var("nbkg_nuis_dielectron")->setRange(_low, _high);

  _sigma = ws->var("mass_kappa_dielectron")->getVal() - 1.0;
  _low = std::max(0.0, 1.0-5.0*_sigma);
  _high = 1.0+5.0*_sigma;
  ws->var("mass_nuis_dielectron")->setRange(_low, _high);

  return 0;
}



Int_t TwoBody::SetDiphotonData( Float_t peak, Float_t kmpl ){
  //
  // set di-photon channel dataset
  //

  Float_t _lowmass = LowMass(peak, kmpl);
  Float_t _bkg = Background(_lowmass);
  Int_t _n = Data( _lowmass );

  // create dataset
  RooRealVar * _vn = ws->var("n");
  _vn->setVal(_n);
  _vn->setRange( 0.0, 1.0+_bkg+10.0*(_n+1.0));
  RooDataSet * _data = new RooDataSet( "data", "", *_vn );
  _data->add( *_vn );

  SetChannelData("diphoton", _data);

  delete _data;

  return 0;
}



Int_t TwoBody::SetDiphotonParameters( Float_t peak, Float_t kmpl ){
  //
  // set all di-photon channel parameter values
  //

  Float_t _eff = Efficiency(peak, kmpl);
  Float_t _lowmass = LowMass(peak, kmpl);
  Float_t _bkg = Background(_lowmass);

  Float_t _seff = 0.025;
  //Float_t _sbkg = _bkg * 0.101; // no need to duplicate what's in workspace

  // set values
  ws->var("efficiency_diphoton")->setVal(_eff);
  ws->var("bkg_est_diphoton")->setVal(_bkg);
  ws->var("nsig_kappa_diphoton")->setVal(1.0 + _seff/_eff);
  //ws->var("nbkg_kappa_diphoton")->setVal(1.0 + _sbkg/_bkg);

  // set ranges
  Float_t _sigma = ws->var("nsig_kappa_diphoton")->getVal() - 1.0;
  Float_t _low = std::max(0.0, 1.0-5.0*_sigma);
  Float_t _high = 1.0+5.0*_sigma;

  ws->var("nsig_nuis_diphoton")->setRange(_low, _high);

  _sigma = ws->var("nbkg_kappa_diphoton")->getVal() - 1.0;
  _low = std::max(0.0, 1.0-5.0*_sigma);
  _high = 1.0+5.0*_sigma;

  ws->var("nbkg_nuis_diphoton")->setRange(_low, _high);

  return 0;
}



RooAbsData * TwoBody::createThreeChannelObservedData( void ){
  
  // uses included global functions from Sean Simon
  SetDiphotonData( ws->var("peak")->getVal(), 0.01 );
  
  // create combined dataset
  RooRealVar * mass = ws->var("mass");
  RooRealVar * n = ws->var("n");
  delete data; // the pointer should always be valid, so delete first to be safe
  data = new RooDataSet( "combdata", "combined data",
			 RooArgSet((const RooRealVar)(*mass), (const RooRealVar)(*n) ),
			 Index(*channel_cat),
			 Import(mData) );
  
  data->Print();
  
  return data;
}



RooAbsData * TwoBody::createDileptonObservedData( void ){
  
  // create combined dataset
  RooRealVar * mass = ws->var("mass");
  delete data; // the pointer should always be valid, so delete first to be safe
  data = new RooDataSet( "combdata", "combined data",
			 RooArgSet((const RooRealVar)(*mass) ),
			 Index(*channel_cat),
			 Import(mData) );
  
  data->Print();
  
  return data;
}



Int_t TwoBody::CreateDiphotonToyMc( Float_t peak, Float_t kmpl ){
  //
  // generate a toy di-photon channel dataset with systematics
  // set mData to point to it
  //

  Float_t _lowmass = LowMass(peak, kmpl);
  Float_t _bkg = Background(_lowmass);

  // generate expected number of events from its uncertainty
  RooDataSet * _ds = ws->pdf("syst_nbkg_diphoton")->generate(*ws->var("nbkg_nuis_diphoton"), 1);
  Double_t _ntoy = ((RooRealVar *)(_ds->get(0)->first()))->getVal() * _bkg;
  delete _ds;

  Int_t _n = r.Poisson(_ntoy);

  // create dataset
  RooRealVar * _vn = ws->var("n");
  _vn->setVal(_n);
  _vn->setRange( 0.0, 1.0+_bkg+10.0*(_n+1.0));
  RooDataSet * _data = new RooDataSet( "data", "", *_vn );
  _data->add( *_vn );

  SetChannelData("diphoton", _data);

  Int_t _result = (Int_t)(_data->sumEntries());

  delete _data;

  return _result;
}



Double_t TwoBody::GetRandom( std::string pdf, std::string var ){
  //
  // generates a random number using a pdf in the workspace
  //
  
  // generate a dataset with one entry
  RooDataSet * _ds = ws->pdf(pdf.c_str())->generate(*ws->var(var.c_str()), 1);

  Double_t _result = ((RooRealVar *)(_ds->get(0)->first()))->getVal();
  delete _ds;

  return _result;
}



Int_t TwoBody::CreateDimuonToyMc( void ){
  //
  // generate a toy di-muon dataset with systematics
  // set mData accordingly
  //

  // generate expected number of events from its uncertainty
  //RooDataSet * _ds = ws->pdf("syst_nbkg_dimuon")->generate(*ws->var("nbkg_nuis_dimuon"), 1);
  //Double_t _ntoy = ((RooRealVar *)(_ds->get(0)->first()))->getVal() * (ws->var("nbkg_est_dimuon")->getVal());
  //delete _ds;

  Double_t _ntoy = GetRandom("syst_nbkg_dimuon", "nbkg_nuis_dimuon") * (ws->var("nbkg_est_dimuon")->getVal());

  Int_t _n = r.Poisson(_ntoy);

  // all nuisance parameters:
  //   nsig_nuis_dimuon, 
  //   nbkg_nuis_dimuon,
  //   lumi_nuis

  // create dataset
  RooRealVar * _mass = ws->var("mass");
  RooArgSet _vars(*_mass);

  RooAbsPdf * _pdf = ws->pdf("bkgpdf_dimuon");

  RooAbsPdf::GenSpec * _spec = _pdf->prepareMultiGen(_vars,
						     Name("toys"),
						     NumEvents(_n),
						     Extended(kFALSE),
						     Verbose(kTRUE));
  //RooDataSet * _data = _pdf->generate(*_spec);
  delete data;
  data = _pdf->generate(*_spec); // class member

  delete _spec;

  SetChannelData("dimuon", data);

  Int_t n_generated_entries = (Int_t)(data->sumEntries());

  //delete _data;

  // debug
  std::cout << "!!!!!!!!!!!!!! _ntoy   = " << _ntoy << std::endl;
  std::cout << "!!!!!!!!!!!!!! _n      = " << _n    << std::endl;
  std::cout << "!!!!!!!!!!!!!! n_generated_entries = " << n_generated_entries    << std::endl;

  return n_generated_entries;
}



Int_t TwoBody::CreateDimuonPeData( Int_t nev ){
  //
  // generate a di-muon pseudo-data set
  // set mData accordingly
  //

  Int_t _n = nev;

  // create dataset
  RooRealVar * _mass = ws->var("mass");
  RooArgSet _vars(*_mass);

  RooAbsPdf * _pdf = ws->pdf("bkgpdf_dimuon");

  RooAbsPdf::GenSpec * _spec = _pdf->prepareMultiGen(_vars,
						     Name("toys"),
						     NumEvents(_n),
						     Extended(kFALSE),
						     Verbose(kTRUE));
  //RooDataSet * _data = _pdf->generate(*_spec);
  delete data;
  data = _pdf->generate(*_spec); // class member

  delete _spec;

  SetChannelData("dimuon", data);

  Int_t n_generated_entries = (Int_t)(data->sumEntries());

  //delete _data;

  // debug
  //std::cout << "!!!!!!!!!!!!!! _n      = " << _n    << std::endl;
  //std::cout << "!!!!!!!!!!!!!! _result = " << _result    << std::endl;

  return n_generated_entries;
}



Int_t TwoBody::CreateDielectronToyMc( void ){
  //
  // generate a toy di-electron dataset with systematics
  // set mData accordingly
  //

  Double_t _ntoy = GetRandom("syst_nbkg_dielectron", "nbkg_nuis_dielectron") * (ws->var("nbkg_est_dielectron")->getVal());

  Int_t _n = r.Poisson(_ntoy);

  // all nuisance parameters:
  //   nsig_nuis_dielectron, 
  //   nbkg_nuis_dielectron,
  //   mass_nuis_dielectron,
  //   lumi_nuis

  // create dataset
  RooRealVar * _mass = ws->var("mass");
  RooArgSet _vars(*_mass);

  RooAbsPdf * _pdf = ws->pdf("bkgpdf_dielectron");

  RooAbsPdf::GenSpec * _spec = _pdf->prepareMultiGen(_vars,
						     Name("toys"),
						     NumEvents(_n),
						     Extended(kFALSE),
						     Verbose(kTRUE));
  //RooDataSet * _data = _pdf->generate(*_spec);
  delete data;
  data = _pdf->generate(*_spec); // owned by the class

  delete _spec;

  SetChannelData("dielectron", data);

  Int_t n_generated_entries = (Int_t)(data->sumEntries());

  //delete _data;

  // debug
  std::cout << "!!!!!!!!!!!!!! _ntoy   = " << _ntoy << std::endl;
  std::cout << "!!!!!!!!!!!!!! _n      = " << _n    << std::endl;
  std::cout << "!!!!!!!!!!!!!! n_generated_entries = " << n_generated_entries    << std::endl;

  return n_generated_entries;
}



Int_t TwoBody::CreateDielectronPeData( Int_t nev ){
  //
  // generate a di-electron pseudo-data set
  // set mData accordingly
  //

  Int_t _n = nev;

  // create dataset
  RooRealVar * _mass = ws->var("mass");
  RooArgSet _vars(*_mass);

  RooAbsPdf * _pdf = ws->pdf("bkgpdf_dielectron");

  RooAbsPdf::GenSpec * _spec = _pdf->prepareMultiGen(_vars,
						     Name("toys"),
						     NumEvents(_n),
						     Extended(kFALSE),
						     Verbose(kTRUE));
  //RooDataSet * _data = _pdf->generate(*_spec);
  delete data;
  data = _pdf->generate(*_spec); // owned by the class

  delete _spec;

  SetChannelData("dielectron", data);

  Int_t n_generated_entries = (Int_t)(data->sumEntries());

  //delete _data;

  // debug
  //std::cout << "!!!!!!!!!!!!!! _n      = " << _n    << std::endl;
  //std::cout << "!!!!!!!!!!!!!! _result = " << _result    << std::endl;

  return n_generated_entries;
}



RooAbsData * TwoBody::CreateThreeChannelToyMc( void ){
  
  // uses included global functions from Sean Simon
  CreateDiphotonToyMc( ws->var("peak")->getVal(), 0.01 );
  CreateDimuonToyMc();
  CreateDielectronToyMc();
  
  // create combined dataset
  RooRealVar * mass = ws->var("mass");
  RooRealVar * n = ws->var("n");
  delete data; // the pointer should always be valid, so delete first to be safe
  data = new RooDataSet( "combdata", "combined data",
			 RooArgSet((const RooRealVar)(*mass), (const RooRealVar)(*n) ),
			 Index(*channel_cat),
			 Import(mData) );
  
  data->Print();
  
  return data;
}



RooAbsData * TwoBody::CreateDileptonToyMc( void ){
  
  CreateDimuonToyMc();
  CreateDielectronToyMc();
  
  // create combined dataset
  RooRealVar * mass = ws->var("mass");
  delete data; // the pointer should always be valid, so delete first to be safe
  data = new RooDataSet( "combdata", "combined data",
			 RooArgSet((const RooRealVar)(*mass) ),
			 Index(*channel_cat),
			 Import(mData) );
  
  data->Print();
  
  return data;
}



RooAbsData * TwoBody::CreateDileptonPeData( Int_t n_ee, Int_t n_mumu ){
  //
  // Creates background-only pseudodata 
  // which mimics the observed data.
  // Number of events exactly matches the observed number
  //
  
  CreateDimuonPeData(n_mumu);
  CreateDielectronPeData(n_ee);
  
  // create combined dataset
  RooRealVar * mass = ws->var("mass");
  delete data; // the pointer should always be valid, so delete first to be safe
  data = new RooDataSet( "combdata", "combined data",
			 RooArgSet( (const RooRealVar)(*mass) ),
			 Index(*channel_cat),
			 Import(mData) );
  
  data->Print();
  
  return data;
}



MCMCInterval * TwoBody::GetMcmcInterval(ModelConfig mc,
					double conf_level,
					int n_iter,
					int n_burn,
					double left_side_tail_fraction,
					int n_bins){
  // use MCMCCalculator  (takes about 1 min)
  // Want an efficient proposal function, so derive it from covariance
  // matrix of fit

  /*  
  RooFitResult* fit = ws->pdf("model")->fitTo(*data,Save());
  ProposalHelper ph;
  ph.SetVariables((RooArgSet&)fit->floatParsFinal());
  ph.SetCovMatrix(fit->covarianceMatrix());
  ph.SetUpdateProposalParameters(kTRUE); // auto-create mean vars and add mappings
  ph.SetCacheSize(100);
  ProposalFunction* pf = ph.GetProposalFunction();
  */

  // FIXME: testing: this proposal function seems fairly robust
  SequentialProposal sp(10.0);

  MCMCCalculator mcmc( *data, mc );
  mcmc.SetConfidenceLevel(conf_level);
  mcmc.SetNumIters(n_iter);          // Metropolis-Hastings algorithm iterations

  // FIXME: testing: different proposal function
  //mcmc.SetProposalFunction(*pf);
  mcmc.SetProposalFunction(sp);

  mcmc.SetNumBurnInSteps(n_burn); // first N steps to be ignored as burn-in
  mcmc.SetLeftSideTailFraction(left_side_tail_fraction);
  mcmc.SetNumBins(n_bins);
  
  // FIXME: testing good initial values - don't seem to do anything different
  //ws->var("ratio")->setVal(0.01);
  //ws->var("nsig_nuis_dielectron")->setVal(1.0);

//mcInt = mcmc.GetInterval();
  try {
    mcInt = mcmc.GetInterval();
  } catch ( std::length_error &ex) {
    mcInt = 0;
  }

  //std::cout << "!!!!!!!!!!!!!! interval" << std::endl;
  if (mcInt == 0) std::cout << "No interval found!" << std::endl;
  
  return mcInt;
}



void TwoBody::makeMcmcPosteriorPlot( std::string filename ){

  if (mcInt){
    TCanvas c1("c1");
    MCMCIntervalPlot plot(*mcInt);
    plot.Draw();
    c1.SaveAs(filename.c_str());
    // test scatter
    //TCanvas c2("c2");
    //plot.DrawChainScatter(*ws->var("ratio"),*ws->var("nsig_nuis_dielectron"));
    //c2.SaveAs("scatter_mcmc.png");
  }
  
  return;
}



double TwoBody::printMcmcUpperLimit( double peak, std::string filename ){
  //
  // print out the upper limit on the first Parameter of Interest
  //

  RooRealVar * firstPOI = (RooRealVar*) mc.GetParametersOfInterest()->first();
  double _limit = mcInt->UpperLimit(*firstPOI);
  cout << "\n95% upper limit on " <<firstPOI->GetName()<<" is : "<<
    _limit <<endl;

  if (filename.size()!=0){
    
    std::ofstream aFile;

    // append to file if exists
    aFile.open(filename.c_str(), std::ios_base::app);

    char buf[1024];
    sprintf(buf, "%7.1f   %7.6f", peak, _limit);

    aFile << buf << std::endl;

    // close outfile here so it is safe even if subsequent iterations crash
    aFile.close();

  }

  return _limit;
}



void TwoBody::printDileptonRatioSignificance( std::string filename,
					      Double_t peak,
					      Double_t pllr){
  //
  // print out the test statistic and the corresponding resonance mass
  //

  if (filename.size()!=0){
    
    std::ofstream aFile;

    // append to file if exists
    aFile.open(filename.c_str(), std::ios_base::app);

    char buf[1024];
    sprintf(buf, "%7.1f   %7.6f", peak, pllr);

    aFile << buf << std::endl;

    // close outfile here so it is safe even if subsequent iterations crash
    aFile.close();

  }

  return;
}



ModelConfig TwoBody::prepareDimuonRatioModel( std::string inputdir ){
  //
  // prepare workspace and ModelConfig for the dimuon xsec ratio limit
  //
  std::string _infile = inputdir+"ws_dimuon_ratio.root";
  AddWorkspace(_infile.c_str(),
	       "myWS",
	       "dimuon",
	       "peak,mass,ratio");

  ws->pdf("model_dimuon")->SetName("model");

  // set all vars to const except <par>
  std::set<std::string> par;
  par.insert("mass");
  par.insert("ratio");
  par.insert("nsig_nuis_dimuon");  
  par.insert("nbkg_nuis_dimuon"); 
  FixVariables(par); 
  
  // POI
  RooArgSet sPoi( *(ws->var("ratio")) );

  // nuisance
  RooArgSet sNuis( *(ws->var("nsig_nuis_dimuon")),
		   *(ws->var("nbkg_nuis_dimuon"))
		   );

  // observables
  RooArgSet sObs( *(ws->var("mass")) );

  // prior
  ws->factory( "Uniform::prior_ratio(ratio)" );  
  ws->factory( "Lognormal::syst_nbkg_dimuon(nbkg_nuis_dimuon,nom_nbkg_nuis_dimuon[1.0,0.1,1.9],nbkg_kappa_dimuon[1.20])" );
  ws->factory( "PROD::prior(prior_ratio,syst_nbkg_dimuon)" );  

  // ModelConfig
  mc.SetWorkspace(*ws);
  mc.SetPdf(*(ws->pdf("model")));
  mc.SetParametersOfInterest( sPoi );
  mc.SetPriorPdf( *(ws->pdf("prior")) );
  mc.SetNuisanceParameters( sNuis );
  mc.SetObservables( sObs );

  //ws->import(mc);

  return mc;
}


ModelConfig TwoBody::prepareDileptonRatioModel_old( std::string inputdir ){
  //
  // prepare workspace and ModelConfig for the combined dilepton xsec ratio limit
  //
  std::string _infile = inputdir+"ws_dimuon_ratio.root";
  //std::string _infile = inputdir+"ws_dimuon_ratio_addflat.root";
  //std::string _infile = inputdir+"ws_dimuon_ratio_swap.root";
  //std::string _infile = inputdir+"ws_dimuon_ratio_flatbg.root";
  //std::string _infile = inputdir+"myWS_mu700.root";
  //std::string _infile = inputdir+"ws_dimuon_ratio_30may2011.root";
  AddWorkspace(_infile.c_str(),
	       "myWS",
	       "dimuon",
	       "peak,mass,ratio,width_p0,width_p1,width");
  _infile = inputdir+"ws_dielectron_ratio.root";
  //_infile = inputdir+"ws_dielectron_ratio_addflat.root";
  //_infile = inputdir+"ws_dielectron_ratio_swap.root";
  //_infile = inputdir+"ws_dielectron_ratio_flatbg.root";
  //_infile = inputdir+"myWS_e700.root";
  AddWorkspace(_infile.c_str(),
	       "myWS",
	       "dielectron",
	       "peak,mass,ratio,width_p0,width_p1,width");

  RooSimultaneous model("model", "model", *channel_cat);
  model.addPdf(*ws->pdf("model_dimuon"), "dimuon");
  model.addPdf(*ws->pdf("model_dielectron"), "dielectron");
  ws->import(model, RecycleConflictNodes());

  // set all vars to const except <par>
  std::set<std::string> par;
  par.insert("mass");
  par.insert("ratio");
  par.insert("nsig_nuis_dimuon");  
  par.insert("nsig_nuis_dielectron");
  par.insert("nbkg_nuis_dimuon"); 
  par.insert("nbkg_nuis_dielectron");
  par.insert("mass_nuis_dielectron");
  par.insert("fbkg_flat_dimuon");
  par.insert("fbkg_flat_dielectron");
  FixVariables(par); 

  // POI
  RooArgSet sPoi( *(ws->var("ratio")) );

  // nuisance
  RooArgSet sNuis( *(ws->var("nsig_nuis_dimuon")),
		   *(ws->var("nsig_nuis_dielectron")),
		   // *(ws->var("nbkg_nuis_dimuon")),
		   // *(ws->var("nbkg_nuis_dielectron")),
		   // *(ws->var("mass_nuis_dimuon")),
		   *(ws->var("mass_nuis_dielectron"))
		    );

  // global observables
  RooArgSet sGlobalObs( *(ws->var("nsig_nuis_dimuon")),
		   *(ws->var("nsig_nuis_dielectron")),
		   // *(ws->var("nbkg_nuis_dimuon")),
		   // *(ws->var("nbkg_nuis_dielectron")),
		   // *(ws->var("mass_nuis_dimuon")),
		   *(ws->var("mass_nuis_dielectron"))
		    );

  // observables
  RooArgSet sObs( *(ws->var("mass")) );

  // prior
  ws->factory( "Uniform::prior(ratio)" );  

  // ModelConfig
  mc.SetWorkspace(*ws);
  mc.SetPdf(*(ws->pdf("model")));
  mc.SetParametersOfInterest( sPoi );
  mc.SetPriorPdf( *(ws->pdf("prior")) );
  mc.SetNuisanceParameters( sNuis );
  mc.SetObservables( sObs );

  ws->import(mc);

  return mc;
}


ModelConfig TwoBody::prepareDileptonRatioModel( std::string inputdir,
						std::string channel ){
  //
  // Prepare workspace and ModelConfig for the combined dilepton xsec ratio limit.
  // Now for the individual channels as well, specified by <channel>
  //

  // Based on <channel> figure out which channels need to be initialized and run
  bool do_dimuon = false;
  bool do_dielectron = false;
  int n_channels = 0; // this is safety for future extra channels
  if (channel.find("dilepton")!=std::string::npos){
    do_dimuon = true;
    do_dielectron = true;
    n_channels += 2;
  }
  else if (channel.find("dimuon")!=std::string::npos){
    do_dimuon = true;
    ++n_channels;
  }
  else if (channel.find("dielectron")!=std::string::npos){
    do_dielectron = true;
    ++n_channels;
  }

  // import workspaces from individual files
  std::string _infile;
  if (do_dimuon){
    _infile = inputdir+"ws_dimuon_ratio.root";
    //std::string _infile = inputdir+"ws_dimuon_ratio_addflat.root";
    //std::string _infile = inputdir+"ws_dimuon_ratio_swap.root";
    //std::string _infile = inputdir+"ws_dimuon_ratio_flatbg.root";
    //std::string _infile = inputdir+"myWS_mu700.root";
    //std::string _infile = inputdir+"ws_dimuon_ratio_30may2011.root";
    AddWorkspace(_infile.c_str(),
		 "myWS",
		 "dimuon",
		 "peak,mass,ratio,width_p0,width_p1,width");
  }

  if (do_dielectron){
    _infile = inputdir+"ws_dielectron_ratio.root";
    //_infile = inputdir+"ws_dielectron_ratio_addflat.root";
    //_infile = inputdir+"ws_dielectron_ratio_swap.root";
    //_infile = inputdir+"ws_dielectron_ratio_flatbg.root";
    //_infile = inputdir+"myWS_e700.root";
    AddWorkspace(_infile.c_str(),
		 "myWS",
		 "dielectron",
		 "peak,mass,ratio,width_p0,width_p1,width");
  }

  // specify the main model PDF
  //
  // combination of dimuon and dielectron
  if (do_dimuon && do_dielectron && n_channels == 2){
    RooSimultaneous model("model", "model", *channel_cat);
    model.addPdf(*ws->pdf("model_dimuon"), "dimuon");
    model.addPdf(*ws->pdf("model_dielectron"), "dielectron");
    ws->import(model, RecycleConflictNodes());
  }
  // dimuon only channel
  else if (do_dimuon && n_channels == 1){
    ws->pdf("model_dimuon")->SetName("model");
  }
  // dielectron only channel
  else if (do_dielectron && n_channels == 1){
    ws->pdf("model_dielectron")->SetName("model");
  }

  // prior
  std::string prior_constructor = 
    "PROD::prior(prior_ratio";
  ws->factory( "Uniform::prior_ratio(ratio)" );  
  if (do_dimuon){
    ws->factory( "Lognormal::syst_nbkg_dimuon(nbkg_nuis_dimuon,nom_nbkg_nuis_dimuon[1.0,0.1,1.9],nbkg_kappa_dimuon[1.20])" );
    prior_constructor += ",syst_nbkg_dimuon";
  }
  if (do_dielectron){
    ws->factory( "Lognormal::syst_nbkg_dielectron(nbkg_nuis_dielectron,nom_nbkg_nuis_dielectron[1.0,0.1,1.9],nbkg_kappa_dielectron[1.20])" );
    prior_constructor += ",syst_nbkg_dielectron";
  }
  prior_constructor += ")";
  //ws->factory( "PROD::prior(prior_ratio,syst_nbkg_dimuon,syst_nbkg_dielectron)" );  
  ws->factory( prior_constructor.c_str() );  

  // set various model parameters: values, ranges, basically
  // anything that needs to change from what was loaded with
  // the workspaces from input files.
  SetDileptonRatioParameters(channel, "");

  // set all vars to const except <par>
  std::set<std::string> par;
  par.insert("mass");
  par.insert("ratio");
  // FIXME: uncomment nuisances - commented out for validation with bump hunter
  if (do_dimuon){
    par.insert("nsig_nuis_dimuon");  
    par.insert("nbkg_nuis_dimuon"); 
    //
    //par.insert("fbkg_flat_dimuon");
  }
  if (do_dielectron){
    par.insert("nsig_nuis_dielectron");
    par.insert("nbkg_nuis_dielectron");
    par.insert("mass_nuis_dielectron");
    //
    //par.insert("fbkg_flat_dielectron");
  }
  FixVariables(par); 

  // POI
  RooArgSet sPoi( *(ws->var("ratio")) );

  // define nuisance parameters
  RooArgSet sNuis;
  if (do_dimuon){
    sNuis.add(*(ws->var("nsig_nuis_dimuon")));
    sNuis.add(*(ws->var("nbkg_nuis_dimuon")));
  }
  if (do_dielectron){
    sNuis.add(*(ws->var("nsig_nuis_dielectron")));
    sNuis.add(*(ws->var("nbkg_nuis_dielectron")));
    sNuis.add( *(ws->var("mass_nuis_dielectron")) );
  }

  // define global observables
  RooArgSet sGlobalObs;
  if (do_dimuon){
    sGlobalObs.add( *(ws->var("nom_nsig_nuis_dimuon")) );
    sGlobalObs.add( *(ws->var("nom_nbkg_nuis_dimuon")) );
  }
  if (do_dielectron){
    sGlobalObs.add( *(ws->var("nom_nsig_nuis_dielectron")) );
    sGlobalObs.add( *(ws->var("nom_nbkg_nuis_dielectron")) );
    sGlobalObs.add( *(ws->var("nom_mass_nuis_dielectron")) );
  }

  // observables
  RooArgSet sObs( *(ws->var("mass")) );

  // ModelConfig
  mc.SetWorkspace(*ws);
  mc.SetPdf(*(ws->pdf("model")));
  mc.SetParametersOfInterest( sPoi );
  mc.SetPriorPdf( *(ws->pdf("prior")) );
  mc.SetNuisanceParameters( sNuis );
  mc.SetObservables( sObs );
  mc.SetGlobalObservables( sGlobalObs );

  ws->import(mc);

  return mc;
}


Int_t TwoBody::SetDileptonRatioParameters( std::string channel,
					   std::string option ){
  //
  // Set model parameter values and such. Generally, most of the
  // values in the workspace are expected to be correct.
  // If something needs to be changed, here is the place to do it.
  //

  // do nothing if explicitely requested
  if (option.find("no_change")!=std::string::npos) return 0;

  // Based on <channel> figure out which channels need to be initialized and run
  bool do_dimuon = false;
  bool do_dielectron = false;
  int n_channels = 0; // this is safety for future extra channels
  if (channel.find("dilepton")!=std::string::npos){
    do_dimuon = true;
    do_dielectron = true;
    n_channels += 2;
  }
  else if (channel.find("dimuon")!=std::string::npos){
    do_dimuon = true;
    ++n_channels;
  }
  else if (channel.find("dielectron")!=std::string::npos){
    do_dielectron = true;
    ++n_channels;
  }
  
  // dimuon parameters
  if (do_dimuon){
    // ranges
    Float_t _sigma = ws->var("nsig_kappa_dimuon")->getVal() - 1.0;
    Float_t _low = std::max(0.0, 1.0-5.0*_sigma);
    Float_t _high = 1.0+5.0*_sigma;
    ws->var("nsig_nuis_dimuon")->setRange(_low, _high);

    // dimuon background shape
    RooRealVar * a_dimuon = ws->var("a_dimuon");
    RooRealVar * b_dimuon = ws->var("b_dimuon");
    if (a_dimuon) a_dimuon->setVal(-0.002207);
    if (b_dimuon) b_dimuon->setVal(-3.774);
    
    // option to double resolution
    if (option.find("resx2")!=std::string::npos){
      
    }
  } // end of dimuon parameters block

  // dielectron parameters
  if (do_dielectron){
    // ranges
    Float_t _sigma = ws->var("nsig_kappa_dielectron")->getVal() - 1.0;
    Float_t _low = std::max(0.0, 1.0-5.0*_sigma);
    Float_t _high = 1.0+5.0*_sigma;
    ws->var("nsig_nuis_dielectron")->setRange(_low, _high);

    _sigma = ws->var("mass_kappa_dielectron")->getVal() - 1.0;
    _low = std::max(0.0, 1.0-5.0*_sigma);
    _high = 1.0+5.0*_sigma;
    ws->var("mass_nuis_dielectron")->setRange(_low, _high);

  } // end of dielectron parameter block

  return 0;
}



ModelConfig TwoBody::prepareThreeChannelModel( std::string inputdir ){

  std::string _infile = inputdir+"ws_dimuon.root";
  AddWorkspace(_infile.c_str(),
	       "myWS",
	       "dimuon",
	       "xsec,mass,peak,width_p0,width_p1,width,lumi_nuis,lumi_kappa");

  _infile = inputdir+"ws_dielectron.root";
  AddWorkspace(_infile.c_str(),
	       "myWS",
	       "dielectron",
	       "xsec,mass,peak,width_p0,width_p1,width,lumi_nuis,lumi_kappa");

  _infile = inputdir+"ws_diphoton.root";
  AddWorkspace(_infile.c_str(),
	       "myWS",
	       "diphoton",
	       "xsec,n,lumi_nuis,lumi_kappa");

  RooSimultaneous model("model", "model", *channel_cat);
  model.addPdf(*ws->pdf("model_dimuon"), "dimuon");
  model.addPdf(*ws->pdf("model_dielectron"), "dielectron");
  model.addPdf(*ws->pdf("model_diphoton"), "diphoton");
  ws->import(model, RecycleConflictNodes());


  // set all vars to const except <par>
  std::set<std::string> par;
  par.insert("mass");
  par.insert("n");
  par.insert("xsec");
  par.insert("nsig_nuis_dimuon");  
  par.insert("nsig_nuis_dielectron");
  par.insert("nsig_nuis_diphoton");
  par.insert("nbkg_nuis_dimuon"); 
  par.insert("nbkg_nuis_dielectron");
  par.insert("nbkg_nuis_diphoton");
  par.insert("mass_nuis_dielectron");
  par.insert("lumi_nuis");
  FixVariables(par); 
  
  // POI
  RooArgSet sPoi( *(ws->var("xsec")) );

  // nuisance
  ws->var("mass_nuis_dimuon")->setConstant(kTRUE);
  RooArgSet sNuis( *(ws->var("nsig_nuis_dimuon")),
		   *(ws->var("nsig_nuis_dielectron")),
		   *(ws->var("nsig_nuis_diphoton")),
		   *(ws->var("nbkg_nuis_dimuon")),
		   *(ws->var("nbkg_nuis_dielectron")),
		   *(ws->var("nbkg_nuis_diphoton")),
		   //*(ws->var("mass_nuis_dimuon")),
		   *(ws->var("mass_nuis_dielectron")),
		   *(ws->var("lumi_nuis"))
		    );

  // observables
  RooArgSet sObs( *(ws->var("mass")),
		  *(ws->var("n")) );

  // prior
  ws->factory( "Uniform::prior(xsec)" );  

  // ModelConfig
  mc.SetWorkspace(*ws);
  mc.SetPdf(*(ws->pdf("model")));
  mc.SetParametersOfInterest( sPoi );
  mc.SetPriorPdf( *(ws->pdf("prior")) );
  mc.SetNuisanceParameters( sNuis );
  mc.SetObservables( sObs );

  //ws->import(mc);

  return mc;
}


void TwoBody::DimuonRatioLimit( Float_t peak,
				std::string mode,      // obsereved, expected, mass limit (extra k-factor uncertainty)
				std::string suffix,    // suffix for output file names
				Int_t ntoys,           // number of pseudoexperiments for expected limit
				Int_t mcmc_iter,
				Int_t mcmc_burnin,
				std::string inputdir){ // directory with workspace files
  //
  // limit on ratio = xsec(Z')/xsec(Z) in dimuon channel
  //

  std::string _legend = "[TwoBody::DimuonRatioLimit]: ";

  ModelConfig mc = prepareDimuonRatioModel(inputdir);  

  // set model parameters and data
  SetDimuonRatioParameters(peak);
  ws->Print();

  // change fit range
  std::cout << _legend << "setting fit range to [200; 2500]" << std::endl;
  ws->var("mass")->setRange(200.0, 2500.0);
  
  // change POI range
  //double poiUpper = GetPoiUpper("dimuon", peak);
  //std::cout << _legend << "setting POI range to [0; " << poiUpper << "]" << std::endl;
  //ws->var("ratio")->setRange(0.0, poiUpper);
  
  int pe_counter = 0;
  std::vector<Double_t> _limits;
  while (pe_counter < ntoys){
    
    // for mass limit, add k-factor systematics to the nsig systematics
    // FIXME: this is a correlated part of the uncertainty!!!
    //  - different uncertainties for graviton and Z' models
    if ( mode.find("mass_") != std::string::npos ){
      
      std::cout << _legend << std::endl;
      std::cout << _legend << "this a mass limit calculation," << std::endl;
      std::cout << _legend << "I will add k-factor uncertainty to the nsig uncertainty" << std::endl;
      std::cout << _legend << "I will do it " << ntoys << " times, so one can average. " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << _legend << std::endl;

      Double_t kfactor_err = GetKfactorUncertainty(peak, mode);

      double nsig_kappa = ws->var("nsig_kappa_dimuon")->getVal();
      nsig_kappa = sqrt(nsig_kappa*nsig_kappa + kfactor_err*kfactor_err);
      ws->var("nsig_kappa_dimuon")->setVal(nsig_kappa);

      //ntoys = 1;
      
    }
    
    else if ( mode.find("expected") != std::string::npos ){
      
      std::cout << _legend << std::endl;
      std::cout << _legend << "this is pseudoexperiment " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << _legend << "for the expected limit estimate" << std::endl;
      std::cout << _legend << std::endl;

      // prepare PE data
      CreateDimuonToyMc();
    }
    
    else{ //  "regular" observed limit
      
      std::cout << _legend << std::endl;
      std::cout << _legend << "calculating an observed limit..." << std::endl;
      std::cout << _legend << "I will do it " << ntoys << " times, so one can average. " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << _legend << std::endl;

      //ntoys = 1;
    }

    // data
    data = new RooDataSet(*(mData["dimuon"]));

    // change POI range
    delete mpMc;
    mpMc = new ModelConfig(mc);
    mpMc->SetWorkspace(*ws);
    double poiUpper = GetPoiUpper("dimuon", peak);
    std::cout << _legend << "setting POI range to [0; " << poiUpper << "]" << std::endl;
    ws->var("ratio")->setRange(0.0, poiUpper);

    mcInt = GetMcmcInterval(mc,
			    0.95,
			    mcmc_iter,
			    mcmc_burnin,
			    0.0,
			    100);
    
    ++pe_counter;

    if (!mcInt){
      continue;
    }
    else{
      
      std::string _outfile = "dimuon_ratio_mcmc_limit_" + suffix + ".ascii";
      printMcmcUpperLimit( peak, _outfile );
      
      // limits for averaging/medianing
      RooRealVar * firstPOI = (RooRealVar*) mc.GetParametersOfInterest()->first();
      double _limit = mcInt->UpperLimit(*firstPOI);
      _limits.push_back(_limit);
      
      //++pe_counter;

    } // end of valid mcInt block

  } // end of while

  // write median limit to a file
  if (_limits.size() > 0){
    Double_t _median_limit = TMath::Median(_limits.size(), &_limits[0]);
    std::vector<Double_t> _mass_limit;
    _mass_limit.push_back(peak);
    _mass_limit.push_back(_median_limit);
    std::string _outfile = "dimuon_ratio_mcmc_median_limit_" + suffix + ".ascii";
    PrintToFile(_outfile, _mass_limit, "#  mass         median_limit");
  }

  std::string _outfile = "dimuon_ratio_mcmc_posterior_" + suffix + ".pdf";
  makeMcmcPosteriorPlot( _outfile );
    
  return;
}


void TwoBody::DielectronRatioLimit( Float_t peak,
				std::string mode,      // obsereved, expected, mass limit (extra k-factor uncertainty)
				std::string suffix,    // suffix for output file names
				Int_t ntoys,           // number of pseudoexperiments for expected limit
				Int_t mcmc_iter,
				Int_t mcmc_burnin,
				std::string inputdir){ // directory with workspace files
  //
  // limit on ratio = xsec(Z')/xsec(Z) in dielectron channel
  //

  std::string _legend = "[TwoBody::DielectronRatioLimit]: ";

  ModelConfig mc = prepareDileptonRatioModel(inputdir, "dielectron");

  // set model parameters and data
  //SetDielectronRatioParameters(peak);
  ws->var("peak")->setVal(peak);
  ws->Print();
  
  // change fit range
  std::cout << _legend << "setting fit range to [200; 2500]" << std::endl;
  ws->var("mass")->setRange(200.0, 2500.0);

  // change POI range
  //double poiUpper = GetPoiUpper("dielectron", peak);
  //std::cout << _legend << "setting POI range to [0; " << poiUpper << "]" << std::endl;
  //ws->var("ratio")->setRange(0.0, poiUpper);
  
  int pe_counter = 0;
  std::vector<Double_t> _limits;
  while (pe_counter < ntoys){
    
    // for mass limit, add k-factor systematics to the nsig systematics
    // FIXME: this is a correlated part of the uncertainty!!!
    //  - different uncertainties for graviton and Z' models
    if ( mode.find("mass_") != std::string::npos ){
      
      std::cout << _legend << std::endl;
      std::cout << _legend << "this a mass limit calculation," << std::endl;
      std::cout << _legend << "I will add k-factor uncertainty to the nsig uncertainty" << std::endl;
      std::cout << _legend << "I will do it " << ntoys << " times, so one can average. " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << _legend << std::endl;

      Double_t kfactor_err = GetKfactorUncertainty(peak, mode);

      double nsig_kappa = ws->var("nsig_kappa_dielectron")->getVal();
      nsig_kappa = sqrt(nsig_kappa*nsig_kappa + kfactor_err*kfactor_err);
      ws->var("nsig_kappa_dielectron")->setVal(nsig_kappa);

      //ntoys = 1;      
    }
    
    else if ( mode.find("expected") != std::string::npos ){
      
      std::cout << _legend << std::endl;
      std::cout << _legend << "this is pseudoexperiment " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << _legend << "for the expected limit estimate" << std::endl;
      std::cout << _legend << std::endl;

      // prepare PE data
      CreateDielectronToyMc();
    }
    
    else{ //  "regular" observed limit
      
      std::cout << _legend << std::endl;
      std::cout << _legend << "calculating an observed limit..." << std::endl;
      std::cout << _legend << "I will do it " << ntoys << " times, so one can average. " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << _legend << std::endl;

      //ntoys = 1;
    }

    // data
    data = new RooDataSet(*(mData["dielectron"]));

    // change POI range
    delete mpMc;
    mpMc = new ModelConfig(mc);
    mpMc->SetWorkspace(*ws);
    double poiUpper = GetPoiUpper("dielectron", peak);
    std::cout << _legend << "setting POI range to [0; " << poiUpper << "]" << std::endl;
    ws->var("ratio")->setRange(0.0, poiUpper);

    mcInt = GetMcmcInterval(mc,
			    0.95,
			    mcmc_iter,
			    mcmc_burnin,
			    0.0,
			    100);
    
    ++pe_counter;

    if (!mcInt){
      continue;
    }
    else{
      std::string _outfile = "dielectron_ratio_mcmc_limit_" + suffix + ".ascii";
      printMcmcUpperLimit( peak, _outfile );
      
      // limits for averaging/medianing
      RooRealVar * firstPOI = (RooRealVar*) mc.GetParametersOfInterest()->first();
      double _limit = mcInt->UpperLimit(*firstPOI);
      _limits.push_back(_limit);
    }

  } // end of while

  // write median limit to a file
  if (_limits.size() > 0){
    Double_t _median_limit = TMath::Median(_limits.size(), &_limits[0]);
    std::vector<Double_t> _mass_limit;
    _mass_limit.push_back(peak);
    _mass_limit.push_back(_median_limit);
    std::string _outfile = "dimuon_ratio_mcmc_median_limit_" + suffix + ".ascii";
    PrintToFile(_outfile, _mass_limit, "#  mass         median_limit");
  }

  std::string _outfile = "dielectron_ratio_mcmc_posterior_" + suffix + ".pdf";
  makeMcmcPosteriorPlot( _outfile );
    
  return;
}


Double_t TwoBody::DileptonRatioSignificance( std::string mode,
					     std::string channel,
					     std::string inputdir, // directory with workspace files
					     Double_t mass_low,
					     Double_t mass_high,
					     Double_t mass_step,
					     Int_t ntoys,
					     std::string suffix,    // suffix for output file names
					     Double_t fit_range_low,
					     Double_t fit_range_high,
					     bool add_fake_data,// quick check: possibility to scale data up and add interesting events
					     Double_t width_factor
					     ){ 
  //
  // Significance for xsec(Z')/xsec(Z) in dimuon and dielectron channels
  // Now adding individual channels here as well, specify <channel>
  // as "dilepton", "dimuon or "dielectron"
  //

  // this is a prefix for printouts, to know where the message is coming from
  std::string _legend = "[TwoBody::DileptonRatioSignificance]: ";

  // settings for adding signal (special study)
  double _peak = 965.4; // best fit combined
  //double _peak = 961.9; // best fit ee
  double mu_sig_ee   = 5.2;
  //double mu_sig_ee   = 0.0;
  double mu_sig_mumu = mu_sig_ee * 1.4;
  std::string sigPdfDimuonName = "sigpdf_dimuon";
  std::string sigPdfDielectronName = "sigpdf_dielectron";

  // Based on <channel> figure out which channels need to be initialized and run
  bool do_dimuon = false;
  bool do_dielectron = false;
  int n_channels = 0; // this is safety for future extra channels
  if (channel.find("dilepton")!=std::string::npos){
    do_dimuon = true;
    do_dielectron = true;
    n_channels += 2;
  }
  else if (channel.find("dimuon")!=std::string::npos){
    do_dimuon = true;
    ++n_channels;
  }
  else if (channel.find("dielectron")!=std::string::npos){
    do_dielectron = true;
    ++n_channels;
  }

  // this is the significance we want to evaluate and return
  Double_t significance = 0.0;

  // load workspaces and prepare the model
  ModelConfig mc = prepareDileptonRatioModel(inputdir, channel);

  // define the set of parameters that define a null hypothesis
  RooArgSet null_params;
  ws->var("ratio")->setVal(0.0); // value for null hypothesis
  null_params.add( *ws->var("ratio") );
  if (do_dimuon){
    ws->var("nsig_nuis_dimuon")->setVal(1.0); // value for null hypothesis
    null_params.add( *ws->var("nsig_nuis_dimuon") );
  }
  if (do_dielectron){
    ws->var("nsig_nuis_dielectron")->setVal(1.0); // value for null hypothesis
    ws->var("mass_nuis_dielectron")->setVal(1.0); // value for null hypothesis
    null_params.add( *ws->var("nsig_nuis_dielectron") );
    null_params.add( *ws->var("mass_nuis_dielectron") );
  }
  std::string null_parameters_name = "NullParameters";
  std::string null_parameters_snapshot_name = null_parameters_name+ "Snapshot";
  ws->defineSet( null_parameters_name.c_str(), null_params );
  ws->saveSnapshot(null_parameters_snapshot_name.c_str(), *ws->set(null_parameters_name.c_str()));
  

  // this is for quick checks only!
  // when we know of interesting events but don't have properly
  // processed data, we can scale up the existing data
  // and add interesting events by hand
  // absolutely do not do this for real result!!!!!!!
  if (add_fake_data){
    //
    // change the loaded data and some values in the workspace
    //

    Double_t _desired_lumi = 4000.0; // scale to this lumi
    Double_t _scale;
    std::vector<Double_t> _v; // vector to add mass points by hand to be inserted into the data

    // dimuon
    if (do_dimuon){
      _v.clear();
      _scale = _desired_lumi / 4000.0; // dimuons first
      CreateScaledData("dimuon", 600.0, _scale, _v);
    }

    // dielectron
    if (do_dielectron){
      _v.clear();
      _scale = _desired_lumi / 4000.0; // dimuons first
      //_v.push_back(930.0);
      CreateScaledData("dielectron", 600.0, _scale, _v);
    }
  }

  // get observed numbers of events
  Int_t n_ee = -1; // safe default
  Int_t n_mumu = -1;
  if (do_dimuon) n_mumu = mData["dimuon"]->sumEntries();
  if (do_dielectron) n_ee = mData["dielectron"]->sumEntries();

  ws->var("mass")->setRange(fit_range_low, fit_range_high);

  Double_t _width = ws->var("width_p1")->getVal();
  ws->var("width_p1")->setVal(_width*width_factor);

  std::pair<Double_t, Double_t> _max_pllr;
  Double_t _max_pllr_obs = 0.0;
  Double_t _max_pllr_mass = mass_low;

  // let's time this
  TStopwatch t;
  t.Start();

  // observed
  if (mode.find("data")!=std::string::npos){
    createDileptonObservedData();

    // FIXME: testing
    // fit and get signal yield
    //PlotData("dielectron");
    ws->var("peak")->setVal(_peak);
    RooFitResult * _fit = ws->pdf("model")->fitTo(*data,Save());
    //ws->function("nsig_dielectron")->Print();
    //ws->function("nsig_dimuon")->Print();

    _max_pllr = get_pllr_max(mass_low, mass_high, mass_step, "dilepton_ratio_best_fit.pdf", null_parameters_name);
    _max_pllr_obs = _max_pllr.first;
    _max_pllr_mass = _max_pllr.second;
    
    // print to file
    std::string _outfile = "dilepton_ratio_pllr_" + mode + suffix + ".ascii";
    printDileptonRatioSignificance( _outfile, _max_pllr_mass, _max_pllr_obs );
  }
  else if (mode.find("pseudo")!=std::string::npos){
    
    int pe_counter = 0;
    while (pe_counter < ntoys){
      
      std::cout << _legend << std::endl;
      std::cout << _legend << "this is pseudoexperiment " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << _legend << "for the significance estimate" << std::endl;
      std::cout << _legend << std::endl;
      
      // prepare PE data
      if (do_dimuon && do_dielectron && n_channels == 2){
	CreateDileptonPeData(n_ee, n_mumu);
	// add signal (optional)
	AddPseudoData("dimuon", sigPdfDimuonName, mu_sig_mumu, _peak, true);
	AddPseudoData("dielectron", sigPdfDielectronName, mu_sig_ee, _peak, true);
	delete data; // the pointer should always be valid, so delete first to be safe
	RooRealVar * _mass = ws->var("mass");
	data = new RooDataSet( "combdata", "combined data",
			       RooArgSet( (const RooRealVar)(*_mass) ),
			       Index(*channel_cat),
			       Import(mData) );
      }
      else if (do_dimuon && n_channels == 1){
	CreateDimuonPeData(n_mumu);
	AddPseudoData("dimuon", sigPdfDimuonName, mu_sig_mumu, _peak, true);
	delete data;
	data = new RooDataSet(*(mData["dimuon"]));
      }
      else if (do_dielectron && n_channels == 1){
	CreateDielectronPeData(n_ee);
	AddPseudoData("dielectron", sigPdfDielectronName, mu_sig_ee, _peak, true);
	delete data;
	data = new RooDataSet(*(mData["dielectron"]));
      }
      
      //PlotData("dielectron");

      _max_pllr = get_pllr_max(mass_low, mass_high, mass_step, null_parameters_name);
      _max_pllr_obs = _max_pllr.first;
      _max_pllr_mass = _max_pllr.second;
      
      std::string _outfile = "dilepton_ratio_pllr_" + mode + suffix + ".ascii";
      printDileptonRatioSignificance( _outfile, _max_pllr_mass, _max_pllr_obs );
      
      ++pe_counter;
    } // end of while
  } // end of if (pseudo)

  t.Print();

  return significance;
}



Double_t TwoBody::DileptonRatioSignificanceRobustness( std::string inputdir, // directory with workspace files
						       std::string channel,
						       Double_t mass_low,
						       Double_t mass_high,
						       Double_t mass_step,
						       std::string option ){ 
  //
  // Study how robust is the combined significance for xsec(Z')/xsec(Z)
  // in dimuon and dielectron channels against various modifications of
  // the procedure: observable range, background shape etc.
  //

  std::string _legend = "[TwoBody::DileptonRatioSignificanceRobustness]: ";

  Double_t significance = 0.0;

  // Based on <channel> figure out which channels need to be initialized and run
  bool do_dimuon = false;
  bool do_dielectron = false;
  int n_channels = 0; // this is safety for future extra channels
  if (channel.find("dilepton")!=std::string::npos){
    do_dimuon = true;
    do_dielectron = true;
    n_channels += 2;
  }
  else if (channel.find("dimuon")!=std::string::npos){
    do_dimuon = true;
    ++n_channels;
  }
  else if (channel.find("dielectron")!=std::string::npos){
    do_dielectron = true;
    ++n_channels;
  }

  ModelConfig mc = prepareDileptonRatioModel(inputdir, channel);  

  //Int_t n_ee = mData["dielectron"]->sumEntries();
  //Int_t n_mumu = mData["dimuon"]->sumEntries();

  // define the set of parameters that define a null hypothesis
  RooArgSet null_params;
  ws->var("ratio")->setVal(0.0); // value for null hypothesis
  null_params.add( *ws->var("ratio") );
  if (do_dimuon){
    ws->var("nsig_nuis_dimuon")->setVal(1.0); // value for null hypothesis
    null_params.add( *ws->var("nsig_nuis_dimuon") );
  }
  if (do_dielectron){
    ws->var("nsig_nuis_dielectron")->setVal(1.0); // value for null hypothesis
    ws->var("mass_nuis_dielectron")->setVal(1.0); // value for null hypothesis
    null_params.add( *ws->var("nsig_nuis_dielectron") );
    null_params.add( *ws->var("mass_nuis_dielectron") );
  }
  std::string null_parameters_name = "NullParameters";
  ws->defineSet( null_parameters_name.c_str(), null_params );

  // prepare output file
  std::ofstream aFile;
  

  // let's time this
  TStopwatch t;
  t.Start();

  if (option.find("low_range") != std::string::npos){
    //
    // scan - move the lower boundary
    //
    
    // append to file if exists
    std::string _outfile = "dilepton_ratio_low_boundary_scan.ascii";
    aFile.open(_outfile.c_str(), std::ios_base::app);

    for (Double_t _low = 200.0; _low <900.0; _low += 25.0){
      
      ws->var("mass")->setRange(_low, 2000.0);
      
      std::pair<Double_t, Double_t> _max_pllr;
      Double_t _max_pllr_obs = 0.0;
      Double_t _max_pllr_mass = mass_low;
      
      createDileptonObservedData();
      _max_pllr = get_pllr_max(mass_low, mass_high, mass_step, "", null_parameters_name); // no fit plot needed
      _max_pllr_obs = _max_pllr.first;
      _max_pllr_mass = _max_pllr.second;
      
      // print to file
      //std::string _outfile = "dilepton_ratio_low_boundary_scan.ascii";
      //printDileptonRatioSignificance( _outfile, _max_pllr_mass, _max_pllr_obs );
      char buf[1024];
      sprintf(buf, "%7.1f   %7.6f   %7.1f", _low, _max_pllr_obs, _max_pllr_mass);
      aFile << buf << std::endl;
      
    }
  }
  else if (option.find("high_range") != std::string::npos){
    //
    // scan - move the upper boundary
    //

    // append to file if exists
    std::string _outfile = "dilepton_ratio_high_boundary_scan.ascii";
    aFile.open(_outfile.c_str(), std::ios_base::app);

    for (Double_t _high = 2000.0; _high > 1000.0; _high -= 25.0){
      
      ws->var("mass")->setRange(650.0, _high);
      
      std::pair<Double_t, Double_t> _max_pllr;
      Double_t _max_pllr_obs = 0.0;
      Double_t _max_pllr_mass = mass_low;
      
      createDileptonObservedData();
      _max_pllr = get_pllr_max(mass_low, mass_high, mass_step, "", null_parameters_name); // no fit plot needed
	_max_pllr_obs = _max_pllr.first;
	_max_pllr_mass = _max_pllr.second;
	
	// print to file
	//std::string _outfile = "dilepton_ratio_low_boundary_scan.ascii";
	//printDileptonRatioSignificance( _outfile, _max_pllr_mass, _max_pllr_obs );
	char buf[1024];
	sprintf(buf, "%7.1f   %7.6f   %7.1f", _high, _max_pllr_obs, _max_pllr_mass);
	aFile << buf << std::endl;
	
    }
  }

  t.Print();
  
  // close outfile here so it is safe even if subsequent iterations crash
  aFile.close();

  return significance;
}



Double_t TwoBody::get_pllr( std::string nullParametersName ){
  //
  // Compute the profile log likelihood ratio test statistic
  // Gets the set of null model parameters from the workspace by name
  //

  std::string _legend = "[TwoBody::get_pllr]: ";

  // use ProfileLikelihood
  ProfileLikelihoodCalculator plc(*data, mc);
  plc.SetConfidenceLevel(0.68);

  std::string null_parameters_snapshot_name = nullParametersName+ "Snapshot";
  ws->loadSnapshot( null_parameters_snapshot_name.c_str() );

  const RooArgSet * pNullParameters = ws->set(nullParametersName.c_str());
  plc.SetNullParameters(*pNullParameters);

  // suppress RooFit verbosity
  RooFit::MsgLevel msglevel = RooMsgService::instance().globalKillBelow();
  RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);

  RooStats::HypoTestResult * htr = plc.GetHypoTest();
  Double_t significance = htr->Significance();

  logfile << _legend << "significance = " << significance << std::endl;
  delete htr;

  // return RooFit verbosity to previous level
  RooMsgService::instance().setGlobalKillBelow(msglevel);

  return significance;
  //return 4.0;
}
  
  

std::pair<Double_t, Double_t> TwoBody::get_pllr_max( Double_t mass_low,
						     Double_t mass_high,
						     Double_t mass_step,
						     std::string fit_plot_file_name,
						     std::string null_parameters_name ){
  //
  // Scan the mass range and find the most extreme test statistic value
  // 
  // If fit_plot_file_name is not "", draw fit projections, save to file
  //

  std::pair<Double_t, Double_t> _res;

  Double_t _max_pllr = 0.0;
  Double_t _max_mass = 0.0;

  // FIXME: does llr slopes with mass?
  TH1F * h_llr = new TH1F("llr","llr",(int)((mass_high-mass_low)/mass_step)+1,mass_low, mass_high);
  int _co = 1;
  for (Double_t _p = mass_low; _p < mass_high; _p += mass_step){

    // set resonance peak position
    ws->var("peak")->setVal(_p);

    Double_t _pllr = get_pllr(null_parameters_name);
    if (_pllr > _max_pllr){
      _max_pllr = _pllr;
      _max_mass = _p;
    }

    // FIXME: does llr slopes with mass?
    //std::cout << _pllr << std::endl;
    h_llr->SetBinContent(_co,_pllr);
    ++_co;
  }
  h_llr->Draw();

  // try adaptive
  ws->var("peak")->setVal(_max_mass);
  Double_t _min_range = ws->var("peak")->getMin();
  Double_t _max_range = ws->var("peak")->getMax();
  ws->var("peak")->setRange(_max_mass-mass_step, _max_mass+mass_step);
  ws->var("peak")->setConstant(kFALSE);
  RooFitResult * _fit = ws->pdf("model")->fitTo(*data,Save());
  //RooFitResult * _fit = 0;
  _max_mass = ws->var("peak")->getVal();

  // put all back
  ws->var("peak")->setRange(_min_range, _max_range);
  ws->var("peak")->setConstant(kTRUE);
  //_max_pllr = get_pllr(null_parameters_name);

  if (fit_plot_file_name.size()>0){
    // make and save fit plot
  /*

    ws->function("nsig_dimuon")->Print();
    ws->function("nsig_dielectron")->Print();
    RooPlot * _frame1 = ws->var("mass")->frame(100) ;
    RooPlot * _frame2 = ws->var("mass")->frame(100) ;
    *data->plotOn(_frame1, Cut("channels==channels::dimuon"), Name("dimuon")) ;
    ws->pdf("model")->plotOn(_frame1,Slice(*ws->cat("channels"),"dimuon"),ProjWData(*ws->cat("channels"),*data)) ;
    *data->plotOn(_frame2, Cut("channels==channels::dielectron"), Name("dielectron")) ;
    ws->pdf("model")->plotOn(_frame2,Slice(*ws->cat("channels"),"dielectron"),ProjWData(*ws->cat("channels"),*data)) ;
    TCanvas c2("c2", "c2", 425, 550);
    c2.Divide(1,2);
    c2.cd(1);
    _frame1->Draw();
    c2.cd(2);
    _frame2->Draw();
    c2.SaveAs(fit_plot_file_name.c_str());
    delete _frame1;
    delete _frame2;
  */
  }

  _max_pllr = get_pllr(null_parameters_name);

  _res.first = _max_pllr;
  _res.second = _max_mass;

  // clean up
  delete _fit;

  return _res;
}



Double_t TwoBody::GetKfactorUncertainty( Float_t peak, std::string model ){
  //
  // return k-factor uncertainty for a given model
  //

  if (model.find("mass_cteq") != std::string::npos){
    // cteq611 fit
    return 0.0 + 0.00996035*log(peak/250.0);
  }
  else if (model.find("mass_graviton") != std::string::npos){
    // graviton model
    return 0.10;
  }
  else return 0.0;
}



void TwoBody::DileptonRatioLimit( Float_t peak,
				  std::string mode,      // obsereved, expected, mass limit (extra k-factor uncertainty)
				  std::string suffix,    // suffix for output file names
				  Int_t ntoys,           // number of pseudoexperiments for expected limit
				  Int_t mcmc_iter,
				  Int_t mcmc_burnin,
				  std::string inputdir){ // directory with workspace files
  //
  // combined limit on ratio = xsec(Z')/xsec(Z) in dimuon and dielectron channels
  //

  std::string _legend = "[TwoBody::DileptonRatioLimit]: ";

  ModelConfig mc = prepareDileptonRatioModel(inputdir, "dilepton");  

  // set model parameters and data
  SetDimuonRatioParameters(peak);
  SetDielectronRatioParameters(peak);

  ws->Print();

  // change fit range
  std::cout << _legend << "setting fit range to [200; 2500]" << std::endl;
  ws->var("mass")->setRange(200.0, 2500.0);

  // change POI range
  //double poiUpper = GetPoiUpper("dilepton", peak);
  //std::cout << _legend << "setting POI range to [0; " << poiUpper << "]" << std::endl;
  //ws->var("ratio")->setRange(0.0, poiUpper);
  

  //ws->SaveAs("ws_dilepton_ratio.root");

  int pe_counter = 0;
  std::vector<Double_t> _limits;
  while (pe_counter < ntoys){
    
    // for mass limit, add k-factor systematics to the nsig systematics
    // FIXME: this is a correlated part of the uncertainty!!!
    //  - different uncertainties for graviton and Z' models
    if ( mode.find("mass_") != std::string::npos ){
      
      std::cout << _legend << std::endl;
      std::cout << _legend << "this a mass limit calculation," << std::endl;
      std::cout << _legend << "I will add k-factor uncertainty to the nsig uncertainty" << std::endl;
      std::cout << _legend << "I will do it " << ntoys << " times, so one can average. " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << _legend << std::endl;

      Double_t kfactor_err = GetKfactorUncertainty(peak, mode);

      double nsig_kappa = ws->var("nsig_kappa_dimuon")->getVal();
      nsig_kappa = sqrt(nsig_kappa*nsig_kappa + kfactor_err*kfactor_err);
      ws->var("nsig_kappa_dimuon")->setVal(nsig_kappa);

      nsig_kappa = ws->var("nsig_kappa_dielectron")->getVal();
      nsig_kappa = sqrt(nsig_kappa*nsig_kappa + kfactor_err*kfactor_err);
      ws->var("nsig_kappa_dielectron")->setVal(nsig_kappa);

      //ntoys = 1;
    }
    
    else if ( mode.find("expected") != std::string::npos ){
      
      std::cout << _legend << std::endl;
      std::cout << _legend << "this is pseudoexperiment " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << _legend << "for the expected limit estimate" << std::endl;
      std::cout << _legend << std::endl;

      // prepare PE data
      CreateDileptonToyMc();
    }
    
    else{ //  "regular" observed limit
      
      std::cout << _legend << std::endl;
      std::cout << _legend << "calculating an observed limit..." << std::endl;
      std::cout << _legend << "I will do it " << ntoys << " times, so one can average. " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << _legend << std::endl;

      //ntoys = 1;
    }

    // data
    //data = new RooDataSet(*(mData["dimuon"]));

    createDileptonObservedData();

    // change POI range
    delete mpMc;
    mpMc = new ModelConfig(mc);
    mpMc->SetWorkspace(*ws);
    double poiUpper = GetPoiUpper("dilepton", peak);
    std::cout << _legend << "setting POI range to [0; " << poiUpper << "]" << std::endl;
    ws->var("ratio")->setRange(0.0, poiUpper);

    mcInt = GetMcmcInterval(mc,
			    0.95,
			    mcmc_iter,
			    mcmc_burnin,
			    0.0,
			    100);
    
    ++pe_counter;

    if (!mcInt){
      continue;
    }
    else{
      std::string _outfile = "dilepton_ratio_mcmc_limit_" + suffix + ".ascii";
      printMcmcUpperLimit( peak, _outfile );
      
      // limits for averaging/medianing
      RooRealVar * firstPOI = (RooRealVar*) mc.GetParametersOfInterest()->first();
      double _limit = mcInt->UpperLimit(*firstPOI);
      _limits.push_back(_limit);
    }

  } // end of while

  // write median limit to a file
  if (_limits.size() > 0){
    Double_t _median_limit = TMath::Median(_limits.size(), &_limits[0]);
    std::vector<Double_t> _mass_limit;
    _mass_limit.push_back(peak);
    _mass_limit.push_back(_median_limit);
    std::string _outfile = "dimuon_ratio_mcmc_median_limit_" + suffix + ".ascii";
    PrintToFile(_outfile, _mass_limit, "#  mass         median_limit");
  }

  std::string _outfile = "dilepton_ratio_mcmc_posterior_" + suffix + ".pdf";
  makeMcmcPosteriorPlot( _outfile );
   
  return;
}


void TwoBody::ThreeChannelLimit( Float_t peak,
				 std::string mode,   // obsereved, expected, mass limit (extra k-factor uncertainty)
				 std::string suffix, // suffix for output file names
				 Int_t ntoys,        // number of pseudoexperiments for expected limit
				 Int_t mcmc_iter,
				 Int_t mcmc_burnin,
				 std::string inputdir){ // directory with workspace files
  //
  // combined di-lepton/di-photon limit
  //

  std::string _legend = "[TwoBody::ThreeChannelLimit]: ";

  ModelConfig mc = prepareThreeChannelModel(inputdir);  
  
  // set model parameters and data
  SetSharedParameters(peak);
  SetDimuonParameters();
  SetDielectronParameters();
  SetDiphotonParameters( ws->var("peak")->getVal(), 0.01 );
  ws->Print();
  Diagnostics();
  
  createThreeChannelObservedData();

  int pe_counter = 0;
  while (pe_counter < ntoys){
    
    // for mass limit, add k-factor systematics to the lumi systematics
    //  - we consider only the graviton theoretical models for
    //    the three channel combination, so add 10%
    if ( mode.find("mass") != std::string::npos ){
      
      std::cout << _legend << std::endl;
      std::cout << _legend << "this a mass limit calculation," << std::endl;
      std::cout << _legend << "I will add k-factor uncertainty to the lumi uncertainty" << std::endl;
      std::cout << _legend << std::endl;

      double lumi_kappa = ws->var("lumi_kappa")->getVal();
      lumi_kappa =  sqrt(lumi_kappa*lumi_kappa + 0.1*0.1);
      ws->var("lumi_kappa")->setVal(lumi_kappa);
    }
    
    else if ( mode.find("expected") != std::string::npos ){
      
      std::cout << _legend << std::endl;
      std::cout << _legend << "this is pseudoexperiment " << pe_counter+1 << " of " << ntoys << std::endl;
      std::cout << _legend << "for the expected limit estimate" << std::endl;
      std::cout << _legend << std::endl;

      // prepare PE data
      CreateThreeChannelToyMc();
    }
    
    else{ //  "regular" observed limit
      
      std::cout << _legend << std::endl;
      std::cout << _legend << "calculating an observed limit..." << std::endl;
      std::cout << _legend << std::endl;

      ntoys = 1;
    }

    mcInt = GetMcmcInterval(mc,
			    0.95,
			    mcmc_iter,
			    mcmc_burnin,
			    0.0,
			    100);
    
    ++pe_counter;

    if (!mcInt){
      continue;
    }
    else{
      std::string _outfile = "comb_" + suffix + ".ascii";
      printMcmcUpperLimit( peak, _outfile );
    }

  } // end of while

  std::string _outfile = "comb_mcmc_posterior_" + suffix + ".pdf";
  makeMcmcPosteriorPlot( _outfile );
    
  return;
}


RooAbsData * 
TwoBody::CreateScaledData( std::string channel,
			   Double_t cutoff,
			   Double_t scale,
			   std::vector<Double_t> events){
  //
  // create "fake" data scaled from a smaller dataset
  // to a higher luminosity, with extra "interesting" events added
  //   - channel: ee, mumu or whatever else is there by now
  //   - cutoff:  only use events below this to generate extra entries
  //   - scale:   how much more data is desired
  //   - events:  masses for extra "interesting" events to be added
  //
  
  RooRealVar * _mass = ws->var("mass");
  //std::string _lumi_str = "lumi_"+channel;
  //Double_t _lumi_orig = ws->var(_lumi_str.c_str())->getVal();
  //Double_t scale = lumi/_lumi_orig;
  Int_t _n_orig = mData[channel]->sumEntries();
  Int_t _n_add = (Int_t)((scale-1.0)*((Double_t)_n_orig)) - events.size();
  std::cout << "[TwoBody::CreateScaledData]: extending data in channel " << channel << std::endl;
  //std::cout << "[TwoBody::CreateScaledData]: current luminosity: " << _lumi_orig << std::endl;
  std::cout << "[TwoBody::CreateScaledData]: current entries: " << _n_orig << std::endl;
  //std::cout << "[TwoBody::CreateScaledData]: requested luminosity: " << lumi << std::endl;
  std::cout << "[TwoBody::CreateScaledData]: will add events: " << _n_add << std::endl;
  std::cout << "[TwoBody::CreateScaledData]: only scale events below cutoff = " << cutoff << std::endl;
    
  // check if a valid dataset extension is requested
  if (_n_add < 1){
    std::cout << "[TwoBody::CreateScaledData]: no scaling of data requested"
	      << std::endl;
  }
  else{
    RooDataSet * _reduced = (RooDataSet *)(mData[channel]->reduce("mass<600"));
    RooDataHist * _dh = _reduced->binnedClone("dh", "dh");
    RooHistPdf * _hp = new RooHistPdf("hp", "hp", *_mass, *_dh);
    RooDataSet * _extra = _hp->generate(*_mass, _n_add);
    
    // append scaled background
    mData[channel]->append(*_extra);

    delete _reduced;
    delete _dh;
    delete _hp;
    delete _extra;
  }

  // append interesting events
  for (std::vector<Double_t>::const_iterator m = events.begin();
       m != events.end();
       ++m){
    _mass->setVal(*m);
    mData[channel]->add(*_mass);
    std::cout << "[TwoBody::CreateScaledData]: added m = " << *m << std::endl;
  }
  
  // cross check
  /*
  TCanvas c;
  RooPlot * _frame = _mass->frame();
  mData[channel]->plotOn(_frame);
  std::string _pdf_str = "bkgpdf_"+channel;
  ws->pdf(_pdf_str.c_str())->plotOn(_frame);
  _frame->Draw();
  c.SaveAs("reducedData.png");
  */

  return mData[channel];
}



void TwoBody::PlotData( std::string channel ){
  //
  // Plot channel data. Does not clean up after itself!
  //

  RooRealVar * _mass = ws->var("mass");

  TCanvas * c = new TCanvas();
  c->SetLogy();
  RooPlot * _frame = _mass->frame();
  mData[channel]->plotOn(_frame);
  _frame->Draw();
  c->SaveAs("data.png");

  return;
}



Int_t TwoBody::AddPseudoData( std::string channel,
			      std::string pdfName,
			      double nev,
			      double peak,
			      bool poisson ){
  //
  // generate a pseudo-data set according to pdfName
  // and append it to channel
  // if poisson==true, randomize number of generated
  // events by Poisson
  //

  std::string legend = "[TwoBody::AddPseudoData]: ";

  Int_t _n = (Int_t)(nev+0.5);
  double _dn = (double)_n;

  //if (poisson) _n = r.Poisson(nev);
  if (poisson) _n = r.Poisson(_dn);

  // set signal resonance mass (just in case)
  ws->var("peak")->setVal(peak);

  // create dataset
  RooRealVar * _mass = ws->var("mass");
  RooArgSet _vars(*_mass);

  RooAbsPdf * _pdf = ws->pdf(pdfName.c_str());

  RooAbsPdf::GenSpec * _spec = _pdf->prepareMultiGen(_vars,
						     Name("toys"),
						     NumEvents(_n),
						     Extended(kFALSE),
						     Verbose(kTRUE));
  RooDataSet * _data = 0;

  _data = _pdf->generate(*_spec);

  Int_t n_generated = (Int_t)(_data->sumEntries());

  delete _spec;

  //SetChannelData("dimuon", data);
  if ( mData.find(channel) == mData.end() ){
    mData[channel.c_str()] = _data;
  }
  else{
    mData[channel]->append(*_data);
  }

  delete _data;

  // debug
  std::cout << legend << "generated " << n_generated << " events with "
	    << pdfName << " and added to the "
	    << channel << " channel"  << std::endl;
  //std::cout << "!!!!!!!!!!!!!! _result = " << _result    << std::endl;

  return n_generated;
}



//----------------- global functions 

Double_t dimuon_ratio_limit( Float_t peak,
			     std::string mode,    // obsereved, expected, mass limit (extra k-factor uncertainty)
			     std::string suffix,  // suffix for output file names
			     Int_t ntoys,         // number of pseudoexperiments for expected limit
			     Int_t mcmc_iter,     // number of MCMC iterations
			     Int_t mcmc_burnin,   // number of MCMC burn in steps to be discarded
			     std::string inputdir){ // directory with workspace files
  
  Double_t limit = -1.0;  

  TwoBody dimuon;
  dimuon.DimuonRatioLimit(peak,
			  mode,
			  suffix,
			  ntoys,
			  mcmc_iter,
			  mcmc_burnin,
			  inputdir);
  
  return limit;
}


Double_t dilepton_diphoton_combine( Float_t peak,
				    std::string mode,    // obsereved, expected, mass limit (extra k-factor uncertainty)
				    std::string suffix,  // suffix for output file names
				    Int_t ntoys,         // number of pseudoexperiments for expected limit
				    Int_t mcmc_iter,     // number of MCMC iterations
				    Int_t mcmc_burnin,   // number of MCMC burn in steps to be discarded
				    std::string inputdir){ // directory with workspace files

  Double_t limit = -1.0;  

  TwoBody comb;
  comb.ThreeChannelLimit(peak,
			 mode,
			 suffix,
			 ntoys,
			 mcmc_iter,
			 mcmc_burnin,
			 inputdir);

  return limit;
}


Double_t limit( std::string channel, // dimuon, dielectron, mumuee, etc
		std::string mode,    // observed, expected, mass limit (extra k-factor uncertainty)
		Float_t peak,        // resonance mass
		std::string suffix,  // suffix for output file names
		Int_t ntoys,         // number of pseudoexperiments for expected limit
		Int_t mcmc_iter,     // number of MCMC iterations
		Int_t mcmc_burnin,   // number of MCMC burn in steps to be discarded
		std::string inputdir){ // directory with workspace files

  // time it
  TStopwatch t;
  t.Start();
  
  Double_t limit = -1.0;  

  TwoBody manager;

  //dimuon single channel ratio limit
  if (channel.find("dimuon") != std::string::npos ){
    manager.DimuonRatioLimit(peak, mode, suffix,
			     ntoys, mcmc_iter, mcmc_burnin,
			     inputdir);
  }
  if (channel.find("dielectron") != std::string::npos ){
    manager.DielectronRatioLimit(peak, mode, suffix,
				 ntoys, mcmc_iter, mcmc_burnin,
				 inputdir);
  }
  //dilepton ee-mumu channels combined ratio limit
  if (channel.find("dilepton") != std::string::npos ){
    manager.DileptonRatioLimit(peak, mode, suffix,
			       ntoys, mcmc_iter, mcmc_burnin,
			       inputdir);
  }
  // mumu+ee+gg combined xsec limit
  else if (channel.find("mumu-ee-gg-xsec") != std::string::npos ){
    manager.ThreeChannelLimit(peak, mode, suffix,
			      ntoys, mcmc_iter, mcmc_burnin,
			      inputdir);

  }

  // get time
  t.Print();
  
  return limit;
}



Double_t significance( std::string mode,
		       std::string channel,
		       std::string inputdir, // directory with workspace files
		       Double_t mass_low,
		       Double_t mass_high,
		       Double_t mass_step,
		       Int_t ntoys,
		       std::string suffix,
		       Double_t fit_range_low,
		       Double_t fit_range_high,
		       bool add_fake_data,
		       Double_t width_factor){
  //
  // driver method to get significance with LEE
  //
  
  Double_t significance = 0.0;  

  TwoBody manager;

  significance = manager.DileptonRatioSignificance( mode,
						    channel,
						    inputdir,
						    mass_low, mass_high, mass_step,
						    ntoys,
						    suffix,
						    fit_range_low,
						    fit_range_high,
						    add_fake_data,
						    width_factor);
  return significance;
}



Double_t robustness( std::string inputdir, // directory with workspace files
		     std::string channel,
		     Double_t mass_low,
		     Double_t mass_high,
		     Double_t mass_step,
		     std::string option ){
  //
  // robustness checks for significance: moving fit boundaries and such
  //
  
  Double_t significance = 0.0;  

  TwoBody manager;

  significance = manager.DileptonRatioSignificanceRobustness( inputdir,
							      channel,
							      mass_low, mass_high, mass_step,
							      option );
  return significance;
}



void devel( void ){
  //
  // quick checks for development
  //

  TwoBody manager;

  ModelConfig mc = manager.prepareDileptonRatioModel( "",
						      "dilepton" );
  manager.GetWorkspace()->Print();

  std::vector<Double_t> _v;
  _v.push_back(1500.0);
  _v.push_back(1500.0);
  _v.push_back(1500.0);
  _v.push_back(1500.0);
  RooAbsData * _d = manager.CreateScaledData("dimuon", 600.0, 3.0, _v);
  delete _d;

  return;
}
