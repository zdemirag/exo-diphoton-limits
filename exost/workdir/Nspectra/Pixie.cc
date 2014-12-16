//
// authors: Gena Kukartsev, Stefan A. Schmitz
//
// project started: April 2012
// 

#include "Pixie.hh"
#include <cmath>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <boost/algorithm/string.hpp>
#include "RooRealVar.h"
#include "RooWorkspace.h"

using namespace std;
using namespace RooFit;
using namespace RooStats;

//--------------------Constructor
Pixie::Pixie() : legend("Pixie"){
;
}

Pixie::~Pixie() {
;
}

std::vector<std::string> Pixie::commaSepStringToStringVec(std::string commaSepString) {
   std::vector<std::string> stringVec;
   boost::split(stringVec, commaSepString, boost::is_any_of(","));
   return stringVec;
}

std::vector<std::string> Pixie::commaSepStringToStringForRooArgSet(std::string channelname, std::string commaSepString, std::vector<std::string> sharedVars) {
   std::string funclegend = "commaSepStringToStringVecPlusChannelName(...) - ";
   funclegend += channelname;

   std::vector<std::string> stringVec;
   stringVec = commaSepStringToStringVec(commaSepString);

      //check if shared variables have been defined
      if( sharedVars.empty() ){
         cout << legend << funclegend << " WARNING: shared variables have not yet been set (only ok if there are none)" << endl;
      }

   for(std::vector<std::string>::iterator it= stringVec.begin(); it != stringVec.end(); it++ ){
      //check if variable is contained in the sharedVars vector
      if(std::find(sharedVars.begin(), sharedVars.end(), *(it)) == sharedVars.end()){
         (*it)=(*it)+"_"+channelname;
      }
   }

   return stringVec;
}

void Pixie::SetupFor7and8Combination(RooWorkspace * ws, std::string channelname){
   std::string funclegend = "SetupFor7and8Combination(...) - ";
   funclegend += channelname;

   double zprimemass = ws->var("peak")->getVal();
   double factor7to8 = 1.0;
   double factorZprime7over8 = 1.0;
   double factorZ7over8 = 1.0;


   if (channelname == "dimuon2011"){
      double lumi_2011 = 5300.;
      double lumi_2012 = 4076.;

      double nz_2011 = ws->var("nz_dimuon2011")->getVal();
      double nsig_scale_2011 = ws->var("nsig_scale_dimuon2011")->getVal();
      double nz_2012 = ws->var("nz_dimuon2012")->getVal();
      double nsig_scale_2012 = ws->var("nsig_scale_dimuon2012")->getVal();
      cout << legend << funclegend << " nz_2011: " << nz_2011 << " nsig_scale_2011: " << nsig_scale_2011 << " lumi_2011: " << lumi_2011 << " nz_2012: " << nz_2012 << " nsig_scale_2012: " << nsig_scale_2012 << "lumi_2012: " << lumi_2012 << " factorZ7over8_data: " << ((nz_2011 * nsig_scale_2011) / lumi_2011) << " / " << ( (nz_2012 * nsig_scale_2012) / lumi_2012) << endl ;

      cout << "factor factorZ7over8_data only calculated for cross checks, theory number 970/1117" << endl;
      //factorZ7over8 = ((nz_2011 * nsig_scale_2011) / lumi_2011)/( (nz_2012 * nsig_scale_2012) / lumi_2012);
      factorZ7over8 = 970./1117.;

      factorZprime7over8 = 3.5 + (-2.6) * exp(6.3e-5*zprimemass);

      factor7to8 = factorZprime7over8 / factorZ7over8;
      ws->var("nsig_scale_dimuon2011")->setVal( ws->var("nsig_scale_dimuon2011")->getVal() * factor7to8 );
   }
   if (channelname == "dielectron2011"){
      double lumi_2011 = 5000.;
      double lumi_2012 = 3600.;

      double nz_2011 = ws->var("nz_dielectron2011")->getVal();
      double nsig_scale_2011 = ws->var("nsig_scale_dielectron2011")->getVal();
      double nz_2012 = ws->var("nz_dielectron2012")->getVal();
      double nsig_scale_2012 = ws->var("nsig_scale_dielectron2012")->getVal();
      cout << legend << funclegend << " nz_2011: " << nz_2011 << " nsig_scale_2011: " << nsig_scale_2011 << " lumi_2011: " << lumi_2011 << " nz_2012: " << nz_2012 << " nsig_scale_2012: " << nsig_scale_2012 << "lumi_2012: " << lumi_2012 << " factorZ7over8_data: " << ((nz_2011 * nsig_scale_2011) / lumi_2011) << " / " << ( (nz_2012 * nsig_scale_2012) / lumi_2012) << endl ;

      cout << "factor factorZ7over8_data only calculated for cross checks, theory number 970/1117" << endl;
      //factorZ7over8 = ((nz_2011 * nsig_scale_2011) / lumi_2011)/( (nz_2012 * nsig_scale_2012) / lumi_2012);
      factorZ7over8 = 970./1117.;

      factorZprime7over8 = 3.5 + (-2.6) * exp(6.3e-5*zprimemass);

      factor7to8 = factorZprime7over8 / factorZ7over8;
      ws->var("nsig_scale_dielectron2011")->setVal( ws->var("nsig_scale_dielectron2011")->getVal() * factor7to8 );
   }

   cout << legend << funclegend << "applied factor " << factorZprime7over8 << " / " << factorZ7over8 << " = " << factor7to8 << endl;

}

