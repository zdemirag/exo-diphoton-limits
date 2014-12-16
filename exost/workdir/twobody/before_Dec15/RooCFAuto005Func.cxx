/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooCFAuto005Func.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 

ClassImp(RooCFAuto005Func) 

 RooCFAuto005Func::RooCFAuto005Func(const char *name, const char *title, 
                        RooAbsReal& _nbkg_kappa_b,
                        RooAbsReal& _nbkg_kappa_m,
                        RooAbsReal& _mass,
                        RooAbsReal& _nbkg_kappa_msquared,
                        RooAbsReal& _beta_nbkg) :
   RooAbsReal(name,title), 
   nbkg_kappa_b("nbkg_kappa_b","nbkg_kappa_b",this,_nbkg_kappa_b),
   nbkg_kappa_m("nbkg_kappa_m","nbkg_kappa_m",this,_nbkg_kappa_m),
   mass("mass","mass",this,_mass),
   nbkg_kappa_msquared("nbkg_kappa_msquared","nbkg_kappa_msquared",this,_nbkg_kappa_msquared),
   beta_nbkg("beta_nbkg","beta_nbkg",this,_beta_nbkg)
 { 
 } 


 RooCFAuto005Func::RooCFAuto005Func(const RooCFAuto005Func& other, const char* name) :  
   RooAbsReal(other,name), 
   nbkg_kappa_b("nbkg_kappa_b",this,other.nbkg_kappa_b),
   nbkg_kappa_m("nbkg_kappa_m",this,other.nbkg_kappa_m),
   mass("mass",this,other.mass),
   nbkg_kappa_msquared("nbkg_kappa_msquared",this,other.nbkg_kappa_msquared),
   beta_nbkg("beta_nbkg",this,other.beta_nbkg)
 { 
 } 



 Double_t RooCFAuto005Func::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   return pow(nbkg_kappa_b+nbkg_kappa_m*mass+nbkg_kappa_msquared*mass*mass,beta_nbkg) ; 
 } 



