#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1D.h"
#include "TH1F.h"
#include "TH2D.h"
#include "TH2F.h"
#include "TMath.h"
#include "TVector2.h"
#include "TGraphErrors.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>

int main(int argc, char** argv) {

  if( argc<3 ) {
     std::cout << argv[0] << ":  " << std::endl ;
     std::cout << " Functionality: skimming... "  << std::endl;
     std::cout << "                 "  << std::endl;
     std::cout << " usage: " << argv[0] << " inputfile.root outputfile.root" << std::endl ;
     exit(1) ;
  }

  // input file name
  std::string inputfile((const char*)argv[1]); 
  // output file name
  std::string outputfile((const char*)argv[2]);
  // initialize
  // root files
  TFile* finput = new TFile(inputfile.c_str());
  TFile* foutput = new TFile(outputfile.c_str(), "recreate");
  // tree
  TTree* tree = (TTree*)finput->Get("tree");
  // out_tree
  TTree* tree_out = tree->CloneTree(0);

  TFile* ftkhp = new TFile("/data2/yanchu/informations/leptoneff/lpt76x/all76.root");
  TH1F* tkhpetadt=(TH1F*)ftkhp->Get("tkhpdtetahist");
  TH1F* tkhpetamc=(TH1F*)ftkhp->Get("tkhpmcetahist");
  TH1F* hpetadt=(TH1F*)ftkhp->Get("hpdtetahist");
  TH1F* hpetamc=(TH1F*)ftkhp->Get("hpmcetahist");
  TH1F* isoeta=(TH1F*)ftkhp->Get("tkissfeta");

  TFile* ftrg = new TFile("/data2/yanchu/informations/triggereff/trg76x/triggereff76x.root");
  TH2D* mul1ptetadata=(TH2D*)ftrg->Get("mul1ptetadata");
  TH2D* mul2ptetadata=(TH2D*)ftrg->Get("mul2ptetadata");
  TH2D* ell1ptetadata=(TH2D*)ftrg->Get("ell1ptetadata");
  TH2D* mul1ptetamc=(TH2D*)ftrg->Get("mul1ptetamc");
  TH2D* mul2ptetamc=(TH2D*)ftrg->Get("mul2ptetamc");
  TH2D* ell1ptetamc=(TH2D*)ftrg->Get("ell1ptetamc");
  TH2D* ell1pteta=(TH2D*)ell1ptetadata->Clone("ell1pteta");
  ell1pteta->Divide(ell1ptetamc);

  TFile* loosE = new TFile("/afs/cern.ch/work/y/yanchu/graviton/CMSSW_7_6_3_patch2/src/CMGTools/XZZ2l2nu/data/CutBasedID_LooseWP_76X_18Feb.txt_SF2D.root");
  TH2F* esfh2=(TH2F*)loosE->Get("EGamma_SF2D");

  Double_t effdt1a,effdt2a,effmc1a,effmc2a,errdt1a,errdt2a,errmc1a,errmc2a,effdt1,effdt2,effmc1,effmc2,errdt1,errdt2,errmc1,errmc2,trgsfall,idsfall,isosfall,trgsfallerr,idsfallerr,isosfallerr,temp1,temp2;
  TBranch *b_trgsfall=tree_out->Branch("trgsf",&trgsfall,"trgsf/D");
  TBranch *b_isosfall=tree_out->Branch("isosf",&isosfall,"isosf/D");
  TBranch *b_idsfall=tree_out->Branch("idsf",&idsfall,"idsf/D");
  TBranch *b_trgsfallerr=tree_out->Branch("trgsf_err",&trgsfallerr,"trgsf_err/D");
  TBranch *b_isosfallerr=tree_out->Branch("isosf_err",&isosfallerr,"isosf_err/D");
  TBranch *b_idsfallerr=tree_out->Branch("idsf_err",&idsfallerr,"idsf_err/D");

  Float_t llnunu_l1_l1_phi, llnunu_l1_l1_eta, llnunu_l1_l2_phi, llnunu_l1_l2_eta,llnunu_l1_l1_pt,llnunu_l1_l2_pt,llnunu_l1_l1_eSCeta,llnunu_l1_l2_eSCeta;
  int lpdgid;
  tree->SetBranchAddress("llnunu_l1_l1_pdgId",&lpdgid);
  tree->SetBranchAddress("llnunu_l1_l1_phi",&llnunu_l1_l1_phi);
  tree->SetBranchAddress("llnunu_l1_l2_phi",&llnunu_l1_l2_phi);
  tree->SetBranchAddress("llnunu_l1_l1_pt",&llnunu_l1_l1_pt);
  tree->SetBranchAddress("llnunu_l1_l2_pt",&llnunu_l1_l2_pt);
  tree->SetBranchAddress("llnunu_l1_l1_eta",&llnunu_l1_l1_eta);
  tree->SetBranchAddress("llnunu_l1_l2_eta",&llnunu_l1_l2_eta);
  tree->SetBranchAddress("llnunu_l1_l1_eSCeta",&llnunu_l1_l1_eSCeta);
  tree->SetBranchAddress("llnunu_l1_l2_eSCeta",&llnunu_l1_l2_eSCeta);

  for (int i=0; i<(int)tree->GetEntries(); i++){
    tree->GetEntry(i);
    if(abs(lpdgid)==13){
      effdt1=tkhpetadt->GetBinContent(tkhpetadt->FindBin(llnunu_l1_l1_eta));
      effmc1=tkhpetamc->GetBinContent(tkhpetamc->FindBin(llnunu_l1_l1_eta));
      errdt1=tkhpetadt->GetBinError(tkhpetadt->FindBin(llnunu_l1_l1_eta));
      errmc1=tkhpetamc->GetBinError(tkhpetamc->FindBin(llnunu_l1_l1_eta));
      effdt1a=hpetadt->GetBinContent(hpetadt->FindBin(llnunu_l1_l1_eta));
      effmc1a=hpetamc->GetBinContent(hpetamc->FindBin(llnunu_l1_l1_eta));
      errdt1a=hpetadt->GetBinError(hpetadt->FindBin(llnunu_l1_l1_eta));
      errmc1a=hpetamc->GetBinError(hpetamc->FindBin(llnunu_l1_l1_eta));
      effdt2=tkhpetadt->GetBinContent(tkhpetadt->FindBin(llnunu_l1_l2_eta));
      effmc2=tkhpetamc->GetBinContent(tkhpetamc->FindBin(llnunu_l1_l2_eta));
      errdt2=tkhpetadt->GetBinError(tkhpetadt->FindBin(llnunu_l1_l2_eta));
      errmc2=tkhpetamc->GetBinError(tkhpetamc->FindBin(llnunu_l1_l2_eta));
      effdt2a=hpetadt->GetBinContent(hpetadt->FindBin(llnunu_l1_l2_eta));
      effmc2a=hpetamc->GetBinContent(hpetamc->FindBin(llnunu_l1_l2_eta));
      errdt2a=hpetadt->GetBinError(hpetadt->FindBin(llnunu_l1_l2_eta));
      errmc2a=hpetamc->GetBinError(hpetamc->FindBin(llnunu_l1_l2_eta));
      temp1=effdt1*effdt2a+effdt1a*effdt2-effdt1a*effdt2a;
      temp2=effmc1*effmc2a+effmc1a*effmc2-effmc1a*effmc2a;
      if(temp1==0||temp2==0){
	idsfall=1;
	idsfallerr=0;}
      else{
      idsfall=temp1/temp2;
      idsfallerr=(TMath::Power((effdt2-effdt2a)*errdt1a,2)+TMath::Power((effdt1-effdt1a)*errdt2a,2)+TMath::Power(effdt1a*errdt2,2)+TMath::Power(effdt2a*errdt1,2))/TMath::Power(temp1,2)+(TMath::Power((effmc2-effmc2a)*errmc1a,2)+TMath::Power((effmc1-effmc1a)*errmc2a,2)+TMath::Power(effmc1a*errmc2,2)+TMath::Power(effmc2a*errmc1,2))/TMath::Power(temp2,2);
      idsfallerr=TMath::Power(idsfallerr,.5)*idsfall;
      }
      effdt1=isoeta->GetBinContent(isoeta->FindBin(llnunu_l1_l1_eta));
      effdt2=isoeta->GetBinContent(isoeta->FindBin(llnunu_l1_l2_eta));
      errdt1=isoeta->GetBinError(isoeta->FindBin(llnunu_l1_l1_eta));
      errdt2=isoeta->GetBinError(isoeta->FindBin(llnunu_l1_l2_eta));
      isosfall=effdt1*effdt2;
      isosfallerr=TMath::Power((TMath::Power(effdt1*errdt2,2)+TMath::Power(errdt1*effdt2,2)),.5);
      if(isosfall==0)isosfall==1;
      effdt1=mul1ptetadata->GetBinContent(mul1ptetadata->FindBin(llnunu_l1_l1_pt,abs(llnunu_l1_l1_eta)));
      effdt2=mul2ptetadata->GetBinContent(mul2ptetadata->FindBin(llnunu_l1_l2_pt,abs(llnunu_l1_l2_eta)));
      errdt1=mul1ptetadata->GetBinError(mul1ptetadata->FindBin(llnunu_l1_l1_pt,abs(llnunu_l1_l1_eta)));
      errdt2=mul2ptetadata->GetBinError(mul2ptetadata->FindBin(llnunu_l1_l2_pt,abs(llnunu_l1_l2_eta)));
      effmc1=mul1ptetamc->GetBinContent(mul1ptetamc->FindBin(llnunu_l1_l1_pt,abs(llnunu_l1_l1_eta)));
      effmc2=mul2ptetamc->GetBinContent(mul2ptetamc->FindBin(llnunu_l1_l2_pt,abs(llnunu_l1_l2_eta)));
      errmc1=mul1ptetamc->GetBinError(mul1ptetamc->FindBin(llnunu_l1_l1_pt,abs(llnunu_l1_l1_eta)));
      errmc2=mul2ptetamc->GetBinError(mul2ptetamc->FindBin(llnunu_l1_l2_pt,abs(llnunu_l1_l2_eta)));
      temp1=effdt1+effdt2-effdt1*effdt2;
      temp2=effmc1+effmc2-effmc1*effmc2;
      trgsfall=temp1/temp2;
      trgsfallerr=(TMath::Power((1-effdt1)*errdt2,2)+TMath::Power((1-effdt2)*errdt1,2))/TMath::Power(temp1,2)+(TMath::Power((1-effmc1)*errmc2,2)+TMath::Power((1-effmc2)*errmc1,2))/TMath::Power(temp2,2);
      trgsfallerr=TMath::Power(trgsfallerr,.5)*trgsfall;
      if(trgsfall==0)trgsfall=1;
    }
    if(abs(lpdgid)==11){
      effdt1=esfh2->GetBinContent(esfh2->FindBin(abs(llnunu_l1_l1_eSCeta),llnunu_l1_l1_pt));
      effdt2=esfh2->GetBinContent(esfh2->FindBin(abs(llnunu_l1_l2_eSCeta),llnunu_l1_l2_pt));
      errdt1=esfh2->GetBinError(esfh2->FindBin(abs(llnunu_l1_l1_eSCeta),llnunu_l1_l1_pt));
      errdt2=esfh2->GetBinError(esfh2->FindBin(abs(llnunu_l1_l2_eSCeta),llnunu_l1_l2_pt));
      idsfall=effdt1*effdt2;
      idsfallerr=TMath::Power((TMath::Power(effdt1*errdt2,2)+TMath::Power(errdt1*effdt2,2)),.5);
      if(isosfall==0)isosfall==1;
      isosfall=1;
      isosfallerr=0;
      trgsfall=ell1pteta->GetBinContent(ell1pteta->FindBin(llnunu_l1_l1_pt,abs(llnunu_l1_l1_eta)));
      trgsfallerr=ell1pteta->GetBinError(ell1pteta->FindBin(llnunu_l1_l1_pt,abs(llnunu_l1_l1_eta)));
      if(trgsfall==0)trgsfall=1;
    }

    tree_out->Fill();

  }

  foutput->cd();
  tree_out->Write();
  foutput->Close();
  finput->Close();

  return 0;

}



