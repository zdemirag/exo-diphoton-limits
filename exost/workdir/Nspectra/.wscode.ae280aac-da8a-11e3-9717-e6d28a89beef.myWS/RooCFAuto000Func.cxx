/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

// class declaration include file below retrieved from workspace code storage
#include "RooCFAuto000Func.h"
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 

ClassImp(RooCFAuto000Func) 

 RooCFAuto000Func::RooCFAuto000Func(const char *name, const char *title, 
                        RooAbsReal& _peak,
                        RooAbsReal& _eff_a,
                        RooAbsReal& _eff_b) :
   RooAbsReal(name,title), 
   peak("peak","peak",this,_peak),
   eff_a("eff_a","eff_a",this,_eff_a),
   eff_b("eff_b","eff_b",this,_eff_b)
 { 
 } 


 RooCFAuto000Func::RooCFAuto000Func(const RooCFAuto000Func& other, const char* name) :  
   RooAbsReal(other,name), 
   peak("peak",this,other.peak),
   eff_a("eff_a",this,other.eff_a),
   eff_b("eff_b",this,other.eff_b)
 { 
 } 



 Double_t RooCFAuto000Func::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   return peak*exp(eff_a+eff_b*peak) ; 
 } 