void Pixie::SetupFor7and8Combination_RS(RooWorkspace * ws, std::string channelname){
   std::string funclegend = "SetupFor7and8Combination_RS(...) - ";
   funclegend += channelname;

   double zprimemass = ws->var("peak")->getVal();
   double factor7to8 = 1.0;
   double factorZprime7over8 = 1.0;
   double factorZ7over8 = 1.0;


   if (channelname == "dimuon2011"){
      double lumi_2011 = 5300.;
      double lumi_2012 = 4076.;

      double nz_2011 = ws->var("nz_dimuon2011")->getVal();
      double nsig_scale_2011 = ws->var("nsig_scale_dimuon2011")->getVal();
      double nz_2012 = ws->var("nz_dimuon2012")->getVal();
      double nsig_scale_2012 = ws->var("nsig_scale_dimuon2012")->getVal();
      cout << legend << funclegend << " nz_2011: " << nz_2011 << " nsig_scale_2011: " << nsig_scale_2011 << " lumi_2011: " << lumi_2011 << " nz_2012: " << nz_2012 << " nsig_scale_2012: " << nsig_scale_2012 << "lumi_2012: " << lumi_2012 << " factorZ7over8_data: " << ((nz_2011 * nsig_scale_2011) / lumi_2011) << " / " << ( (nz_2012 * nsig_scale_2012) / lumi_2012) << endl ;

      cout << "factor factorZ7over8_data only calculated for cross checks, theory number 970/1117" << endl;
      //factorZ7over8 = ((nz_2011 * nsig_scale_2011) / lumi_2011)/( (nz_2012 * nsig_scale_2012) / lumi_2012);
      factorZ7over8 = 970./1117.;

      factorZprime7over8 = 0.75 + (-0.0001494) * zprimemass;

      factor7to8 = factorZprime7over8 / factorZ7over8;
      ws->var("nsig_scale_dimuon2011")->setVal( ws->var("nsig_scale_dimuon2011")->getVal() * factor7to8 );
   }
   if (channelname == "dielectron2011"){
      double lumi_2011 = 5000.;
      double lumi_2012 = 3600.;

      double nz_2011 = ws->var("nz_dielectron2011")->getVal();
      double nsig_scale_2011 = ws->var("nsig_scale_dielectron2011")->getVal();
      double nz_2012 = ws->var("nz_dielectron2012")->getVal();
      double nsig_scale_2012 = ws->var("nsig_scale_dielectron2012")->getVal();
      cout << legend << funclegend << " nz_2011: " << nz_2011 << " nsig_scale_2011: " << nsig_scale_2011 << " lumi_2011: " << lumi_2011 << " nz_2012: " << nz_2012 << " nsig_scale_2012: " << nsig_scale_2012 << "lumi_2012: " << lumi_2012 << " factorZ7over8_data: " << ((nz_2011 * nsig_scale_2011) / lumi_2011) << " / " << ( (nz_2012 * nsig_scale_2012) / lumi_2012) << endl ;

      cout << "factor factorZ7over8_data only calculated for cross checks, theory number 970/1117" << endl;
      //factorZ7over8 = ((nz_2011 * nsig_scale_2011) / lumi_2011)/( (nz_2012 * nsig_scale_2012) / lumi_2012);
      factorZ7over8 = 970./1117.;

      factorZprime7over8 = 0.75 + (-0.0001494) * zprimemass;

      factor7to8 = factorZprime7over8 / factorZ7over8;
      ws->var("nsig_scale_dielectron2011")->setVal( ws->var("nsig_scale_dielectron2011")->getVal() * factor7to8 );
   }

   cout << legend << funclegend << "applied factor " << factorZprime7over8 << " / " << factorZ7over8 << " = " << factor7to8 << endl;

}

