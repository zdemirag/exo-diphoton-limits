/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

// class declaration include file below retrieved from workspace code storage
#include "RooCFAuto004Func.h"
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 

ClassImp(RooCFAuto004Func) 

 RooCFAuto004Func::RooCFAuto004Func(const char *name, const char *title, 
                        RooAbsReal& _nsig_kappa,
                        RooAbsReal& _beta_nsig) :
   RooAbsReal(name,title), 
   nsig_kappa("nsig_kappa","nsig_kappa",this,_nsig_kappa),
   beta_nsig("beta_nsig","beta_nsig",this,_beta_nsig)
 { 
 } 


 RooCFAuto004Func::RooCFAuto004Func(const RooCFAuto004Func& other, const char* name) :  
   RooAbsReal(other,name), 
   nsig_kappa("nsig_kappa",this,other.nsig_kappa),
   beta_nsig("beta_nsig",this,other.beta_nsig)
 { 
 } 



 Double_t RooCFAuto004Func::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   return pow(nsig_kappa,beta_nsig) ; 
 } 



