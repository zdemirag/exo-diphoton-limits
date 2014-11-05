/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOCFAUTO005FUNC
#define ROOCFAUTO005FUNC

#include "RooAbsReal.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class RooCFAuto005Func : public RooAbsReal {
public:
  RooCFAuto005Func() {} ; 
  RooCFAuto005Func(const char *name, const char *title,
	      RooAbsReal& _nsig_kappa,
	      RooAbsReal& _beta_nsig);
  RooCFAuto005Func(const RooCFAuto005Func& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooCFAuto005Func(*this,newname); }
  inline virtual ~RooCFAuto005Func() { }

protected:

  RooRealProxy nsig_kappa ;
  RooRealProxy beta_nsig ;
  
  Double_t evaluate() const ;

private:

  ClassDef(RooCFAuto005Func,1) // Your description goes here...
};
 
#endif