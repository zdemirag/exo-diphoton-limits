/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOCFAUTO011FUNC
#define ROOCFAUTO011FUNC

#include "RooAbsReal.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class RooCFAuto011Func : public RooAbsReal {
public:
  RooCFAuto011Func() {} ; 
  RooCFAuto011Func(const char *name, const char *title,
	      RooAbsReal& _res_a,
	      RooAbsReal& _peak,
	      RooAbsReal& _res_b,
	      RooAbsReal& _res_c);
  RooCFAuto011Func(const RooCFAuto011Func& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooCFAuto011Func(*this,newname); }
  inline virtual ~RooCFAuto011Func() { }

protected:

  RooRealProxy res_a ;
  RooRealProxy peak ;
  RooRealProxy res_b ;
  RooRealProxy res_c ;
  
  Double_t evaluate() const ;

private:

  ClassDef(RooCFAuto011Func,1) // Your description goes here...
};
 
#endif
