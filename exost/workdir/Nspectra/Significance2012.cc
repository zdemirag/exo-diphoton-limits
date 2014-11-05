#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <map>

#include "RooWorkspace.h"
#include "TFile.h"

#include "ModelConfiguratorZprime.hh"
#include "Resultator.hh"
#include "PoiRangeEstimator.hh"
#include "DataPruner.hh"

using namespace RooFit;
using namespace std;

int main(int argc, char* argv[]) {

   //read options

   bool run_channel1 = true;
   bool run_channel2 = true;
   bool run_channel3 = true;
   bool run_channel4 = true;
   bool datapruning = true;

   double mass_low = static_cast<double>(atof(argv[5]));
   double mass_high = static_cast<double>(atof(argv[6]));
   double mass_step = static_cast<double>(atof(argv[7]));
   double fit_range_low = static_cast<double>(atof(argv[8]));
   double fit_range_high = static_cast<double>(atof(argv[9]));
   double width_factor = static_cast<double>(atof(argv[10]));
   unsigned int ntoys  = atoi(argv[11]);

   string filesuffix = argv[12];
   string mode = argv[13];
   string workspacedir = argv[14];

   int help_bool = atoi(argv[15]);
   if(help_bool == 0){
      datapruning = false;
   }

   int help_run_channel = atoi(argv[1]);
   if(help_run_channel == 0){
      run_channel1 = false;
   }
   else{
      run_channel1 = true;
   }
   help_run_channel = atoi(argv[2]);
   if(help_run_channel == 0){
      run_channel2 = false;
   }
   else{
      run_channel2 = true;
   }
   help_run_channel = atoi(argv[3]);
   if(help_run_channel == 0){
      run_channel3 = false;
   }
   else{
      run_channel3 = true;
   }
   help_run_channel = atoi(argv[4]);
   if(help_run_channel == 0){
      run_channel4 = false;
   }
   else{
      run_channel4 = true;
   }



cout << "do some testing ..." << endl;

std::map<string, RooWorkspace*> WSvec; // maps channel names to workspaces

// DEFINE WOKSPACES AND CHANNEL NAMES

// CHANNEL: dimuon2011

const std::string filename1 = workspacedir + "/ws_dimuon_ratio_full2011v1.root"; // the root file containing the workspace
const std::string ws_name1 = "myWS"; // the name of the workspace TObject to be used
const std::string channelname1 = "dimuon2011"; // name of the channel -> to be used in your daugther class of ModelConfigurator
TFile file1(filename1.c_str(), "read"); // construct TFile object to load the workspace

if(run_channel1){

RooWorkspace * ws1 = (RooWorkspace *)file1.Get( ws_name1.c_str() ); // load the workspace
cout << "workspacename: " << ws1->GetName();

WSvec.insert( pair<string, RooWorkspace *>(channelname1, ws1) ); // insert channel info

}

// CHANNEL: dimuon2012

const std::string filename3 = workspacedir + "/ws_dimuon_ratio_prelim_2012.root"; // the root file containing the workspace
const std::string ws_name3 = "myWS"; // the name of the workspace TObject to be used
const std::string channelname3 = "dimuon2012"; // name of the channel -> to be used in your daugther class of ModelConfigurator
TFile file3(filename3.c_str(), "read"); // construct TFile object to load the workspace

if(run_channel3){

RooWorkspace * ws3 = (RooWorkspace *)file3.Get( ws_name3.c_str() ); // load the workspace
cout << "workspacename: " << ws3->GetName();

WSvec.insert( pair<string, RooWorkspace *>(channelname3, ws3) ); // insert channel info

}

// CHANNEL: dielectron2011

const std::string filename2 = workspacedir + "/ws_dielectron_ratio_full2011v1.root"; // the root file containing the workspace
const std::string ws_name2 = "myWS"; // the name of the workspace TObject to be used
const std::string channelname2 = "dielectron2011"; // name of the channel -> to be used in your daugther class of ModelConfigurator
TFile file2(filename2.c_str(), "read"); // construct TFile object to load the workspace

if(run_channel2){

RooWorkspace * ws2 = (RooWorkspace *)file2.Get( ws_name2.c_str() ); // load the workspace
cout << "workspacename: " << ws2->GetName();

WSvec.insert( pair<string, RooWorkspace *>(channelname2, ws2) ); // insert channel info

}

// CHANNEL: dielectron2012

const std::string filename4 = workspacedir + "/ws_dielectron_ratio_prelim_2012.root"; // the root file containing the workspace
const std::string ws_name4 = "myWS"; // the name of the workspace TObject to be used
const std::string channelname4 = "dielectron2012"; // name of the channel -> to be used in your daugther class of ModelConfigurator
TFile file4(filename4.c_str(), "read"); // construct TFile object to load the workspace

if(run_channel4){

RooWorkspace * ws4 = (RooWorkspace *)file4.Get( ws_name4.c_str() ); // load the workspace
cout << "workspacename: " << ws4->GetName();

WSvec.insert( pair<string, RooWorkspace *>(channelname4, ws4) ); // insert channel info

}

// CREATE MODELCONFIGURATOR

ModelConfiguratorZprime * myConfigurator = new ModelConfiguratorZprime(WSvec); // construct ModelConfigurator

// ADDITIONAL INFORMATION FOR COMBINING THE CHANNELS

//define list with shared Vars
std::string sharedVarsString = "peak,mass,ratio,width_p0,width_p1,width";
// set shared variables 
// COMMENT: this structure may not be sufficient
myConfigurator->setSharedVars(sharedVarsString);
// COMMENT: for combination of 2011 and 2012 data we will also need to handle correlated Variables -> think about best way to do this

// define the parameter of interest
// CONVENTION: poi needs to have the same name in all single channel workspaces
std::string poiString = "ratio";
myConfigurator->setPoiString(poiString);


// ASSOCIATE VARIABLES WITH STATISTICAL MEANING

// CHANNEL: dimuon2011

if(run_channel1){

//see twobody.C line 1790 ff
string nuisanceParamsString_channel1 = "beta_nsig,beta_nbkg";
string GlobalObsParamsString_channel1 = "glob_nsig,glob_nbkg";
string ObservableParamsString_channel1 = "mass";

myConfigurator->setNuisanceParams(channelname1, nuisanceParamsString_channel1);
myConfigurator->setGlobalObs(channelname1, GlobalObsParamsString_channel1);
myConfigurator->setObservables(channelname1, ObservableParamsString_channel1);

}

// CHANNEL: dimuon2012

if(run_channel3){

string nuisanceParamsString_channel3 = "beta_nsig,beta_nbkg";
string GlobalObsParamsString_channel3 = "glob_nsig,glob_nbkg";
string ObservableParamsString_channel3 = "mass";

myConfigurator->setNuisanceParams(channelname3, nuisanceParamsString_channel3);
myConfigurator->setGlobalObs(channelname3, GlobalObsParamsString_channel3);
myConfigurator->setObservables(channelname3, ObservableParamsString_channel3);

}

// CHANNEL: dielectron2011

if(run_channel2){

string nuisanceParamsString_channel2 = "beta_nsig,beta_nbkg,beta_mass";
string GlobalObsParamsString_channel2 = "glob_nsig,glob_nbkg,glob_mass";
string ObservableParamsString_channel2 = "mass";

myConfigurator->setNuisanceParams(channelname2, nuisanceParamsString_channel2);
myConfigurator->setGlobalObs(channelname2, GlobalObsParamsString_channel2);
myConfigurator->setObservables(channelname2, ObservableParamsString_channel2);

}

// CHANNEL: dielectron2012

if(run_channel4){

string nuisanceParamsString_channel4 = "beta_nsig,beta_nbkg,beta_mass";
string GlobalObsParamsString_channel4 = "glob_nsig,glob_nbkg,glob_mass";
string ObservableParamsString_channel4 = "mass";

myConfigurator->setNuisanceParams(channelname4, nuisanceParamsString_channel4);
myConfigurator->setGlobalObs(channelname4, GlobalObsParamsString_channel4);
myConfigurator->setObservables(channelname4, ObservableParamsString_channel4);

}

//setup combined RooWorkSpace and ModelConfig

myConfigurator->Setup();

//safe combined WS

//myConfigurator->WriteCombinedWS("CombinedWS2011x2.root");

//Setup DataPruner
std::map<std::string , double> Rangemap;
if( datapruning){
   if(run_channel1){ Rangemap.insert( pair<std::string, double>("dimuon2011", 400.) );}
   if(run_channel2){ Rangemap.insert( pair<std::string, double>("dielectron2011", 400.) );}
   if(run_channel3){ Rangemap.insert( pair<std::string, double>("dimuon2012", 400.) );}
   if(run_channel4){ Rangemap.insert( pair<std::string, double>("dielectron2012", 400.) );}
}

DataPruner * mydatapruner = new DataPruner(Rangemap);

// RUN Significance results
Resultator * myResultator = new Resultator(myConfigurator,mydatapruner);
myResultator->calculateRatioSignificance( mode, mass_low, mass_high, mass_step, ntoys, filesuffix, fit_range_low, fit_range_high, width_factor, false) ;

cout << ".. did some testing" << endl;

}