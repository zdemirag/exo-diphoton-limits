/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOCFAUTO007FUNC
#define ROOCFAUTO007FUNC

#include "RooAbsReal.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class RooCFAuto007Func : public RooAbsReal {
public:
  RooCFAuto007Func() {} ; 
  RooCFAuto007Func(const char *name, const char *title,
	      RooAbsReal& _mass_kappa,
	      RooAbsReal& _beta_mass);
  RooCFAuto007Func(const RooCFAuto007Func& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooCFAuto007Func(*this,newname); }
  inline virtual ~RooCFAuto007Func() { }

protected:

  RooRealProxy mass_kappa ;
  RooRealProxy beta_mass ;
  
  Double_t evaluate() const ;

private:

  ClassDef(RooCFAuto007Func,1) // Your description goes here...
};
 
#endif