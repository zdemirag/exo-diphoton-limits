/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooCFAuto011Func.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 

ClassImp(RooCFAuto011Func) 

 RooCFAuto011Func::RooCFAuto011Func(const char *name, const char *title, 
                        RooAbsReal& _res_a,
                        RooAbsReal& _peak,
                        RooAbsReal& _res_b,
                        RooAbsReal& _res_c) :
   RooAbsReal(name,title), 
   res_a("res_a","res_a",this,_res_a),
   peak("peak","peak",this,_peak),
   res_b("res_b","res_b",this,_res_b),
   res_c("res_c","res_c",this,_res_c)
 { 
 } 


 RooCFAuto011Func::RooCFAuto011Func(const RooCFAuto011Func& other, const char* name) :  
   RooAbsReal(other,name), 
   res_a("res_a",this,other.res_a),
   peak("peak",this,other.peak),
   res_b("res_b",this,other.res_b),
   res_c("res_c",this,other.res_c)
 { 
 } 



 Double_t RooCFAuto011Func::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   return 0.01*sqrt(res_a*res_a/peak/peak+res_b*res_b/peak+res_c) ; 
 } 



