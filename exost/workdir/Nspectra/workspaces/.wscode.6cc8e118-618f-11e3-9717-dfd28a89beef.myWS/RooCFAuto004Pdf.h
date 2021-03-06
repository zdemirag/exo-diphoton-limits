/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/

#ifndef ROOCFAUTO004PDF
#define ROOCFAUTO004PDF

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"
 
class RooCFAuto004Pdf : public RooAbsPdf {
public:
  RooCFAuto004Pdf() {} ; 
  RooCFAuto004Pdf(const char *name, const char *title,
	      RooAbsReal& _a,
	      RooAbsReal& _mass,
	      RooAbsReal& _b,
	      RooAbsReal& _c,
	      RooAbsReal& _d);
  RooCFAuto004Pdf(const RooCFAuto004Pdf& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooCFAuto004Pdf(*this,newname); }
  inline virtual ~RooCFAuto004Pdf() { }

protected:

  RooRealProxy a ;
  RooRealProxy mass ;
  RooRealProxy b ;
  RooRealProxy c ;
  RooRealProxy d ;
  
  Double_t evaluate() const ;

private:

  ClassDef(RooCFAuto004Pdf,1) // Your description goes here...
};
 
#endif