void Pixie::SetResolution_RS(RooWorkspace * ws, double kdivMplred, std::vector<std::string> channelnames){
   std::string funclegend = "SetResolution_RS(ModelConfigurator * modelconfigurator, double kdivMplred) - ";



   bool kdivMplred_ok = false;
   double p0 = 0.;
   double p1 = 0.;
   
   if (kdivMplred == 0.2){
      cout << legend << funclegend << "adjust BW FWHM for k/ M_{Pl,red} = " <<  kdivMplred << endl;
      kdivMplred_ok = true;
      p0 = 7.6;
      p1 = 0.056;
   }
   if (kdivMplred == 0.1){
      cout << legend << funclegend << "adjust BW FWHM for k/ M_{Pl,red} = " <<  kdivMplred << endl;
      kdivMplred_ok = true;
      p0 = 1.04;
      p1 = 0.017;
   }
   
   if (kdivMplred == 0.05){
      cout << legend << funclegend << "adjust BW FWHM for k/ M_{Pl,red} = " <<  kdivMplred << endl;
      kdivMplred_ok = true;
      p0 = 0.38;
      p1 = 0.0043;
   }
   if (kdivMplred == 0.01){
      cout << legend << funclegend << "adjust BW FWHM for k/ M_{Pl,red} = " <<  kdivMplred << endl;
      kdivMplred_ok = true;
      p0 = 0.1;
      p1 = 0.0001;
      //p0 = 0.1;
      //p1 = 0.0000001; 
   }

   if(kdivMplred_ok == false){
      cout << legend << funclegend << "ERROR: can not adjust BW FWHM for k/ M_{Pl,red} = " <<  kdivMplred << endl;
   }


   // width variables are shared -> not possible to set them separately for each channel
   //std::vector<std::string> channelnames = modelconfigurator->getChannelNames();
   //for( std::vector<std::string>::iterator vecIt = channelnames.begin(); vecIt != channelnames.end(); vecIt++){

      std::string p0name = "width_p0"; //Comment: unfortunately hardcoded
      //p0name += "_";
      //p0name += *vecIt;
      std::string p1name = "width_p1"; //Comment: unfortunately hardcoded
      //p1name += "_";
      //p1name +=  *vecIt;

      ws->var(p0name.c_str())->setVal(p0);
      ws->var(p1name.c_str())->setVal(p1);
      cout << legend << funclegend << "set var " << p0name << " to: " << endl << ws->var(p0name.c_str())->getVal() << endl; 
      cout << legend << funclegend << "set var " << p1name << " to: " << endl << ws->var(p1name.c_str())->getVal() << endl;
   //}
}


//this function makes a single evaluation of the likelihood -> test for finding problems in the likelihood evaluation
void Pixie::CheckLikelihood(RooWorkspace * ws, ModelConfig * modelconfig){
cout << "not yet implemented ..." << endl;
}

void Pixie::SetResolution_ZPSI(RooWorkspace * ws, std::vector<std::string> channelnames){
   std::string funclegend = "SetResolution_ZPSI(ModelConfigurator * modelconfigurator, std::vector<std::string> channelnames) - ";

   double p0 = 0.;
   double p1 = 0.;
   cout << legend << funclegend << "adjust BW FWHM for Z_PSI "<< endl;

   //c=001
   //p0 = 0.03765;//0.0;
   //p1 = 0.000128;//0.006;

   //c=005
   //p0 = 0.02187;
   //p1 = 0.003549; 

   //c=01
   p0 = 0.375;
   p1 = 0.01402;

   std::string p0name = "width_p0"; //Comment: unfortunately hardcoded
   //p0name += "_";
   //p0name += *vecIt;
   std::string p1name = "width_p1"; //Comment: unfortunately hardcoded

   cout << p0name << " is: " << ws->var(p0name.c_str())->getVal() << endl;
   cout << p1name << " is: " << ws->var(p1name.c_str())->getVal() << endl;
   //p1name += "_";
   //p1name +=  *vecIt;

   ws->var(p0name.c_str())->setVal(p0);
   ws->var(p1name.c_str())->setVal(p1);
   cout << legend << funclegend << "set var " << p0name << " to: " << endl << ws->var(p0name.c_str())->getVal() << endl; 
   cout << legend << funclegend << "set var " << p1name << " to: " << endl << ws->var(p1name.c_str())->getVal() << endl;
}
