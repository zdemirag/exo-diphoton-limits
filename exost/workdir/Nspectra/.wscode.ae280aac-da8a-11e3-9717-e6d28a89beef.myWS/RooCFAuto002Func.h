/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOCFAUTO002FUNC
#define ROOCFAUTO002FUNC

#include "RooAbsReal.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class RooCFAuto002Func : public RooAbsReal {
public:
  RooCFAuto002Func() {} ; 
  RooCFAuto002Func(const char *name, const char *title,
	      RooAbsReal& _peak,
	      RooAbsReal& _eff_a,
	      RooAbsReal& _eff_b);
  RooCFAuto002Func(const RooCFAuto002Func& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooCFAuto002Func(*this,newname); }
  inline virtual ~RooCFAuto002Func() { }

protected:

  RooRealProxy peak ;
  RooRealProxy eff_a ;
  RooRealProxy eff_b ;
  
  Double_t evaluate() const ;

private:

  ClassDef(RooCFAuto002Func,1) // Your description goes here...
};
 
#endif
