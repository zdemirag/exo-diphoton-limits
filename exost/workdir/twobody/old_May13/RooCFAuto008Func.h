/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOCFAUTO008FUNC
#define ROOCFAUTO008FUNC

#include "RooAbsReal.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class RooCFAuto008Func : public RooAbsReal {
public:
  RooCFAuto008Func() {} ; 
  RooCFAuto008Func(const char *name, const char *title,
	      RooAbsReal& _mass_kappa,
	      RooAbsReal& _beta_mass);
  RooCFAuto008Func(const RooCFAuto008Func& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooCFAuto008Func(*this,newname); }
  inline virtual ~RooCFAuto008Func() { }

protected:

  RooRealProxy mass_kappa ;
  RooRealProxy beta_mass ;
  
  Double_t evaluate() const ;

private:

  ClassDef(RooCFAuto008Func,1) // Your description goes here...
};
 
#endif