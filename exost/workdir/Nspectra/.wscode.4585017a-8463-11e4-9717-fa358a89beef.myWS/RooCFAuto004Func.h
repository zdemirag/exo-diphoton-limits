/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOCFAUTO004FUNC
#define ROOCFAUTO004FUNC

#include "RooAbsReal.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class RooCFAuto004Func : public RooAbsReal {
public:
  RooCFAuto004Func() {} ; 
  RooCFAuto004Func(const char *name, const char *title,
	      RooAbsReal& _nsig_kappa,
	      RooAbsReal& _beta_nsig);
  RooCFAuto004Func(const RooCFAuto004Func& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooCFAuto004Func(*this,newname); }
  inline virtual ~RooCFAuto004Func() { }

protected:

  RooRealProxy nsig_kappa ;
  RooRealProxy beta_nsig ;
  
  Double_t evaluate() const ;

private:

  ClassDef(RooCFAuto004Func,1) // Your description goes here...
};
 
#endif
