#include "TF1.h"
#include "TFile.h"
#include "TTree.h"

Float_t  Efficiency(Float_t _MASS, Float_t _KMPL) { // 0.236
  TF1 * f_mweff = new TF1("f_mweff","pol2",150,2250);
  if(_KMPL<  0.011) {
    f_mweff->SetParameters(0.959349, -6.57673e-05, 2.88438e-08);
  } else if(_KMPL<  0.051) {
    f_mweff->SetParameters(0.946474, -3.15184e-05, 1.1381e-08);
  } else {
    f_mweff->SetParameters(0.952185, -1.81127e-05, 4.14634e-09);
  }
  TF1 * f_eff = new TF1("f_eff","pol4",250,2000);
  f_eff->SetParameters(0.21, 0.000207, 8.44e-08, -6.79e-11, 1.14e-14);
  Float_t efficiency = 0.97*0.97*f_eff->Eval(_MASS)*f_mweff->Eval(_MASS);
  delete f_eff;
  delete f_mweff;
  return efficiency;
}


Float_t Background(Float_t _LOWMASS) { // 3422
  TF1 * fmc = new
TF1("fmc","TMath::Exp([0]+[1]*x+[2]*x*x+[3]*x*x*x+[4]*x*x*x*x)",200,2000);
  fmc->SetParameters(4.8952898e+00, -3.82985e-02, 4.21476e-05, -2.31767e-08,
4.68418e-12);
  Float_t background_estimate = fmc->Integral(_LOWMASS,2400);
  delete fmc;
  return background_estimate;
}

Float_t LowMass(Float_t _MASS, Float_t _KMPL) { // 2.3638
  TF1 * f_sigeff = new TF1("f_sigeff","pol2",150,2250);
  if(_KMPL<  0.011) {
    f_sigeff->SetParameters( 0.512502, 0.00864582, 7.98281e-07);
  } else if(_KMPL<  0.051) {
    f_sigeff->SetParameters( 0.864847, 0.00959414, 1.37907e-06);
  } else {
    f_sigeff->SetParameters( 3.14934, 0.0138367, 2.25437e-06);
  }
  Float_t sigma_effective = f_sigeff->Eval(_MASS);
  delete f_sigeff;
  Float_t lowmass = _MASS - 5*sigma_effective;
  return lowmass;
}

Int_t  Data(Float_t _LOWMASS) {
  TFile*         fileData = new TFile("tardir/data361cmssw_diphoton.root");
  TTree*         treeData = (TTree*)fileData->Get("tDiphoton");
  double minv;
  TBranch * b_minv;
  treeData->SetBranchAddress("minv",&minv,&b_minv);
  b_minv = treeData->GetBranch("minv");
  Long64_t nentries = treeData->GetEntriesFast();
  Int_t data=0;
  for(int jentry=0;jentry!=nentries;++jentry) {
    b_minv->GetEntry(jentry);
    if(minv>  _LOWMASS&&  minv<  2400) {
      data+=1;
    }
  }
  fileData->Close();
  return data;
} 
