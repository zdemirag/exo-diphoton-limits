/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooCFAuto002Pdf.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 

ClassImp(RooCFAuto002Pdf) 

 RooCFAuto002Pdf::RooCFAuto002Pdf(const char *name, const char *title, 
                        RooAbsReal& _a,
                        RooAbsReal& _mass,
                        RooAbsReal& _b,
                        RooAbsReal& _c,
                        RooAbsReal& _d) :
   RooAbsPdf(name,title), 
   a("a","a",this,_a),
   mass("mass","mass",this,_mass),
   b("b","b",this,_b),
   c("c","c",this,_c),
   d("d","d",this,_d)
 { 
 } 


 RooCFAuto002Pdf::RooCFAuto002Pdf(const RooCFAuto002Pdf& other, const char* name) :  
   RooAbsPdf(other,name), 
   a("a",this,other.a),
   mass("mass",this,other.mass),
   b("b",this,other.b),
   c("c",this,other.c),
   d("d",this,other.d)
 { 
 } 



 Double_t RooCFAuto002Pdf::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   return (a*pow((1-mass/8000),b))/pow(mass/8000,c+d*log(mass/8000)) ; 
 } 


