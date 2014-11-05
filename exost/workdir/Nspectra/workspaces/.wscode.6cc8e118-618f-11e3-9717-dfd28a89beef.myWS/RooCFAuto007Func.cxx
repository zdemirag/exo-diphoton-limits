/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

// class declaration include file below retrieved from workspace code storage
#include "RooCFAuto007Func.h"
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 

ClassImp(RooCFAuto007Func) 

 RooCFAuto007Func::RooCFAuto007Func(const char *name, const char *title, 
                        RooAbsReal& _nbkg_kappa,
                        RooAbsReal& _beta_nbkg) :
   RooAbsReal(name,title), 
   nbkg_kappa("nbkg_kappa","nbkg_kappa",this,_nbkg_kappa),
   beta_nbkg("beta_nbkg","beta_nbkg",this,_beta_nbkg)
 { 
 } 


 RooCFAuto007Func::RooCFAuto007Func(const RooCFAuto007Func& other, const char* name) :  
   RooAbsReal(other,name), 
   nbkg_kappa("nbkg_kappa",this,other.nbkg_kappa),
   beta_nbkg("beta_nbkg",this,other.beta_nbkg)
 { 
 } 



 Double_t RooCFAuto007Func::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   return pow(nbkg_kappa,beta_nbkg) ; 
 } 



