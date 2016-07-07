#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TLegend.h"
#include "TROOT.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TProfile2D.h"
#include "TF1.h"
#include "TPaveText.h"
#include "TObjArray.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TVector2.h"
#include "TProfile.h"
#include "TGraphErrors.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "kinzptfitv4.h"
#include "JetResolution.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "Minuit2/MinimumBuilder.h"
#include "Math/MinimizerOptions.h"



bool do2016 = true;
bool doData = false;
bool doZpTCorr = false;
bool doJetsCorr = true;
bool doJetsCorrUseLepRes=true;
bool doJetsCorrUseLepResPtErr=true;
bool doJetsCorrUseLepResPtErrJetLep=true;
bool doMetShift = false;
bool doMetShiftAfter = false;
bool doMetSigma = false;
bool doPfLepCorr = false;
bool doPfLepCorrUseTruth=false;

int n_start = 0; //808575;//612088;//599938;//95849;//62859;
int n_interval = 1000;
int n_test = 10;
Float_t _zpt_cut_min = -1;
Float_t _zpt_cut_max = 1000000;
bool doHardOnly = true;
bool doSignalProtection = false;
bool useRaw = false;

std::string fitOption = 
//"useMetShift";
"default";

int jetSelOption = 8;
// 1: first version, with a met_para cut
// 2: using sum(pt_jets) + z_pt = 0 to select jets
// 3: using sum(pt_jets) + met_para + z_pt = 0 to select jets
// 4: using sum(pt_jets) + met_para + z_pt = 0 to select jets, remove request of positive met_para and small met_perp fraction
// 5: using sum(pt_jets) + met_para + z_pt = 0 to select jets, select all jets passed id and lepton veto
// 6: using sum(pt_jets) + met_para + z_pt = 0 to select jets, allow lepton jets in the fit, remove their para from zpt.
// 7: take all tight id jets except leptons.
// 8: take all thght id jets inlcuding leptons, will be split to a jet and a lepton for the fit.

int max_njet = 5;
int Opt = 0;
// 0: "Default" : both +/- allow to vary
// 1: "BackJets" : only allow jets back to the z boost to vary
// 2: "BackBig" : for jets back to the z, only increase jec allowed, for jets same side to the z, only decrease jec allowed
// 21: "BackBigLessConstr" : same as BackBig, but less jer constraint, 1-sigma allowed.
// 3: "DefaultSmallVariation" : both +/- allow to vary, 
// 31 : "DefaultTwoJERVariation" : both +/- allow to vary, 
// 32 : "DefaultThreeJERVariation" : both +/- allow to vary, 

//double jer_scale = 0.894823*0.894823;
Float_t jer_scale = 1.0;


int main(int argc, char** argv) {

  if( argc<3 ) {
     std::cout << argv[0] << ":  " << std::endl ;
     std::cout << " Functionality: apply zpt fit ... "  << std::endl;
     std::cout << "                 "  << std::endl;
     std::cout << " usage: " << argv[0] << " inputfile.root outputfile.root " << std::endl ;
     exit(1) ;
  }

  // input file name
  std::string inputfile((const char*)argv[1]); 
  // output file name
  std::string outputfile((const char*)argv[2]);


  //
  char name[1000];

  //
  gROOT->ProcessLine(".x tdrstyle.C");


  std::string lumiTag = "CMS 13 TeV Simulation for 2015 Data";
  if (doData&&do2016) lumiTag = "CMS 13 TeV 2016B L=3.99 fb^{-1}";
  else if (doData&&!do2016) lumiTag = "CMS 13 TeV 2015 L=2.32 fb^{-1}";
  else if (!doData&&do2016) lumiTag = "CMS 13 TeV Simulation for 2016 Data";

  TPaveText* lumipt = new TPaveText(0.2,0.91,0.9,0.96,"brNDC");
  lumipt->SetBorderSize(0);
  lumipt->SetTextAlign(12);
  lumipt->SetFillStyle(0);
  lumipt->SetTextFont(42);
  lumipt->SetTextSize(0.03);
  lumipt->AddText(0.15,0.15, lumiTag.c_str());



  // initialize
  // root files
  TFile* finput = new TFile(inputfile.c_str());
  TFile* foutput = new TFile(outputfile.c_str(), "recreate");

  TCanvas* plots = new TCanvas("plots", "plots");
  sprintf(name, "%s.ps[", outputfile.c_str());
  plots->Print(name);
 
  
  // tree
  TTree* tree = (TTree*)finput->Get("tree");

  // out_tree
  TTree* tree_out = tree->CloneTree(0);

  Int_t isData;
  tree->SetBranchAddress("isData",&isData);
  ULong64_t evt;
  tree->SetBranchAddress("evt",&evt);

  Float_t llnunu_l1_pt, llnunu_l2_pt, llnunu_l1_phi, llnunu_l2_phi, llnunu_l1_eta, llnunu_l2_eta, llnunu_l1_mass;
  Float_t llnunu_l1_l1_pt, llnunu_l1_l1_phi, llnunu_l1_l1_eta;
  Float_t llnunu_l1_l2_pt, llnunu_l1_l2_phi, llnunu_l1_l2_eta;
  Int_t   llnunu_l1_l1_pdgId, llnunu_l1_l2_pdgId;
  Float_t   llnunu_l1_l1_ptErr, llnunu_l1_l2_ptErr;
  Float_t llnunu_l2_rawPt, llnunu_l2_rawPhi;
  Float_t llnunu_mt;
  Float_t llnunu_deltaPhi, llnunu_CosdphiZMet, llnunu_dPTPara, llnunu_dPTParaRel, llnunu_dPTPerp, llnunu_dPTPerpRel;
  Float_t llnunu_l1_deltaPhi;
  Int_t njet, jet_id[1000];
  Float_t jet_pt[1000], jet_phi[1000], jet_eta[1000], jet_rawPt[1000];
  Float_t jet_chargedHadronEnergyFraction[1000], jet_neutralHadronEnergyFraction[1000], jet_neutralEmEnergyFraction[1000];
  Float_t jet_muonEnergyFraction[1000], jet_chargedEmEnergyFraction[1000], jet_chargedHadronMultiplicity[1000];
  Float_t jet_chargedMultiplicity[1000], jet_neutralMultiplicity[1000];
  Int_t nTrueInt, nVert;
  Float_t rho;
  Int_t ngenZ;
  Float_t genZ_pt[10],genZ_phi[10],genZ_eta[10];
  Int_t ngenLep, genLep_pdgId[100];
  Float_t genLep_pt[100], genLep_phi[100], genLep_eta[100];
 
  tree->SetBranchAddress("llnunu_l1_pt",&llnunu_l1_pt);
  tree->SetBranchAddress("llnunu_l1_phi",&llnunu_l1_phi);
  tree->SetBranchAddress("llnunu_l1_eta",&llnunu_l1_eta);
  tree->SetBranchAddress("llnunu_l2_pt",&llnunu_l2_pt);
  tree->SetBranchAddress("llnunu_l2_phi",&llnunu_l2_phi);
  tree->SetBranchAddress("llnunu_l2_eta",&llnunu_l2_eta);
  tree->SetBranchAddress("llnunu_l2_rawPt",&llnunu_l2_rawPt);
  tree->SetBranchAddress("llnunu_l2_rawPhi",&llnunu_l2_rawPhi);
  tree->SetBranchAddress("llnunu_l1_mass",&llnunu_l1_mass);
  tree->SetBranchAddress("llnunu_l1_deltaPhi",&llnunu_l1_deltaPhi);
  tree->SetBranchAddress("llnunu_l1_l1_pt",&llnunu_l1_l1_pt);
  tree->SetBranchAddress("llnunu_l1_l2_pt",&llnunu_l1_l2_pt);
  tree->SetBranchAddress("llnunu_l1_l1_phi",&llnunu_l1_l1_phi);
  tree->SetBranchAddress("llnunu_l1_l2_phi",&llnunu_l1_l2_phi);
  tree->SetBranchAddress("llnunu_l1_l1_eta",&llnunu_l1_l1_eta);
  tree->SetBranchAddress("llnunu_l1_l2_eta",&llnunu_l1_l2_eta);
  tree->SetBranchAddress("llnunu_l1_l1_pdgId",&llnunu_l1_l1_pdgId);
  tree->SetBranchAddress("llnunu_l1_l2_pdgId",&llnunu_l1_l2_pdgId);
  tree->SetBranchAddress("llnunu_l1_l1_ptErr",&llnunu_l1_l1_ptErr);
  tree->SetBranchAddress("llnunu_l1_l2_ptErr",&llnunu_l1_l2_ptErr);
  tree->SetBranchAddress("llnunu_mt",&llnunu_mt);
  tree->SetBranchAddress("llnunu_deltaPhi", &llnunu_deltaPhi);
  tree->SetBranchAddress("nTrueInt", &nTrueInt);
  tree->SetBranchAddress("nVert", &nVert);
  tree->SetBranchAddress("rho", &rho);
  tree->SetBranchAddress("njet", &njet);
  tree->SetBranchAddress("jet_pt", jet_pt);
  tree->SetBranchAddress("jet_phi", jet_phi);
  tree->SetBranchAddress("jet_eta", jet_eta);
  tree->SetBranchAddress("jet_rawPt", jet_rawPt);
  tree->SetBranchAddress("jet_id", jet_id);
  tree->SetBranchAddress("jet_chargedHadronEnergyFraction", jet_chargedHadronEnergyFraction);
  tree->SetBranchAddress("jet_neutralHadronEnergyFraction", jet_neutralHadronEnergyFraction);
  tree->SetBranchAddress("jet_neutralEmEnergyFraction", jet_neutralEmEnergyFraction);
  tree->SetBranchAddress("jet_muonEnergyFraction", jet_muonEnergyFraction);
  tree->SetBranchAddress("jet_chargedEmEnergyFraction", jet_chargedEmEnergyFraction);
  tree->SetBranchAddress("jet_chargedHadronMultiplicity", jet_chargedHadronMultiplicity);
  tree->SetBranchAddress("jet_chargedMultiplicity", jet_chargedMultiplicity);
  tree->SetBranchAddress("jet_neutralMultiplicity", jet_neutralMultiplicity);
  tree->SetBranchAddress("ngenZ", &ngenZ);
  tree->SetBranchAddress("genZ_pt", genZ_pt);
  tree->SetBranchAddress("genZ_phi", genZ_phi);
  tree->SetBranchAddress("genZ_eta", genZ_eta);
  tree->SetBranchAddress("ngenLep", &ngenLep);
  tree->SetBranchAddress("genLep_pdgId", genLep_pdgId);
  tree->SetBranchAddress("genLep_pt", genLep_pt);
  tree->SetBranchAddress("genLep_phi", genLep_phi);
  tree->SetBranchAddress("genLep_eta", genLep_eta);

  // new branches
  Float_t llnunu_mt_old;
  tree_out->Branch("llnunu_mt_old",&llnunu_mt_old,"llnunu_mt_old/F");

  Float_t llnunu_old_l2_pt, llnunu_old_l2_phi, llnunu_old_deltaPhi;
  tree_out->Branch("llnunu_old_l2_pt",&llnunu_old_l2_pt,"llnunu_old_l2_pt/F");
  tree_out->Branch("llnunu_old_l2_phi",&llnunu_old_l2_phi,"llnunu_old_l2_phi/F");
  tree_out->Branch("llnunu_old_deltaPhi",&llnunu_old_deltaPhi,"llnunu_old_deltaPhi/F");

  Float_t llnunu_l1_l1_pf_pt, llnunu_l1_l1_pf_phi, llnunu_l1_l1_pf_eta, llnunu_l1_l1_pf_frac, llnunu_l1_l1_pf_dR;
  Float_t llnunu_l1_l2_pf_pt, llnunu_l1_l2_pf_phi, llnunu_l1_l2_pf_eta, llnunu_l1_l2_pf_frac, llnunu_l1_l2_pf_dR;
  Int_t   llnunu_l1_l1_pf_idx, llnunu_l1_l2_pf_idx;
  tree_out->Branch("llnunu_l1_l1_pf_pt", &llnunu_l1_l1_pf_pt, "llnunu_l1_l1_pf_pt/F");
  tree_out->Branch("llnunu_l1_l1_pf_phi", &llnunu_l1_l1_pf_phi, "llnunu_l1_l1_pf_phi/F");
  tree_out->Branch("llnunu_l1_l1_pf_eta", &llnunu_l1_l1_pf_eta, "llnunu_l1_l1_pf_eta/F");
  tree_out->Branch("llnunu_l1_l1_pf_frac", &llnunu_l1_l1_pf_frac, "llnunu_l1_l1_pf_frac/F");
  tree_out->Branch("llnunu_l1_l1_pf_dR", &llnunu_l1_l1_pf_dR, "llnunu_l1_l1_pf_dR/F");
  tree_out->Branch("llnunu_l1_l1_pf_idx", &llnunu_l1_l1_pf_idx, "llnunu_l1_l1_pf_idx/I");
  tree_out->Branch("llnunu_l1_l2_pf_pt", &llnunu_l1_l2_pf_pt, "llnunu_l1_l2_pf_pt/F");
  tree_out->Branch("llnunu_l1_l2_pf_phi", &llnunu_l1_l2_pf_phi, "llnunu_l1_l2_pf_phi/F");
  tree_out->Branch("llnunu_l1_l2_pf_eta", &llnunu_l1_l2_pf_eta, "llnunu_l1_l2_pf_eta/F");
  tree_out->Branch("llnunu_l1_l2_pf_frac", &llnunu_l1_l2_pf_frac, "llnunu_l1_l2_pf_frac/F");
  tree_out->Branch("llnunu_l1_l2_pf_dR", &llnunu_l1_l2_pf_dR, "llnunu_l1_l2_pf_dR/F");
  tree_out->Branch("llnunu_l1_l2_pf_idx", &llnunu_l1_l2_pf_idx, "llnunu_l1_l2_pf_idx/I");

  Float_t llnunu_l1_l1_gen_pt(-1e30), llnunu_l1_l1_gen_phi(-1e30), llnunu_l1_l1_gen_eta(-1e30);
  Float_t llnunu_l1_l2_gen_pt(-1e30), llnunu_l1_l2_gen_phi(-1e30), llnunu_l1_l2_gen_eta(-1e30);
  tree_out->Branch("llnunu_l1_l1_gen_pt", &llnunu_l1_l1_gen_pt, "llnunu_l1_l1_gen_pt/F");
  tree_out->Branch("llnunu_l1_l1_gen_phi", &llnunu_l1_l1_gen_phi, "llnunu_l1_l1_gen_phi/F");
  tree_out->Branch("llnunu_l1_l1_gen_eta", &llnunu_l1_l1_gen_eta, "llnunu_l1_l1_gen_eta/F");
  tree_out->Branch("llnunu_l1_l2_gen_pt", &llnunu_l1_l2_gen_pt, "llnunu_l1_l2_gen_pt/F");
  tree_out->Branch("llnunu_l1_l2_gen_phi", &llnunu_l1_l2_gen_phi, "llnunu_l1_l2_gen_phi/F");
  tree_out->Branch("llnunu_l1_l2_gen_eta", &llnunu_l1_l2_gen_eta, "llnunu_l1_l2_gen_eta/F");

  Float_t genLep_pf_pt[100], genLep_pf_phi[100], genLep_pf_eta[100], genLep_pf_frac[100], genLep_pf_dR[100];
  tree_out->Branch("genLep_pf_pt", genLep_pf_pt, "genLep_pf_pt[ngenLep]/F");
  tree_out->Branch("genLep_pf_phi", genLep_pf_phi, "genLep_pf_phi[ngenLep]/F");
  tree_out->Branch("genLep_pf_eta", genLep_pf_eta, "genLep_pf_eta[ngenLep]/F");
  tree_out->Branch("genLep_pf_frac", genLep_pf_frac, "genLep_pf_frac[ngenLep]/F");
  tree_out->Branch("genLep_pf_dR", genLep_pf_dR, "genLep_pf_dR[ngenLep]/F");

  Int_t njet_corr, jet_corr_id[1000];
  Float_t jet_corr_pt[1000], jet_corr_phi[1000], jet_corr_eta[1000], jet_corr_rawPt[1000];
  Float_t jet_corr_reso[1000];
  Float_t jet_corr_jec_corr[1000], jet_corr_jec_corrUp[1000], jet_corr_jec_corrDown[1000];
  tree_out->Branch("njet_corr", &njet_corr, "njet_corr/I");
  tree_out->Branch("jet_corr_id", jet_corr_id, "jet_corr_id[njet_corr]/I");
  tree_out->Branch("jet_corr_pt", jet_corr_pt, "jet_corr_pt[njet_corr]/F");
  tree_out->Branch("jet_corr_phi", jet_corr_phi, "jet_corr_phi[njet_corr]/F");
  tree_out->Branch("jet_corr_eta", jet_corr_eta, "jet_corr_eta[njet_corr]/F");
  tree_out->Branch("jet_corr_rawPt", jet_corr_rawPt, "jet_corr_rawPt[njet_corr]/F");
  tree_out->Branch("jet_corr_reso",jet_corr_reso,"jet_corr_reso[njet_corr]/F");
  tree_out->Branch("jet_corr_jec_corr",jet_corr_jec_corr,"jet_corr_jec_corr[njet_corr]/F");
  tree_out->Branch("jet_corr_jec_corrUp",jet_corr_jec_corrUp,"jet_corr_jec_corrUp[njet_corr]/F");
  tree_out->Branch("jet_corr_jec_corrDown",jet_corr_jec_corrDown,"jet_corr_jec_corrDown[njet_corr]/F");

  Float_t jet_corr_chargedHadronEnergyFraction[1000], jet_corr_neutralHadronEnergyFraction[1000], jet_corr_neutralEmEnergyFraction[1000];
  Float_t jet_corr_muonEnergyFraction[1000], jet_corr_chargedEmEnergyFraction[1000], jet_corr_chargedHadronMultiplicity[1000];
  Float_t jet_corr_chargedMultiplicity[1000], jet_corr_neutralMultiplicity[1000];
  Float_t jet_corr_pt_old[1000], jet_corr_phi_old[1000], jet_corr_eta_old[1000];
  Int_t   jet_corr_sel_idx[1000];
  tree_out->Branch("jet_corr_sel_idx", jet_corr_sel_idx, "jet_corr_sel_idx[njet_corr]/I");
  tree_out->Branch("jet_corr_pt_old", jet_corr_pt_old, "jet_corr_pt_old[njet_corr]/F");
  tree_out->Branch("jet_corr_phi_old", jet_corr_phi_old, "jet_corr_phi_old[njet_corr]/F");
  tree_out->Branch("jet_corr_eta_old", jet_corr_eta_old, "jet_corr_eta_old[njet_corr]/F");
  tree_out->Branch("jet_corr_chargedHadronEnergyFraction", jet_corr_chargedHadronEnergyFraction, "jet_corr_chargedHadronEnergyFraction[njet_corr]/F");
  tree_out->Branch("jet_corr_neutralHadronEnergyFraction", jet_corr_neutralHadronEnergyFraction, "jet_corr_neutralHadronEnergyFraction[njet_corr]/F");
  tree_out->Branch("jet_corr_neutralEmEnergyFraction", jet_corr_neutralEmEnergyFraction, "jet_corr_neutralEmEnergyFraction[njet_corr]/F");
  tree_out->Branch("jet_corr_muonEnergyFraction", jet_corr_muonEnergyFraction, "jet_corr_muonEnergyFraction[njet_corr]/F");
  tree_out->Branch("jet_corr_chargedEmEnergyFraction", jet_corr_chargedEmEnergyFraction, "jet_corr_chargedEmEnergyFraction[njet_corr]/F");
  tree_out->Branch("jet_corr_chargedHadronMultiplicity", jet_corr_chargedHadronMultiplicity, "jet_corr_chargedHadronMultiplicity[njet_corr]/F");
  tree_out->Branch("jet_corr_chargedMultiplicity", jet_corr_chargedMultiplicity, "jet_corr_chargedMultiplicity[njet_corr]/F");
  tree_out->Branch("jet_corr_neutralMultiplicity", jet_corr_neutralMultiplicity, "jet_corr_neutralMultiplicity[njet_corr]/F");

  // ut hard
  Float_t ut_hard_para, ut_hard_perp, ut_hard_pt, ut_hard_phi;
  Float_t ut_hard_para_old, ut_hard_perp_old, ut_hard_pt_old, ut_hard_phi_old;
  Float_t ut_hard_para_raw, ut_hard_perp_raw, ut_hard_pt_raw, ut_hard_phi_raw;
  tree_out->Branch("ut_hard_pt",&ut_hard_pt,"ut_hard_pt/F");
  tree_out->Branch("ut_hard_phi",&ut_hard_phi,"ut_hard_phi/F");
  tree_out->Branch("ut_hard_pt_old",&ut_hard_pt_old,"ut_hard_pt_old/F");
  tree_out->Branch("ut_hard_phi_old",&ut_hard_phi_old,"ut_hard_phi_old/F");
  tree_out->Branch("ut_hard_pt_raw",&ut_hard_pt_raw,"ut_hard_pt_raw/F");
  tree_out->Branch("ut_hard_phi_raw",&ut_hard_phi_raw,"ut_hard_phi_raw/F");

  // bisector phi
  Float_t xi_phi;
  tree_out->Branch("xi_phi",&xi_phi,"xi_phi/F");

  // alias
  tree_out->SetAlias("ut_hard_para", "(ut_hard_pt[0]*cos(ut_hard_phi[0]-llnunu_l1_phi[0]))");
  tree_out->SetAlias("ut_hard_perp", "(ut_hard_pt[0]*sin(ut_hard_phi[0]-llnunu_l1_phi[0]))");
  tree_out->SetAlias("ut_hard_para_old", "(ut_hard_pt_old[0]*cos(ut_hard_phi_old[0]-llnunu_l1_phi[0]))");
  tree_out->SetAlias("ut_hard_perp_old", "(ut_hard_pt_old[0]*sin(ut_hard_phi_old[0]-llnunu_l1_phi[0]))");
  tree_out->SetAlias("ut_hard_para_raw", "(ut_hard_pt_raw[0]*cos(ut_hard_phi_raw[0]-llnunu_l1_phi[0]))");
  tree_out->SetAlias("ut_hard_perp_raw", "(ut_hard_pt_raw[0]*sin(ut_hard_phi_raw[0]-llnunu_l1_phi[0]))");

  tree_out->SetAlias("met_para", "(llnunu_l2_pt[0]*cos(llnunu_l2_phi[0]-llnunu_l1_phi[0]))");
  tree_out->SetAlias("met_perp", "(llnunu_l2_pt[0]*sin(llnunu_l2_phi[0]-llnunu_l1_phi[0]))");
  tree_out->SetAlias("met_para_old", "(llnunu_old_l2_pt[0]*cos(llnunu_old_l2_phi[0]-llnunu_l1_phi[0]))");
  tree_out->SetAlias("met_perp_old", "(llnunu_old_l2_pt[0]*sin(llnunu_old_l2_phi[0]-llnunu_l1_phi[0]))");
  tree_out->SetAlias("met_para_raw", "(llnunu_l2_rawPt[0]*cos(llnunu_l2_rawPhi[0]-llnunu_l1_phi[0]))");
  tree_out->SetAlias("met_perp_raw", "(llnunu_l2_rawPt[0]*sin(llnunu_l2_rawPhi[0]-llnunu_l1_phi[0]))");

  tree_out->SetAlias("jetTightId_old","(jet_id[]>=3)");
  tree_out->SetAlias("jetLepVeto_old","(jet_chargedEmEnergyFraction[]<0.6&&jet_muonEnergyFraction[]<0.6)");
  tree_out->SetAlias("jet_para_old","(jet_pt[]*cos(jet_phi[]-llnunu_l1_phi[0]))");
  tree_out->SetAlias("jet_perp_old","(jet_pt[]*sin(jet_phi[]-llnunu_l1_phi[0]))");

  tree_out->SetAlias("jet_para_raw","(jet_rawPt[]*cos(jet_phi[]-llnunu_l1_phi[0]))");
  tree_out->SetAlias("jet_perp_raw","(jet_rawPt[]*sin(jet_phi[]-llnunu_l1_phi[0]))");

  tree_out->SetAlias("jetTightId","(jet_corr_id[]>=3)");
  tree_out->SetAlias("jetLepVeto","(jet_corr_chargedEmEnergyFraction[]<0.6&&jet_corr_muonEnergyFraction[]<0.6)");
  tree_out->SetAlias("jet_para","(jet_corr_pt[]*cos(jet_corr_phi[]-llnunu_l1_phi[0]))");
  tree_out->SetAlias("jet_perp","(jet_corr_pt[]*sin(jet_corr_phi[]-llnunu_l1_phi[0]))");

  // met shifts
  TFile* file_metshifts;
  file_metshifts = new TFile("skim/DYJetsToLL_M50_V3JetNewSelV6NoMetLepAnyWayAllJetsBigSig1p4LepRes_met_para_study.root");
  //file_metshifts = new TFile("skim/DYJetsToLL_M50_V3SignalProtection_met_para_study.root");
  //file_metshifts = new TFile("file_met_para_shift_vs_zpt_dyjets_jetpt15_forShiftOnly_Corr.root");
  //file_metshifts = new TFile("skim/DYJetsToLL_M50_V3JetNewSelV3BigSig1p4SigProtect_met_para_study.root");
  //file_metshifts = new TFile("skim/DYJetsToLL_M50_V3JetNewSelV4BigSig1p4SignalProtection_met_para_study.root");
  //file_metshifts = new TFile("skim/DYJetsToLL_M50_V3JetNewSelV4BigSig1p5SignalProtection_met_para_study.root");
  //file_metshifts = new TFile("skim/DYJetsToLL_M50_V3JetNewSelV4BigSig1p6SignalProtection_met_para_study.root");

  //TH1D* h_metshifts = (TH1D*)file_metshifts->Get("h_met_para_vs_zpt_shift");
  TH1D* h_metshifts = (TH1D*)file_metshifts->Get("h_met_para_vs_zpt_new_mean");
  TGraph* gr_metshifts = new TGraph(h_metshifts);

  //TFile* file_metshifts_data = new TFile("skim/SingleEMU_Run2015_15Dec_V3JetNewSelV3BigSig1p4SigProtect_met_para_study.root");
  TFile* file_metshifts_data = new TFile("skim/SingleEMU_Run2015_15Dec_V3JetNewSelV6NoMetLepAnyWayAllJetsBigSig1p4LepResSigProtect_met_para_study.root");
  TH1D* h_metshifts_data = (TH1D*)file_metshifts_data->Get("h_met_para_vs_zpt_new_mean");
  TGraph* gr_metshifts_data = new TGraph(h_metshifts_data);


  // Met resolution correction
  TFile* file_metsigma;
  //file_metsigma = new TFile("h_met_sigma_para_vs_perp_met_para_shifted.root");
  file_metsigma = new TFile("h_met_sigma_para_vs_perp_met_para_shifted_corr.root");
  TH1D* h_met_sigma = (TH1D*)file_metsigma->Get("h_met_sigma_para_vs_perp_new");

  // ZPT corr model
  TFile* file_zptcorr = new TFile("file_zpt_corr.root");
  TProfile* pr_zptcorr = (TProfile*)file_zptcorr->Get("pr_zptdiff_vs_zpt_new");

  // lepton pt error
  TFile* file_pterr =  new TFile("pt_reso.root");
  TProfile2D* pr_pt_err_el = (TProfile2D*)file_pterr->Get("pr_dpt_eta_pt_el");
  TProfile2D* pr_pt_err_mu = (TProfile2D*)file_pterr->Get("pr_dpt_eta_pt_mu");
  TH1D* h_pt_err_el = (TH1D*)file_pterr->Get("h_dpt_el");
  TH1D* h_pt_err_mu = (TH1D*)file_pterr->Get("h_dpt_mu");



  // JER
  JME::JetResolution JERReso("Summer15_25nsV6_MC_PtResolution_AK4PFchs.txt");
  JME::JetResolution JERResoData("Summer15_25nsV6_DATA_PtResolution_AK4PFchs.txt");

  JME::JetResolution JERReso2016("Spring16_25nsV6_MC_PtResolution_AK4PFchs.txt");
  JME::JetResolution JERResoData2016("Spring16_25nsV6_DATA_PtResolution_AK4PFchs.txt");

  if (do2016){
    JERReso = JERReso2016;
    JERResoData = JERResoData2016;
  } 
  
  JME::JetParameters JERPars;


  int nentries = (int)tree->GetEntries();
  int n_pass = 0;
  if (!debug) n_start=0;
  for (int i=n_start; i<nentries;  i++){
    tree->GetEntry(i);

    if (llnunu_l1_pt<_zpt_cut_min||llnunu_l1_pt>_zpt_cut_max) continue;

    n_pass++;
    if (debug && n_pass>n_test) break;

    if (debug) {
      std::cout << "#############################################" << std::endl;
      std::cout << "##  Entry " << i << ", Event " << evt << ", njets = " << njet << ", zpt = " << llnunu_l1_pt << std::endl; 
      std::cout << "#############################################" << std::endl;
    } 
    else if ( i%n_interval == 0 ) {
      std::cout << "Event " << i << ", njets = " << njet << ", zpt = " << llnunu_l1_pt << std::endl; 
    }

    double z_pt = (double)llnunu_l1_pt;
    double z_phi = (double)llnunu_l1_phi;
    double met_pt = (double)llnunu_l2_pt;
    double met_phi = (double)llnunu_l2_phi;
    double met_x = met_pt*cos(met_phi);
    double met_y = met_pt*sin(met_phi);
    TVector2 vec_met(met_x, met_y); 

    if (useRaw){
      met_pt = (double)llnunu_l2_rawPt;
      met_phi = (double)llnunu_l2_rawPhi;
    }

    Float_t* jet_pt_used = jet_pt;
    if (useRaw) jet_pt_used = jet_rawPt;    

    std::vector<double> jets_pt;    
    std::vector<double> jets_phi;
    std::vector<double> jets_eta;
    std::vector<double> jets_reso;    
    std::vector<bool>   jets_islep;   
    std::vector<int>    jets_id;   
    std::vector<double> jets_chargedHadronEnergyFraction;
    std::vector<double> jets_neutralHadronEnergyFraction;
    std::vector<double> jets_neutralEmEnergyFraction;
    std::vector<double> jets_muonEnergyFraction;
    std::vector<double> jets_chargedEmEnergyFraction;
    std::vector<int>    jets_chargedHadronMultiplicity;
    std::vector<int>    jets_chargedMultiplicity;
    std::vector<int>    jets_neutralMultiplicity;

    std::vector<int> jets_selected_index;

    // calculate the bisector direction phi for later use
    TVector2 Xi_Direction = (TVector2(cos(llnunu_l1_l1_phi),sin(llnunu_l1_l2_phi))+TVector2(cos(llnunu_l1_l2_phi),sin(llnunu_l1_l2_phi))).Unit();
    xi_phi = TVector2::Phi_mpi_pi(Xi_Direction.Phi());

    // check the results, to be add up as the corrected met. 
    double met_para = met_pt*cos(met_phi-llnunu_l1_phi);
    double met_perp = met_pt*sin(met_phi-llnunu_l1_phi);

    // lepton info
    if (debug) {
      std::cout << " met: para = " << met_para     << ", perp = " << met_perp << ", phi = " << met_phi << std::endl; 
      std::cout << " Z  : pt   = " << llnunu_l1_pt << ", eta = " << llnunu_l1_eta << ", phi = " << llnunu_l1_phi << std::endl; 
      std::cout << " l1 : para = " << llnunu_l1_l1_pt*cos(llnunu_l1_l1_phi-llnunu_l1_phi) << ",  perp = " << llnunu_l1_l1_pt*sin(llnunu_l1_l1_phi-llnunu_l1_phi) << std::endl;
      std::cout << " l2 : para = " << llnunu_l1_l2_pt*cos(llnunu_l1_l2_phi-llnunu_l1_phi) << ",  perp = " << llnunu_l1_l2_pt*sin(llnunu_l1_l2_phi-llnunu_l1_phi) << std::endl;
      std::cout << " l1 : pt = " << llnunu_l1_l1_pt << ", eta = " << llnunu_l1_l1_eta << ", phi = " << llnunu_l1_l1_phi << ", ptErr/pt = " << llnunu_l1_l1_ptErr/llnunu_l1_l1_pt << std::endl; 
      std::cout << " l2 : pt = " << llnunu_l1_l2_pt << ", eta = " << llnunu_l1_l2_eta << ", phi = " << llnunu_l1_l2_phi << ", ptErr/pt = " << llnunu_l1_l2_ptErr/llnunu_l1_l2_pt << std::endl; 
    }

    if (doZpTCorr && ngenZ>0) {
      //met_para -= pr_zptcorr->GetBinContent(pr_zptcorr->FindBin(z_pt));
      met_para -= genZ_pt[0]*cos(genZ_phi[0]-llnunu_l1_phi)-llnunu_l1_pt;
    }

    if (doMetShift&&!doMetShiftAfter) {
      if (isData)  met_para -= h_metshifts_data->GetBinContent(h_metshifts_data->FindBin(llnunu_l1_pt));
      else met_para -= h_metshifts->GetBinContent(h_metshifts->FindBin(llnunu_l1_pt));
    }

    // met resolution correction
    if (doMetSigma) {
      met_para /= h_met_sigma->GetBinContent(h_met_sigma->FindBin(llnunu_l1_pt));
    }

    // attach genLep branches
    if (ngenLep>0) {
      double min_dR1(1e30),min_dR2(1e30);
      int min_idx1(-100),min_idx2(-100);
      for (int j=0; j<ngenLep; j++){
        double deltaR1 = sqrt( pow(genLep_eta[j]-llnunu_l1_l1_eta,2) + pow( TVector2::Phi_mpi_pi(genLep_phi[j]-llnunu_l1_l1_phi),2) );
        double deltaR2 = sqrt( pow(genLep_eta[j]-llnunu_l1_l2_eta,2) + pow( TVector2::Phi_mpi_pi(genLep_phi[j]-llnunu_l1_l2_phi),2) );
        if (deltaR1<min_dR1) {
          min_dR1 = deltaR1;
          min_idx1 = j;
        }
        if (deltaR2<min_dR2) {
          min_dR2 = deltaR2;
          min_idx2 = deltaR2;
        }
      }

      if (min_dR1<0.3) {
        llnunu_l1_l1_gen_pt = genLep_pt[min_idx1];
        llnunu_l1_l1_gen_phi = genLep_phi[min_idx1]; 
        llnunu_l1_l1_gen_eta = genLep_eta[min_idx1];
       }
      if (min_dR2<0.3) {
        llnunu_l1_l2_gen_pt = genLep_pt[min_idx2]; 
        llnunu_l1_l2_gen_phi = genLep_phi[min_idx2]; 
        llnunu_l1_l2_gen_eta = genLep_eta[min_idx2];
       }
    }
    // lepton reco-pfcandidate difference correction
    // loop over jets, find the lepton jets matched to the leptons
    double lep1_pf_pt(-1e30), lep1_pf_phi(-1e30), lep1_pf_eta(-1e30);
    double lep2_pf_pt(-1e30), lep2_pf_phi(-1e30), lep2_pf_eta(-1e30);
    double lep1_pf_frac(-1e30), lep2_pf_frac(-1e30);
    double lep1_pf_min_dR(1e30), lep2_pf_min_dR(1e30);
    double lep1_pf_para(-1e30), lep1_pf_perp(-1e30);
    double lep2_pf_para(-1e30), lep2_pf_perp(-1e30);
    int    lep1_pf_idx(-999), lep2_pf_idx(-999);
    double gen_lep1_pf_pt(-1e30), gen_lep1_pf_phi(-1e30), gen_lep1_pf_eta(-1e30);
    double gen_lep2_pf_pt(-1e30), gen_lep2_pf_phi(-1e30), gen_lep2_pf_eta(-1e30);
    double gen_lep1_pf_frac(-1e30), gen_lep2_pf_frac(-1e30);
    double gen_lep1_pf_min_dR(1e30), gen_lep2_pf_min_dR(1e30);
    double gen_lep1_pf_para(-1e30), gen_lep1_pf_perp(-1e30);
    double gen_lep2_pf_para(-1e30), gen_lep2_pf_perp(-1e30);
    double lep1_reco_para = llnunu_l1_l1_pt*cos(llnunu_l1_l1_phi-llnunu_l1_phi);
    double lep2_reco_para = llnunu_l1_l2_pt*cos(llnunu_l1_l2_phi-llnunu_l1_phi);
    double lep1_reco_perp = llnunu_l1_l1_pt*sin(llnunu_l1_l1_phi-llnunu_l1_phi);
    double lep2_reco_perp = llnunu_l1_l2_pt*sin(llnunu_l1_l2_phi-llnunu_l1_phi);
    double lep1_gen_para(-1e30), lep1_gen_perp(-1e30);
    double lep2_gen_para(-1e30), lep2_gen_perp(-1e30);

    if (ngenLep>0){
      lep1_gen_para=genLep_pt[0]*cos(genLep_phi[0]-llnunu_l1_phi);
      lep1_gen_perp=genLep_pt[0]*sin(genLep_phi[0]-llnunu_l1_phi);
      lep2_gen_para=genLep_pt[1]*cos(genLep_phi[1]-llnunu_l1_phi);
      lep2_gen_perp=genLep_pt[1]*sin(genLep_phi[1]-llnunu_l1_phi);
    }
      
    for (int j=0; j<njet; j++){
      // identify lepton jet
      double efrac = -100;
      double gen_efrac = -100;
      if (ngenLep>0&&abs(genLep_pdgId[0])==11&&abs(genLep_pdgId[1])==11) {
        gen_efrac = jet_chargedEmEnergyFraction[j];
      }
      else if (ngenLep>0&&abs(genLep_pdgId[0])==13&&abs(genLep_pdgId[1])==13) {
        gen_efrac = jet_muonEnergyFraction[j];
      }

      if (abs(llnunu_l1_l1_pdgId)==11&&abs(llnunu_l1_l2_pdgId)==11) {
        efrac = jet_chargedEmEnergyFraction[j];
      }
      else if (abs(llnunu_l1_l1_pdgId)==13&&abs(llnunu_l1_l2_pdgId)==13) {
        efrac = jet_muonEnergyFraction[j];
      }
        
      // find dR match
      double deltaR1 = sqrt( pow(jet_eta[j]-llnunu_l1_l1_eta,2) + pow( TVector2::Phi_mpi_pi(jet_phi[j]-llnunu_l1_l1_phi),2) );
      double deltaR2 = sqrt( pow(jet_eta[j]-llnunu_l1_l2_eta,2) + pow( TVector2::Phi_mpi_pi(jet_phi[j]-llnunu_l1_l2_phi),2) );
       
      double gen_deltaR1(1e30), gen_deltaR2(1e30); 
      if (ngenLep>0) {
        gen_deltaR1 = sqrt( pow(jet_eta[j]-genLep_eta[0],2) + pow( TVector2::Phi_mpi_pi(jet_phi[j]-genLep_phi[0]),2) );
        gen_deltaR2 = sqrt( pow(jet_eta[j]-genLep_eta[1],2) + pow( TVector2::Phi_mpi_pi(jet_phi[j]-genLep_phi[1]),2) );
      }

      //std::cout << " efrac = " << efrac << ", gen_efrac = " << gen_efrac << ", deltaR1 = " << deltaR1 << ", deltaR2 = " << deltaR2 << ", gen_deltaR1 = " << gen_deltaR1 << ", gen_deltaR2 = " << gen_deltaR2 << std::endl;
      // lep1
      if (efrac>0.5 && deltaR1<0.4 && deltaR1<lep1_pf_min_dR) {
        lep1_pf_min_dR = deltaR1;
        lep1_pf_pt = jet_pt[j]*efrac;
        lep1_pf_phi = jet_phi[j];
        lep1_pf_eta = jet_eta[j];
        lep1_pf_frac = efrac;
        lep1_pf_idx = j;
      }
      // lep2
      if (efrac>0.5 && deltaR2<0.4 && deltaR2<lep2_pf_min_dR)  {
        lep2_pf_min_dR = deltaR2;
        lep2_pf_pt = jet_pt[j]*efrac;
        lep2_pf_phi = jet_phi[j];
        lep2_pf_eta = jet_eta[j];
        lep2_pf_frac = efrac;
        lep2_pf_idx = j;
      }

      // genlep1
      if (ngenLep>0 && gen_efrac>0.5 && gen_deltaR1<0.4 && gen_deltaR1<gen_lep1_pf_min_dR) {
        gen_lep1_pf_min_dR = gen_deltaR1;
        gen_lep1_pf_pt = jet_pt[j]*gen_efrac;
        gen_lep1_pf_phi = jet_phi[j];
        gen_lep1_pf_eta = jet_eta[j];
        gen_lep1_pf_frac = gen_efrac;
      }

      // genlep2
      if (ngenLep>0 && gen_efrac>0.5 && gen_deltaR2<0.4 && gen_deltaR2<gen_lep2_pf_min_dR) {
        gen_lep2_pf_min_dR = gen_deltaR2;
        gen_lep2_pf_pt = jet_pt[j]*gen_efrac;
        gen_lep2_pf_phi = jet_phi[j];
        gen_lep2_pf_eta = jet_eta[j];
        gen_lep2_pf_frac = gen_efrac;
      }

    }

    //
    llnunu_l1_l1_pf_pt = lep1_pf_pt;
    llnunu_l1_l1_pf_phi = lep1_pf_phi;
    llnunu_l1_l1_pf_eta = lep1_pf_eta;
    llnunu_l1_l1_pf_frac = lep1_pf_frac;
    llnunu_l1_l1_pf_dR = lep1_pf_min_dR;
    llnunu_l1_l1_pf_idx = lep1_pf_idx;
    llnunu_l1_l2_pf_pt = lep2_pf_pt;
    llnunu_l1_l2_pf_phi = lep2_pf_phi;
    llnunu_l1_l2_pf_eta = lep2_pf_eta;
    llnunu_l1_l2_pf_frac = lep2_pf_frac;
    llnunu_l1_l2_pf_dR = lep2_pf_min_dR; 
    llnunu_l1_l2_pf_idx = lep2_pf_idx;
    //
    if(ngenLep>0){
      genLep_pf_pt[0] = gen_lep1_pf_pt;
      genLep_pf_phi[0] = gen_lep1_pf_phi;
      genLep_pf_eta[0] = gen_lep1_pf_eta;
      genLep_pf_frac[0] = gen_lep1_pf_frac;
      genLep_pf_dR[0] = gen_lep1_pf_min_dR;
      genLep_pf_pt[1] = gen_lep2_pf_pt;
      genLep_pf_phi[1] = gen_lep2_pf_phi;
      genLep_pf_eta[1] = gen_lep2_pf_eta;
      genLep_pf_frac[1] = gen_lep2_pf_frac;
      genLep_pf_dR[1] = gen_lep2_pf_min_dR;
    }

    if (doPfLepCorr) {
      if (debug){
        std::cout << "## lepton pf correction" << std::endl;
        std::cout << " old :  met_para=" << met_para << ", met_perp=" << met_perp << std::endl;
      }
      //
      if (ngenLep>0){
        gen_lep1_pf_para = gen_lep1_pf_pt*cos(gen_lep1_pf_phi-llnunu_l1_phi);
        gen_lep1_pf_perp = gen_lep1_pf_pt*sin(gen_lep1_pf_phi-llnunu_l1_phi);
        gen_lep2_pf_para = gen_lep2_pf_pt*cos(gen_lep2_pf_phi-llnunu_l1_phi);
        gen_lep2_pf_perp = gen_lep2_pf_pt*sin(gen_lep2_pf_phi-llnunu_l1_phi);
      }
      //
      lep1_pf_para = lep1_pf_pt*cos(lep1_pf_phi-llnunu_l1_phi);
      lep1_pf_perp = lep1_pf_pt*sin(lep1_pf_phi-llnunu_l1_phi);
      lep2_pf_para = lep2_pf_pt*cos(lep2_pf_phi-llnunu_l1_phi);
      lep2_pf_perp = lep2_pf_pt*sin(lep2_pf_phi-llnunu_l1_phi);

      // correction
      if (doPfLepCorrUseTruth&&ngenLep>0){
        // correct lep1 
        if (gen_lep1_pf_min_dR<0.3) {
          met_para += gen_lep1_pf_para - lep1_gen_para;
          met_perp += gen_lep1_pf_perp - lep1_gen_perp;
        }
        // correct lep2 
        if (gen_lep2_pf_min_dR<0.3) {
          met_para += gen_lep2_pf_para - lep2_gen_para;
          met_perp += gen_lep2_pf_perp - lep2_gen_perp;
        }
      }
      else {
        // correct lep1 
        if (lep1_pf_min_dR<0.03) {
          met_para += lep1_pf_para - lep1_reco_para;
          met_perp += lep1_pf_perp - lep1_reco_perp;
        }
        // correct lep2
        if (lep2_pf_min_dR<0.03) {
          met_para += lep2_pf_para - lep2_reco_para;
          met_perp += lep2_pf_perp - lep2_reco_perp;
        }
      }

      if (debug){
        std::cout << " new :  met_para=" << met_para << ", met_perp=" << met_perp << std::endl;
        std::cout << " ngenLep = " << ngenLep << std::endl; 
        std::cout << " gen    : lep1_gen_para=" << lep1_gen_para << ", lep2_gen_para=" << lep2_gen_para
             << ", lep1_gen_perp=" << lep1_gen_perp << ", lep2_gen_perp=" << lep2_gen_perp
             << std::endl;
        std::cout << " reco   : lep1_rec_para=" << lep1_reco_para << ", lep2_rec_para=" << lep2_reco_para
             << ", lep1_rec_perp=" << lep1_reco_perp << ", lep2_rec_perp=" << lep2_reco_perp
             << std::endl;
        std::cout << " gen pf : lep1_pfc_para=" << gen_lep1_pf_para << ", lep2_pfc_para=" << gen_lep2_pf_para 
             << ", lep1_pf_perp=" << gen_lep1_pf_perp << ", lep2_pf_perp=" << gen_lep2_pf_perp
             << ", lep1_pf_dR=" << gen_lep1_pf_min_dR << ", lep2_pf_dR=" << gen_lep2_pf_min_dR
             << ", lep1_pf_frac=" << gen_lep1_pf_frac << ", lep2_pf_frac=" << gen_lep2_pf_frac
             << std::endl; 
        std::cout << " reco pf: lep1_pfc_para=" << lep1_pf_para << ", lep2_pf_para=" << lep2_pf_para 
             << ", lep1_pf_perp=" << lep1_pf_perp << ", lep2_pf_perp=" << lep2_pf_perp
             << ", lep1_pf_dR=" << lep1_pf_min_dR << ", lep2_pf_dR=" << lep2_pf_min_dR
             << ", lep1_pf_frac=" << lep1_pf_frac << ", lep2_pf_frac=" << lep2_pf_frac
             << std::endl;
      }
    }

    // recalculate met_pt, met_phi for jet fits 
    met_x = met_para*cos(llnunu_l1_phi)-met_perp*sin(llnunu_l1_phi);
    met_y = met_para*sin(llnunu_l1_phi)+met_perp*cos(llnunu_l1_phi);
    vec_met.Set(met_x, met_y);
    met_pt = vec_met.Mod();
    met_phi = TVector2::Phi_mpi_pi(vec_met.Phi());


    if(jetSelOption!=1) {

      // select jets
      if (jetSelOption<=4) {
        jets_selected_index.clear();
        for (int j=0; j<max_njet; j++){
          // loop over jets to find a best match
          int idx = -100;
          if (jetSelOption==2) {
            idx = find_ut_jet(jets_selected_index, z_pt, z_phi,
                              njet, jet_pt_used, jet_phi, jet_id, jet_chargedEmEnergyFraction, jet_muonEnergyFraction);
          }
          else if (jetSelOption==3) {
            idx = find_ut_jet_v3(jets_selected_index, z_pt, z_phi, met_pt, met_phi,
                              njet, jet_pt_used, jet_phi, jet_id, jet_chargedEmEnergyFraction, jet_muonEnergyFraction);
          }
          else if (jetSelOption==4) {
            idx = find_ut_jet_v4(jets_selected_index, z_pt, z_phi, met_pt, met_phi,
                              njet, jet_pt_used, jet_phi, jet_id, jet_chargedEmEnergyFraction, jet_muonEnergyFraction);
          }
        
          // break if no jet found
          if (idx<0) break;
          // store jet index
          jets_selected_index.push_back(idx);
        }
      }
      //
      else if (jetSelOption==5) {
        jets_selected_index = find_ut_jet_v5(z_pt, z_phi, met_pt, met_phi,
                            njet, jet_pt_used, jet_phi, jet_id, jet_chargedEmEnergyFraction, jet_muonEnergyFraction);
      }
      //
      else if (jetSelOption==6) {
        jets_selected_index = find_ut_jet_v6(z_pt, z_phi, met_pt, met_phi,
                            njet, jet_pt_used, jet_phi, jet_id, jet_chargedEmEnergyFraction, jet_muonEnergyFraction);
      }
      // 
      else if (jetSelOption==7) {
        jets_selected_index = find_ut_jet_v7(z_pt, z_phi, met_pt, met_phi,
                            njet, jet_pt_used, jet_phi, jet_id, jet_chargedEmEnergyFraction, jet_muonEnergyFraction);
      }
      //
      else if (jetSelOption==8) {
        jets_selected_index = find_ut_jet_v8(z_pt, z_phi, met_pt, met_phi,
                            njet, jet_pt_used, jet_phi, jet_id, jet_chargedEmEnergyFraction, jet_muonEnergyFraction);
      }
      //
 
      // signal protection
      if (doSignalProtection&&jets_selected_index.size()>0){
/*
        ut_hard_para = 0;
        ut_hard_perp = 0;
        njet_corr = (int)jets_selected_index.size();
        for (int j=0; j<njet_corr; j++){
          int idx = jets_selected_index.at(j);
          ut_hard_para += jet_pt_used[idx]*cos(jet_phi[idx]-llnunu_l1_phi);
          ut_hard_perp += jet_pt_used[idx]*sin(jet_phi[idx]-llnunu_l1_phi);
        }
        ut_hard_pt = sqrt(ut_hard_para*ut_hard_para+ut_hard_perp*ut_hard_perp);
        if (ut_hard_pt<0.5) {
          jets_selected_index.clear();
        }
*/     
        // cut on first jet para
        int idx = jets_selected_index.at(0);
        double jtpara = jet_pt_used[idx]*cos(jet_phi[idx]-llnunu_l1_phi);
        if (fabs(jtpara/llnunu_l1_pt)<0.5) {
          jets_selected_index.clear();
        }
      }


      // store selected jets information
      njet_corr = (int)jets_selected_index.size();
      for (int j=0; j<njet_corr; j++){

        int idx = jets_selected_index.at(j);
        double jet_para = jet_pt_used[idx]*cos(jet_phi[idx]-llnunu_l1_phi);
        double jet_perp = jet_pt_used[idx]*sin(jet_phi[idx]-llnunu_l1_phi);

        JERPars.setJetPt(jet_pt_used[idx]);
        JERPars.setJetEta(jet_eta[idx]);
        JERPars.setRho(rho);
        double jer;
        //jet resolution
        if (isData) jer = (double)JERResoData.getResolution(JERPars);
        else jer = (double)JERReso.getResolution(JERPars);
        bool is_lepton = false;
        double jpt = jet_pt_used[idx];
        double jphi = jet_phi[idx];
        double jeta = jet_eta[idx];
        int jid = jet_id[idx];
        double jjer = jer;


        jets_reso.push_back(jer);
        jets_pt.push_back(jpt);
        jets_phi.push_back(jphi);
        jets_eta.push_back(jeta);
        jets_islep.push_back(is_lepton);
        jets_id.push_back(jid);
        jets_chargedHadronEnergyFraction.push_back(jet_chargedHadronEnergyFraction[idx]);
        jets_neutralHadronEnergyFraction.push_back(jet_neutralHadronEnergyFraction[idx]);
        jets_neutralEmEnergyFraction.push_back(jet_neutralEmEnergyFraction[idx]);
        jets_muonEnergyFraction.push_back(jet_muonEnergyFraction[idx]);
        jets_chargedEmEnergyFraction.push_back(jet_chargedEmEnergyFraction[idx]);
        jets_chargedHadronMultiplicity.push_back(jet_chargedHadronMultiplicity[idx]);
        jets_chargedMultiplicity.push_back(jet_chargedMultiplicity[idx]);
        jets_neutralMultiplicity.push_back(jet_neutralMultiplicity[idx]);

        if (debug) {
          std::cout << " jet " << j << " idx " << idx << " : para = " << jet_para << ", perp = " << jet_perp << ", eta = " << jet_eta[idx] 
            << ", jer = " << jer << ", lepton ? = " << is_lepton ;
          if (is_lepton) std::cout << ", jet_jer = " << jjer << ", pdg = " << jid ;
          std::cout  << std::endl; 
        }

      }

      // add leptons
      if ((doJetsCorrUseLepRes||jetSelOption==8)&&njet_corr>0) {
        // selection option 8, lepton jets not removed, must have to do this step.
        // handle lepton jets: remove lepton fraction from the lepton jets at first,
        // then update the jer assume it is a small jet
        // lep 1
        double jer=0.0;
        double jpt=llnunu_l1_l1_pt;
        if (doJetsCorrUseLepResPtErr) {
          jer = llnunu_l1_l1_ptErr/llnunu_l1_l1_pt;
          if (jetSelOption==8&&llnunu_l1_l1_pf_idx>=0){
            int iid = -1;
            for (int k=0; k<(int)jets_selected_index.size(); k++) {
              if (jets_selected_index.at(k)==llnunu_l1_l1_pf_idx) iid = k;
            }
            if (iid>=0) {
              if (abs(llnunu_l1_l1_pdgId)==13) {
                double lepef = jets_muonEnergyFraction.at(iid); 
                if (doJetsCorrUseLepResPtErrJetLep) jpt = jets_pt.at(iid)*lepef;
                jets_pt.at(iid) -= jets_pt.at(iid)*lepef;
                jets_muonEnergyFraction.at(iid) = 0.0;
                double sumf = jets_chargedHadronEnergyFraction.at(iid) 
                            + jets_neutralHadronEnergyFraction.at(iid)
                            + jets_neutralEmEnergyFraction.at(iid)
                            + jets_muonEnergyFraction.at(iid)
                            + jets_chargedEmEnergyFraction.at(iid);
                jets_chargedHadronEnergyFraction.at(iid) /=sumf;
                jets_neutralHadronEnergyFraction.at(iid) /=sumf;
                jets_neutralEmEnergyFraction.at(iid) /= sumf;
                jets_muonEnergyFraction.at(iid) /= sumf;
                jets_chargedEmEnergyFraction.at(iid) /= sumf;
              }
              else {
                double lepef = jets_chargedEmEnergyFraction.at(iid);
                if (doJetsCorrUseLepResPtErrJetLep) jpt = jets_pt.at(iid)*lepef;
                jets_pt.at(iid) -= jets_pt.at(iid)*lepef;
                jets_chargedEmEnergyFraction.at(iid) = 0.0;
                double sumf = jets_chargedHadronEnergyFraction.at(iid)         
                            + jets_neutralHadronEnergyFraction.at(iid)
                            + jets_neutralEmEnergyFraction.at(iid)
                            + jets_muonEnergyFraction.at(iid)
                            + jets_chargedEmEnergyFraction.at(iid);
                jets_chargedHadronEnergyFraction.at(iid) /=sumf;
                jets_neutralHadronEnergyFraction.at(iid) /=sumf;
                jets_neutralEmEnergyFraction.at(iid) /= sumf;
                jets_muonEnergyFraction.at(iid) /= sumf;
                jets_chargedEmEnergyFraction.at(iid) /= sumf;
              }
            }
          }
        }
        else {
          if(abs(llnunu_l1_l1_pdgId)==13) {
            jer = pr_pt_err_mu->GetBinContent(pr_pt_err_mu->FindBin(llnunu_l1_l1_pt, llnunu_l1_l1_eta));
            if (jer<=0.0) jer = h_pt_err_mu->GetMean();
            jer *= llnunu_l1_l1_pt;
          }
          else if (abs(llnunu_l1_l1_pdgId)==11) {
            jer = pr_pt_err_el->GetBinContent(pr_pt_err_el->FindBin(llnunu_l1_l1_pt, llnunu_l1_l1_eta));
            if (jer<=0.0) jer = h_pt_err_el->GetMean();
          }
        }
        jets_reso.push_back(jer);
        jets_pt.push_back(jpt);
        jets_phi.push_back(llnunu_l1_l1_phi);
        jets_eta.push_back(llnunu_l1_l1_eta);
        jets_islep.push_back(true);
        jets_id.push_back(llnunu_l1_l1_pdgId);
        jets_chargedHadronEnergyFraction.push_back(-999);
        jets_neutralHadronEnergyFraction.push_back(-999);
        jets_neutralEmEnergyFraction.push_back(-999);
        jets_muonEnergyFraction.push_back(-999);
        jets_chargedEmEnergyFraction.push_back(-999);
        jets_chargedHadronMultiplicity.push_back(-999);
        jets_chargedMultiplicity.push_back(-999);
        jets_neutralMultiplicity.push_back(-999);
        jets_selected_index.push_back(-1);

        // lep2
        jer=0.0;
        jpt = llnunu_l1_l2_pt;
        if (doJetsCorrUseLepResPtErr) {
          jer = llnunu_l1_l2_ptErr/llnunu_l1_l2_pt;
          if (jetSelOption==8&&llnunu_l1_l2_pf_idx>=0){
            int iid = -1;
            for (int k=0; k<(int)jets_selected_index.size(); k++) {
              if (jets_selected_index.at(k)==llnunu_l1_l2_pf_idx) iid = k;
            }
            if (iid>=0) {
              if (abs(llnunu_l1_l2_pdgId)==13) {
                double lepef = jets_muonEnergyFraction.at(iid);
                if (doJetsCorrUseLepResPtErrJetLep) jpt = jets_pt.at(iid)*lepef;
                jets_pt.at(iid) -= jets_pt.at(iid)*lepef;
                jets_muonEnergyFraction.at(iid) = 0.0;
                double sumf = jets_chargedHadronEnergyFraction.at(iid)
                            + jets_neutralHadronEnergyFraction.at(iid)
                            + jets_neutralEmEnergyFraction.at(iid)
                            + jets_muonEnergyFraction.at(iid)
                            + jets_chargedEmEnergyFraction.at(iid);
                jets_chargedHadronEnergyFraction.at(iid) /=sumf;
                jets_neutralHadronEnergyFraction.at(iid) /=sumf;
                jets_neutralEmEnergyFraction.at(iid) /= sumf;
                jets_muonEnergyFraction.at(iid) /= sumf;
                jets_chargedEmEnergyFraction.at(iid) /= sumf;
              }
              else {
                double lepef = jets_chargedEmEnergyFraction.at(iid);
                if (doJetsCorrUseLepResPtErrJetLep) jpt = jets_pt.at(iid)*lepef;
                jets_pt.at(iid) -= jets_pt.at(iid)*lepef;
                jets_chargedEmEnergyFraction.at(iid) = 0.0;
                double sumf = jets_chargedHadronEnergyFraction.at(iid)
                            + jets_neutralHadronEnergyFraction.at(iid)
                            + jets_neutralEmEnergyFraction.at(iid)
                            + jets_muonEnergyFraction.at(iid)
                            + jets_chargedEmEnergyFraction.at(iid);
                jets_chargedHadronEnergyFraction.at(iid) /=sumf;
                jets_neutralHadronEnergyFraction.at(iid) /=sumf;
                jets_neutralEmEnergyFraction.at(iid) /= sumf;
                jets_muonEnergyFraction.at(iid) /= sumf;
                jets_chargedEmEnergyFraction.at(iid) /= sumf;
              }
            }
          }
        }
        else {
          if(abs(llnunu_l1_l2_pdgId)==13) {
            jer = pr_pt_err_mu->GetBinContent(pr_pt_err_mu->FindBin(llnunu_l1_l2_pt, llnunu_l1_l2_eta));
            if (jer<=0.0) jer = h_pt_err_mu->GetMean();
            jer *= llnunu_l1_l2_pt;
          }
          else if (abs(llnunu_l1_l2_pdgId)==11) {
            jer = pr_pt_err_el->GetBinContent(pr_pt_err_el->FindBin(llnunu_l1_l2_pt, llnunu_l1_l2_eta));
            if (jer<=0.0) jer = h_pt_err_el->GetMean();
          }
        }
        jets_reso.push_back(jer);
        jets_pt.push_back(jpt);
        jets_phi.push_back(llnunu_l1_l2_phi);
        jets_eta.push_back(llnunu_l1_l2_eta);
        jets_islep.push_back(true);
        jets_id.push_back(llnunu_l1_l2_pdgId);
        jets_chargedHadronEnergyFraction.push_back(-999);
        jets_neutralHadronEnergyFraction.push_back(-999);
        jets_neutralEmEnergyFraction.push_back(-999);
        jets_muonEnergyFraction.push_back(-999);
        jets_chargedEmEnergyFraction.push_back(-999);
        jets_chargedHadronMultiplicity.push_back(-999);
        jets_chargedMultiplicity.push_back(-999);
        jets_neutralMultiplicity.push_back(-999);
        jets_selected_index.push_back(-2);
      } 
      // update njet_corr
      njet_corr = (int)jets_selected_index.size();

    }
    else {
      for (int j=0; j<njet; j++){

        // lepton veto
        //double deltaR1 = sqrt( pow(jet_eta[j]-llnunu_l1_l1_eta,2) + pow( TVector2::Phi_mpi_pi(jet_phi[j]-llnunu_l1_l1_phi),2) );
        //double deltaR2 = sqrt( pow(jet_eta[j]-llnunu_l1_l2_eta,2) + pow( TVector2::Phi_mpi_pi(jet_phi[j]-llnunu_l1_l2_phi),2) );
        //if (deltaR1<0.4||deltaR2<0.4) continue;
       
        if (debug) {
      //    std::cout << " jet " << j << " : pt = " << jet_pt[j] << ", eta = " << jet_eta[j] << ", phi = " << jet_phi[j] << ", dR l1 = " << deltaR1 << ", dR l2 = " << deltaR2 << std::endl; 
        }

        bool jetLepVeto = (jet_chargedEmEnergyFraction[j]<0.8&&jet_muonEnergyFraction[j]<0.8);
        bool jetTightId = (jet_id[j]>=3);      
        double jet_para = jet_pt_used[j]*cos(jet_phi[j]-llnunu_l1_phi);

        if (jet_pt_used[j]<100.0) continue;
        if (fabs(jet_eta[j])>3.0) continue;
        if (!jetLepVeto) continue;
        if (!jetTightId) continue;
        if (jet_para>-150) continue;

        JERPars.setJetPt(jet_pt_used[j]);
        JERPars.setJetEta(jet_eta[j]);
        JERPars.setRho(rho);
        jets_reso.push_back(JERReso.getResolution(JERPars));
        jets_pt.push_back(jet_pt_used[j]);
        jets_phi.push_back(jet_phi[j]);
        jets_eta.push_back(jet_eta[j]);
        jets_id.push_back(jet_id[j]);
        jets_chargedHadronEnergyFraction.push_back(jet_chargedHadronEnergyFraction[j]);
        jets_neutralHadronEnergyFraction.push_back(jet_neutralHadronEnergyFraction[j]);
        jets_neutralEmEnergyFraction.push_back(jet_neutralEmEnergyFraction[j]);
        jets_muonEnergyFraction.push_back(jet_muonEnergyFraction[j]);
        jets_chargedEmEnergyFraction.push_back(jet_chargedEmEnergyFraction[j]);
        jets_chargedHadronMultiplicity.push_back(jet_chargedHadronMultiplicity[j]);
        jets_chargedMultiplicity.push_back(jet_chargedMultiplicity[j]);
        jets_neutralMultiplicity.push_back(jet_neutralMultiplicity[j]);
      }
      njet_corr = (Int_t)jets_pt.size();

    }

    // if study ut hard fit only, skip the event if no selected jets
    if (doHardOnly && njet_corr<=0 ) continue;
 
    // calculate ut hard old and new
    ut_hard_para = 0;
    ut_hard_perp = 0;
    ut_hard_para_old = 0;
    ut_hard_perp_old = 0;
    ut_hard_pt = 0;
    ut_hard_phi = 0;
    ut_hard_pt_old = 0;
    ut_hard_phi_old = 0;

    // store selected jets information in jet_corr collections
    // will be updated later if jes is refitted. 
    for (int j=0; j<njet_corr; j++) {
      jet_corr_jec_corr[j]=1.0;
      jet_corr_sel_idx[j]=jets_selected_index.at(j);
      jet_corr_pt[j] = jets_pt.at(j);
      jet_corr_phi[j] = jets_phi.at(j);
      jet_corr_eta[j] = jets_eta.at(j);
      jet_corr_reso[j] = jets_reso.at(j);
      jet_corr_id[j] = jets_id.at(j);
      jet_corr_pt_old[j] = jets_pt.at(j);
      jet_corr_phi_old[j] = jets_phi.at(j);
      jet_corr_eta_old[j] = jets_eta.at(j);
      jet_corr_chargedHadronEnergyFraction[j] = jets_chargedHadronEnergyFraction.at(j);
      jet_corr_neutralHadronEnergyFraction[j] = jets_neutralHadronEnergyFraction.at(j);
      jet_corr_neutralEmEnergyFraction[j] = jets_neutralEmEnergyFraction.at(j);
      jet_corr_muonEnergyFraction[j] = jets_muonEnergyFraction.at(j);
      jet_corr_chargedEmEnergyFraction[j] = jets_chargedEmEnergyFraction.at(j);
      jet_corr_chargedHadronMultiplicity[j] = jets_chargedHadronMultiplicity.at(j);
      jet_corr_chargedMultiplicity[j] = jets_chargedMultiplicity.at(j);
      jet_corr_neutralMultiplicity[j] = jets_neutralMultiplicity.at(j);      
      ut_hard_para_old += jets_pt.at(j)*cos(jets_phi.at(j)-llnunu_l1_phi);
      ut_hard_perp_old += jets_pt.at(j)*sin(jets_phi.at(j)-llnunu_l1_phi);
      ut_hard_para += jet_corr_jec_corr[j]*jets_pt.at(j)*cos(jets_phi.at(j)-llnunu_l1_phi);
      ut_hard_perp += jet_corr_jec_corr[j]*jets_pt.at(j)*sin(jets_phi.at(j)-llnunu_l1_phi);
    }

    // remove lepton contribution if 
    if (doJetsCorrUseLepRes) {
      ut_hard_para_old -= llnunu_l1_l1_pt*cos(llnunu_l1_l1_phi-llnunu_l1_phi);
      ut_hard_para_old -= llnunu_l1_l2_pt*cos(llnunu_l1_l2_phi-llnunu_l1_phi);
      ut_hard_perp_old -= llnunu_l1_l1_pt*sin(llnunu_l1_l1_phi-llnunu_l1_phi);
      ut_hard_perp_old -= llnunu_l1_l2_pt*sin(llnunu_l1_l2_phi-llnunu_l1_phi);
      ut_hard_para -= llnunu_l1_l1_pt*cos(llnunu_l1_l1_phi-llnunu_l1_phi);
      ut_hard_para -= llnunu_l1_l2_pt*cos(llnunu_l1_l2_phi-llnunu_l1_phi);
      ut_hard_perp -= llnunu_l1_l1_pt*sin(llnunu_l1_l1_phi-llnunu_l1_phi);
      ut_hard_perp -= llnunu_l1_l2_pt*sin(llnunu_l1_l2_phi-llnunu_l1_phi);
    }

    if (debug) {
      std::cout << " # ut hard old :  para = " << ut_hard_para_old << ", perp = " << ut_hard_perp_old << std::endl;
    }

    // do minut fit of jet energy scale if njet_corr > 0 
    if (doJetsCorr && njet_corr>0) {
      // reset ut hard
      ut_hard_para = 0;
      ut_hard_perp = 0;
      ut_hard_para_old = 0;
      ut_hard_perp_old = 0;
      ut_hard_pt = 0;
      ut_hard_phi = 0;
      ut_hard_pt_old = 0;
      ut_hard_phi_old = 0;

      // recalculate met_pt, met_phi for jet fits 
      met_x = met_para*cos(llnunu_l1_phi)-met_perp*sin(llnunu_l1_phi);
      met_y = met_para*sin(llnunu_l1_phi)+met_perp*cos(llnunu_l1_phi);
      vec_met.Set(met_x, met_y);
      met_pt = vec_met.Mod();
      met_phi = TVector2::Phi_mpi_pi(vec_met.Phi());

      // check the lepton jets, if exists remove it from z_pt : note, this is wrong!!! z_pt is not used in minimizing
      //double z_pt_rest = z_pt;
      //for (int j=0; j<njet_corr; j++){
      //  if (std::max(jets_muonEnergyFraction.at(j),jets_chargedEmEnergyFraction.at(j))>0.2) {
      //    z_pt_rest -= jets_pt.at(j)*cos(jets_phi.at(j)-z_phi);
      //  }
      //}

      // initialize minimizer
      ROOT::Math::MinimizerOptions::SetDefaultPrintLevel(0);
      MetChi2Fcn metChi2; 
      metChi2.InitFunction(z_pt, z_phi,met_pt, met_phi, jets_pt, jets_phi, jets_reso, fitOption);
      ROOT::Minuit2::MnUserParameters upars;
      for (int j=0; j<njet_corr; j++){
        sprintf(name, "jec_%d", j);
       
        if (Opt==0){
          // "default" both +/- allow to vary
          upars.Add(name, 1.0,jets_reso.at(j), 1-jer_scale*jets_reso.at(j), 1+jer_scale*jets_reso.at(j));
        }
        else if (Opt==1){
          //"BackJets": only allow jets back to Z boost to vary
          upars.Add(name, 1.0,jets_reso.at(j), 1-jer_scale*jets_reso.at(j), 1+jer_scale*jets_reso.at(j));
          if (cos(jets_phi.at(j)-llnunu_l1_phi)>0) {
            upars.Fix(j);
          }
        }
        else if (Opt==2 || Opt==21) {
          // 2:  "BackBig": for jets back side to Z, only allow increase JES, for jets same side to Z, only allow decrease JES
          // 21: "BackBigLessConstr" : same as BackBig, but less jer constraint, 1-sigma allowed.
          if (Opt==21) jer_scale=1.0;
          if (cos(jets_phi.at(j)-llnunu_l1_phi)<=0) { 
            upars.Add(name, 1.0,jets_reso.at(j), 1.0, 1+jer_scale*jets_reso.at(j));
          }
          else {
            upars.Add(name, 1.0,jets_reso.at(j), 1-jer_scale*jets_reso.at(j), 1.0);
          }
        }
        else if (Opt==3){
          // 3: DefaultSmallVariation: both +/- allow to vary, but very small variation allowed
          jer_scale=0.01;
          upars.Add(name, 1.0,jets_reso.at(j), 1-jer_scale*jets_reso.at(j), 1+jer_scale*jets_reso.at(j));
        }
        else if (Opt==31){
          // 31: DefaultTwoJERVariation: both +/- allow to vary, but very small variation allowed
          jer_scale=2;
          upars.Add(name, 1.0,jets_reso.at(j), 1-jer_scale*jets_reso.at(j), 1+jer_scale*jets_reso.at(j));
        }
        else if (Opt==32){
          // 32: DefaultThreeJERVariation: both +/- allow to vary, but very small variation allowed
          jer_scale=3;
          upars.Add(name, 1.0,jets_reso.at(j), 1-jer_scale*jets_reso.at(j), 1+jer_scale*jets_reso.at(j));
        }
      }

      // add free parameter to shift met para
      if (fitOption=="useMetShift") {
        upars.Add("met_para_shift", 0, 0.5, -2, 5);
      }
      
      ROOT::Minuit2::MnMigrad migrad(metChi2, upars);
      ROOT::Minuit2::FunctionMinimum min = migrad(0, 1e-7);
      //std::cout << "Minimum: " << min << std::endl;

      if (debug) {
        std::cout << " ## fitted jets ## " << std::endl;
      }
      // fitted jet scale starting from upars[k]
      for (int j=0; j<njet_corr; j++){
        jet_corr_jec_corr[j]= min.UserParameters().Value(j);
        jet_corr_jec_corrUp[j]= min.UserParameters().Value(j)+min.UserParameters().Error(j);
        jet_corr_jec_corrDown[j]= min.UserParameters().Value(j)-min.UserParameters().Error(j);
        jet_corr_pt[j] = jets_pt.at(j)*jet_corr_jec_corr[j];
        jet_corr_phi[j] = jets_phi.at(j);

        double jet_para_old = jets_pt.at(j)*cos(jets_phi.at(j)-llnunu_l1_phi);
        double jet_perp_old = jets_pt.at(j)*sin(jets_phi.at(j)-llnunu_l1_phi);
        double jet_para = jet_corr_jec_corr[j]*jet_para_old;
        double jet_perp = jet_corr_jec_corr[j]*jet_perp_old;

        met_para += (1.0-jet_corr_jec_corr[j])*jet_para_old;
        met_perp += (1.0-jet_corr_jec_corr[j])*jet_perp_old;
        ut_hard_para_old += jet_para_old;
        ut_hard_perp_old += jet_perp_old;
        ut_hard_para += jet_para;
        ut_hard_perp += jet_perp;

        if (debug) {
          std::cout << " jet new " << j << " : para = " << jet_para << ", perp = " << jet_perp << ", eta = " << jets_eta.at(j) << ", jer = " << jets_reso.at(j) << std::endl;               
        }

      }  

      // remove lepton contribution if 
      if (doJetsCorrUseLepRes) {
        ut_hard_para_old -= llnunu_l1_l1_pt*cos(llnunu_l1_l1_phi-llnunu_l1_phi);
        ut_hard_para_old -= llnunu_l1_l2_pt*cos(llnunu_l1_l2_phi-llnunu_l1_phi);
        ut_hard_perp_old -= llnunu_l1_l1_pt*sin(llnunu_l1_l1_phi-llnunu_l1_phi);
        ut_hard_perp_old -= llnunu_l1_l2_pt*sin(llnunu_l1_l2_phi-llnunu_l1_phi);
        ut_hard_para -= llnunu_l1_l1_pt*cos(llnunu_l1_l1_phi-llnunu_l1_phi);
        ut_hard_para -= llnunu_l1_l2_pt*cos(llnunu_l1_l2_phi-llnunu_l1_phi);
        ut_hard_perp -= llnunu_l1_l1_pt*sin(llnunu_l1_l1_phi-llnunu_l1_phi);
        ut_hard_perp -= llnunu_l1_l2_pt*sin(llnunu_l1_l2_phi-llnunu_l1_phi);
      }

      //
      if (fitOption=="useMetShift") {
        met_para += min.UserParameters().Value(njet_corr);
      }

    } // end jes refit



    // met shifts
    if (doMetShift&&doMetShiftAfter) {
      if (isData)  met_para -= h_metshifts_data->GetBinContent(h_metshifts_data->FindBin(llnunu_l1_pt));
      else met_para -= h_metshifts->GetBinContent(h_metshifts->FindBin(llnunu_l1_pt));
    }

    //
    TVector2 vec_ut_hard_zframe(ut_hard_para, ut_hard_perp);
    ut_hard_pt = vec_ut_hard_zframe.Mod();
    ut_hard_phi = TVector2::Phi_mpi_pi(vec_ut_hard_zframe.Rotate(llnunu_l1_phi).Phi()); 
    TVector2 vec_ut_hard_old_zframe(ut_hard_para_old, ut_hard_perp_old);
    ut_hard_pt_old = vec_ut_hard_old_zframe.Mod();
    ut_hard_phi_old = TVector2::Phi_mpi_pi(vec_ut_hard_old_zframe.Rotate(llnunu_l1_phi).Phi()); 

    if (debug) {
      std::cout << " # ut hard new :  para = " << ut_hard_para << ", perp = " << ut_hard_perp << std::endl; 
    }

    // met_x met_y
    met_x = met_para*cos(llnunu_l1_phi)-met_perp*sin(llnunu_l1_phi);
    met_y = met_para*sin(llnunu_l1_phi)+met_perp*cos(llnunu_l1_phi);
    vec_met.Set(met_x, met_y);
    double met = vec_met.Mod();
    double metphi = TVector2::Phi_mpi_pi(vec_met.Phi());
    double deltaPhi = TVector2::Phi_mpi_pi(metphi-llnunu_l1_phi);
    double etc1 = TMath::Sqrt(llnunu_l1_mass*llnunu_l1_mass + llnunu_l1_pt*llnunu_l1_pt);
    double etc2 = TMath::Sqrt(llnunu_l1_mass*llnunu_l1_mass+met*met);
    double mt = TMath::Sqrt(2.0*llnunu_l1_mass*llnunu_l1_mass + 2.0* (etc1*etc2 - llnunu_l1_pt*cos(llnunu_l1_phi)*met_x - llnunu_l1_pt*sin(llnunu_l1_phi)*met_y));


    if (debug) {
      std::cout << " ## event summary ## " << std::endl;
      std::cout << " old:  met = " << llnunu_l2_pt << ", metphi = " << llnunu_l2_phi << ", deltaPhi = " << -llnunu_deltaPhi 
        << ", met_para = " << llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi) 
        << ", met_perp = " << llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi) 
        << ", mt = " << llnunu_mt << std::endl;
      std::cout << " new:  met = " << met << ", metphi = " << metphi << ", deltaPhi = " << deltaPhi 
        << ", met_para = " << met*cos(deltaPhi)
        << ", met_perp = " << met*sin(deltaPhi)
        << ", mt = " << mt << std::endl;
    }

    llnunu_old_l2_pt = (Float_t)llnunu_l2_pt;
    llnunu_old_l2_phi = (Float_t)llnunu_l2_phi;
    llnunu_old_deltaPhi = (Float_t)TVector2::Phi_mpi_pi(llnunu_l2_phi-llnunu_l1_phi);
    
    llnunu_l2_pt = (Float_t)met;
    llnunu_l2_phi = (Float_t)metphi;
    llnunu_deltaPhi = (Float_t)deltaPhi;
    llnunu_mt_old = (Float_t)llnunu_mt; 
    llnunu_mt = (Float_t)mt;
 
    tree_out->Fill();

  }

  foutput->cd();

  // Praw some control plots
  TH1D* hdphi_old = new TH1D("hdphi_old", "dphi", 36, 0, TMath::Pi());
  TH1D* hdphi_new = new TH1D("hdphi_new", "dphi", 36, 0, TMath::Pi());
  TH1D* hdphi_zpt100_old = new TH1D("hdphi_zpt100_old", "dphi_zpt100", 36, 0, TMath::Pi());
  TH1D* hdphi_zpt100_new = new TH1D("hdphi_zpt100_new", "dphi_zpt100", 36, 0, TMath::Pi());
  TH1D* hdphi_zpt100_met50_old = new TH1D("hdphi_zpt100_met50_old", "dphi_zpt100_met50", 36, 0, TMath::Pi());
  TH1D* hdphi_zpt100_met50_new = new TH1D("hdphi_zpt100_met50_new", "dphi_zpt100_met50", 36, 0, TMath::Pi());
  TH1D* hmetphi_old = new TH1D("hmetphi_old", "met phi", 100, -TMath::Pi(), TMath::Pi());
  TH1D* hmetphi_new = new TH1D("hmetphi_new", "met phi", 100, -TMath::Pi(), TMath::Pi());
  TH1D* hmet_para_old = new TH1D("hmet_para_old", "met para", 100, -200, 200);
  TH1D* hmet_para_new = new TH1D("hmet_para_new", "met para", 100, -200, 200);
  TH1D* hmet_perp_old = new TH1D("hmet_perp_old", "met perp", 100, -200, 200);
  TH1D* hmet_perp_new = new TH1D("hmet_perp_new", "met perp", 100, -200, 200);
  TH1D* hmet_para_zpt100_old = new TH1D("hmet_para_zpt100_old", "met para_zpt100", 100, -200, 200);
  TH1D* hmet_para_zpt100_new = new TH1D("hmet_para_zpt100_new", "met para_zpt100", 100, -200, 200);
  TH1D* hmet_perp_zpt100_old = new TH1D("hmet_perp_zpt100_old", "met perp_zpt100", 100, -200, 200);
  TH1D* hmet_perp_zpt100_new = new TH1D("hmet_perp_zpt100_new", "met perp_zpt100", 100, -200, 200);
  TH1D* hmet_para_zpt100_met50_old = new TH1D("hmet_para_zpt100_met50_old", "met para_zpt100_met50", 100, -200, 200);
  TH1D* hmet_para_zpt100_met50_new = new TH1D("hmet_para_zpt100_met50_new", "met para_zpt100_met50", 100, -200, 200);
  TH1D* hmet_perp_zpt100_met50_old = new TH1D("hmet_perp_zpt100_met50_old", "met perp_zpt100_met50", 100, -200, 200);
  TH1D* hmet_perp_zpt100_met50_new = new TH1D("hmet_perp_zpt100_met50_new", "met perp_zpt100_met50", 100, -200, 200);
  TH1D* hmet_para_pos_old = new TH1D("hmet_para_pos_old", "met para", 100, 0, 200);
  TH1D* hmet_para_pos_new = new TH1D("hmet_para_pos_new", "met para", 100, 0, 200);
  TH1D* hmet_perp_pos_old = new TH1D("hmet_perp_pos_old", "met perp", 100, 0, 200);
  TH1D* hmet_perp_pos_new = new TH1D("hmet_perp_pos_new", "met perp", 100, 0, 200);
  TH1D* hmet_para_pos_zpt100_old = new TH1D("hmet_para_pos_zpt100_old", "met para_zpt100", 100, 0, 200);
  TH1D* hmet_para_pos_zpt100_new = new TH1D("hmet_para_pos_zpt100_new", "met para_zpt100", 100, 0, 200);
  TH1D* hmet_perp_pos_zpt100_old = new TH1D("hmet_perp_pos_zpt100_old", "met perp_zpt100", 100, 0, 200);
  TH1D* hmet_perp_pos_zpt100_new = new TH1D("hmet_perp_pos_zpt100_new", "met perp_zpt100", 100, 0, 200);
  TH1D* hmet_para_pos_zpt100_met50_old = new TH1D("hmet_para_pos_zpt100_met50_old", "met para_zpt100_met50", 100, 0, 200);
  TH1D* hmet_para_pos_zpt100_met50_new = new TH1D("hmet_para_pos_zpt100_met50_new", "met para_zpt100_met50", 100, 0, 200);
  TH1D* hmet_perp_pos_zpt100_met50_old = new TH1D("hmet_perp_pos_zpt100_met50_old", "met perp_zpt100_met50", 100, 0, 200);
  TH1D* hmet_perp_pos_zpt100_met50_new = new TH1D("hmet_perp_pos_zpt100_met50_new", "met perp_zpt100_met50", 100, 0, 200);
  TH1D* hmet_para_neg_old = new TH1D("hmet_para_neg_old", "met para", 100, 0, 200);
  TH1D* hmet_para_neg_new = new TH1D("hmet_para_neg_new", "met para", 100, 0, 200);
  TH1D* hmet_perp_neg_old = new TH1D("hmet_perp_neg_old", "met perp", 100, 0, 200);
  TH1D* hmet_perp_neg_new = new TH1D("hmet_perp_neg_new", "met perp", 100, 0, 200);
  TH1D* hmet_para_neg_zpt100_old = new TH1D("hmet_para_neg_zpt100_old", "met para_zpt100", 100, 0, 200);
  TH1D* hmet_para_neg_zpt100_new = new TH1D("hmet_para_neg_zpt100_new", "met para_zpt100", 100, 0, 200);
  TH1D* hmet_perp_neg_zpt100_old = new TH1D("hmet_perp_neg_zpt100_old", "met perp_zpt100", 100, 0, 200);
  TH1D* hmet_perp_neg_zpt100_new = new TH1D("hmet_perp_neg_zpt100_new", "met perp_zpt100", 100, 0, 200);
  TH1D* hmet_para_neg_zpt100_met50_old = new TH1D("hmet_para_neg_zpt100_met50_old", "met para_zpt100_met50", 100, 0, 200);
  TH1D* hmet_para_neg_zpt100_met50_new = new TH1D("hmet_para_neg_zpt100_met50_new", "met para_zpt100_met50", 100, 0, 200);
  TH1D* hmet_perp_neg_zpt100_met50_old = new TH1D("hmet_perp_neg_zpt100_met50_old", "met perp_zpt100_met50", 100, 0, 200);
  TH1D* hmet_perp_neg_zpt100_met50_new = new TH1D("hmet_perp_neg_zpt100_met50_new", "met perp_zpt100_met50", 100, 0, 200);


  TH1D* hmetx_old = new TH1D("hmetx_old", "metx", 100, -200, 200);
  TH1D* hmetx_new = new TH1D("hmetx_new", "metx", 100, -200, 200);
  TH1D* hmety_old = new TH1D("hmety_old", "mety", 100, -200, 200);
  TH1D* hmety_new = new TH1D("hmety_new", "mety", 100, -200, 200);
  TH1D* hmet_old = new TH1D("hmet_old", "met", 200, 0, 1000);
  TH1D* hmet_new = new TH1D("hmet_new", "met", 200, 0, 1000);
  TH1D* hmt_old = new TH1D("hmt_old", "mt", 200, 0, 1000);
  TH1D* hmt_new = new TH1D("hmt_new", "mt", 200, 0, 1000);

  TH1D* hut_hard_para_old = new TH1D("hut_hard_para_old", "ut_hard para", 500, -1000, 1000);
  TH1D* hut_hard_para_new = new TH1D("hut_hard_para_new", "ut_hard para", 500, -1000, 1000);
  TH1D* hut_hard_perp_old = new TH1D("hut_hard_perp_old", "ut_hard perp", 100, -200, 200);
  TH1D* hut_hard_perp_new = new TH1D("hut_hard_perp_new", "ut_hard perp", 100, -200, 200);

  hdphi_old->Sumw2();
  hdphi_new->Sumw2();
  hdphi_zpt100_old->Sumw2();
  hdphi_zpt100_new->Sumw2();
  hdphi_zpt100_met50_old->Sumw2();
  hdphi_zpt100_met50_new->Sumw2();
  hmetphi_old->Sumw2();
  hmetphi_new->Sumw2();
  hmet_para_old->Sumw2();
  hmet_para_new->Sumw2();
  hmet_perp_old->Sumw2();
  hmet_perp_new->Sumw2();
  hmet_para_zpt100_old->Sumw2();
  hmet_para_zpt100_new->Sumw2();
  hmet_perp_zpt100_old->Sumw2();
  hmet_perp_zpt100_new->Sumw2();
  hmet_para_zpt100_met50_old->Sumw2();
  hmet_para_zpt100_met50_new->Sumw2();
  hmet_perp_zpt100_met50_old->Sumw2();
  hmet_perp_zpt100_met50_new->Sumw2();
  hmet_para_pos_old->Sumw2();
  hmet_para_pos_new->Sumw2();
  hmet_perp_pos_old->Sumw2();
  hmet_perp_pos_new->Sumw2();
  hmet_para_pos_zpt100_old->Sumw2();
  hmet_para_pos_zpt100_new->Sumw2();
  hmet_perp_pos_zpt100_old->Sumw2();
  hmet_perp_pos_zpt100_new->Sumw2();
  hmet_para_pos_zpt100_met50_old->Sumw2();
  hmet_para_pos_zpt100_met50_new->Sumw2();
  hmet_perp_pos_zpt100_met50_old->Sumw2();
  hmet_perp_pos_zpt100_met50_new->Sumw2();
  hmet_para_neg_old->Sumw2();
  hmet_para_neg_new->Sumw2();
  hmet_perp_neg_old->Sumw2();
  hmet_perp_neg_new->Sumw2();
  hmet_para_neg_zpt100_old->Sumw2();
  hmet_para_neg_zpt100_new->Sumw2();
  hmet_perp_neg_zpt100_old->Sumw2();
  hmet_perp_neg_zpt100_new->Sumw2();
  hmet_para_neg_zpt100_met50_old->Sumw2();
  hmet_para_neg_zpt100_met50_new->Sumw2();
  hmet_perp_neg_zpt100_met50_old->Sumw2();
  hmet_perp_neg_zpt100_met50_new->Sumw2();

  hmetx_old->Sumw2();
  hmetx_new->Sumw2();
  hmety_old->Sumw2();
  hmety_new->Sumw2();
  hmet_old->Sumw2();
  hmet_new->Sumw2();
  hmt_old->Sumw2();
  hmt_new->Sumw2();
  hut_hard_para_old->Sumw2();
  hut_hard_para_new->Sumw2();
  hut_hard_perp_old->Sumw2();
  hut_hard_perp_new->Sumw2();

  tree_out->Draw("abs(llnunu_old_deltaPhi)>>hdphi_old");
  tree_out->Draw("abs(llnunu_deltaPhi)>>hdphi_new");
  tree_out->Draw("abs(llnunu_old_deltaPhi)>>hdphi_zpt100_old", "llnunu_l1_pt>100");
  tree_out->Draw("abs(llnunu_deltaPhi)>>hdphi_zpt100_new", "llnunu_l1_pt>100");
  tree_out->Draw("abs(llnunu_old_deltaPhi)>>hdphi_zpt100_met50_old", "llnunu_l1_pt>100&&llnunu_old_l2_pt>50");
  tree_out->Draw("abs(llnunu_deltaPhi)>>hdphi_zpt100_met50_new", "llnunu_l1_pt>100&&llnunu_l2_pt>50");
  tree_out->Draw("llnunu_old_l2_phi>>hmetphi_old");
  tree_out->Draw("llnunu_l2_phi>>hmetphi_new");
  tree_out->Draw("llnunu_old_l2_pt*cos(llnunu_old_deltaPhi)>>hmet_para_old");
  tree_out->Draw("llnunu_l2_pt*cos(llnunu_deltaPhi)>>hmet_para_new");
  tree_out->Draw("llnunu_old_l2_pt*sin(llnunu_old_deltaPhi)>>hmet_perp_old");
  tree_out->Draw("llnunu_l2_pt*sin(llnunu_deltaPhi)>>hmet_perp_new");
  tree_out->Draw("llnunu_old_l2_pt*cos(llnunu_old_deltaPhi)>>hmet_para_zpt100_old", "llnunu_l1_pt>100");
  tree_out->Draw("llnunu_l2_pt*cos(llnunu_deltaPhi)>>hmet_para_zpt100_new", "llnunu_l1_pt>100");
  tree_out->Draw("llnunu_old_l2_pt*sin(llnunu_old_deltaPhi)>>hmet_perp_zpt100_old", "llnunu_l1_pt>100");
  tree_out->Draw("llnunu_l2_pt*sin(llnunu_deltaPhi)>>hmet_perp_zpt100_new", "llnunu_l1_pt>100");
  tree_out->Draw("llnunu_old_l2_pt*cos(llnunu_old_deltaPhi)>>hmet_para_zpt100_met50_old","llnunu_l1_pt>100&&llnunu_old_l2_pt>50");
  tree_out->Draw("llnunu_l2_pt*cos(llnunu_deltaPhi)>>hmet_para_zpt100_met50_new", "llnunu_l1_pt>100&&llnunu_l2_pt>50");
  tree_out->Draw("llnunu_old_l2_pt*sin(llnunu_old_deltaPhi)>>hmet_perp_zpt100_met50_old","llnunu_l1_pt>100&&llnunu_old_l2_pt>50");
  tree_out->Draw("llnunu_l2_pt*sin(llnunu_deltaPhi)>>hmet_perp_zpt100_met50_new", "llnunu_l1_pt>100&&llnunu_l2_pt>50");
  tree_out->Draw("llnunu_old_l2_pt*cos(llnunu_old_deltaPhi)>>hmet_para_pos_old", "met_para_old>0");
  tree_out->Draw("llnunu_l2_pt*cos(llnunu_deltaPhi)>>hmet_para_pos_new", "met_para>0");
  tree_out->Draw("llnunu_old_l2_pt*sin(llnunu_old_deltaPhi)>>hmet_perp_pos_old", "met_perp_old>0");
  tree_out->Draw("llnunu_l2_pt*sin(llnunu_deltaPhi)>>hmet_perp_pos_new", "met_perp>0");
  tree_out->Draw("llnunu_old_l2_pt*cos(llnunu_old_deltaPhi)>>hmet_para_pos_zpt100_old", "llnunu_l1_pt>100&&met_para_old>0");
  tree_out->Draw("llnunu_l2_pt*cos(llnunu_deltaPhi)>>hmet_para_pos_zpt100_new", "llnunu_l1_pt>100&&met_para>0");
  tree_out->Draw("llnunu_old_l2_pt*sin(llnunu_old_deltaPhi)>>hmet_perp_pos_zpt100_old", "llnunu_l1_pt>100&&met_perp_old>0");
  tree_out->Draw("llnunu_l2_pt*sin(llnunu_deltaPhi)>>hmet_perp_pos_zpt100_new", "llnunu_l1_pt>100&&met_perp>0");
  tree_out->Draw("llnunu_old_l2_pt*cos(llnunu_old_deltaPhi)>>hmet_para_pos_zpt100_met50_old","llnunu_l1_pt>100&&llnunu_old_l2_pt>50&&met_para_old>0");
  tree_out->Draw("llnunu_l2_pt*cos(llnunu_deltaPhi)>>hmet_para_pos_zpt100_met50_new", "llnunu_l1_pt>100&&llnunu_l2_pt>50&&met_para>0");
  tree_out->Draw("llnunu_old_l2_pt*sin(llnunu_old_deltaPhi)>>hmet_perp_pos_zpt100_met50_old","llnunu_l1_pt>100&&llnunu_old_l2_pt>50&&met_perp_old>0");
  tree_out->Draw("llnunu_l2_pt*sin(llnunu_deltaPhi)>>hmet_perp_pos_zpt100_met50_new", "llnunu_l1_pt>100&&llnunu_l2_pt>50&&met_perp>0");
  tree_out->Draw("llnunu_old_l2_pt*cos(llnunu_old_deltaPhi)>>hmet_para_neg_old", "met_para_old>0");
  tree_out->Draw("llnunu_l2_pt*cos(llnunu_deltaPhi)>>hmet_para_neg_new", "met_para>0");
  tree_out->Draw("llnunu_old_l2_pt*sin(llnunu_old_deltaPhi)>>hmet_perp_neg_old", "met_perp_old>0");
  tree_out->Draw("llnunu_l2_pt*sin(llnunu_deltaPhi)>>hmet_perp_neg_new", "met_perp>0");
  tree_out->Draw("-llnunu_old_l2_pt*cos(llnunu_old_deltaPhi)>>hmet_para_neg_zpt100_old", "llnunu_l1_pt>100&&met_para_old<0");
  tree_out->Draw("-llnunu_l2_pt*cos(llnunu_deltaPhi)>>hmet_para_neg_zpt100_new", "llnunu_l1_pt>100&&met_para<0");
  tree_out->Draw("-llnunu_old_l2_pt*sin(llnunu_old_deltaPhi)>>hmet_perp_neg_zpt100_old", "llnunu_l1_pt>100&&met_perp_old<0");
  tree_out->Draw("-llnunu_l2_pt*sin(llnunu_deltaPhi)>>hmet_perp_neg_zpt100_new", "llnunu_l1_pt>100&&met_perp<0");
  tree_out->Draw("-llnunu_old_l2_pt*cos(llnunu_old_deltaPhi)>>hmet_para_neg_zpt100_met50_old","llnunu_l1_pt>100&&llnunu_old_l2_pt>50&&met_para_old<0");
  tree_out->Draw("-llnunu_l2_pt*cos(llnunu_deltaPhi)>>hmet_para_neg_zpt100_met50_new", "llnunu_l1_pt>100&&llnunu_l2_pt>50&&met_para<0");
  tree_out->Draw("-llnunu_old_l2_pt*sin(llnunu_old_deltaPhi)>>hmet_perp_neg_zpt100_met50_old","llnunu_l1_pt>100&&llnunu_old_l2_pt>50&&met_perp_old<0");
  tree_out->Draw("-llnunu_l2_pt*sin(llnunu_deltaPhi)>>hmet_perp_neg_zpt100_met50_new", "llnunu_l1_pt>100&&llnunu_l2_pt>50&&met_perp<0");

  tree_out->Draw("llnunu_old_l2_pt*cos(llnunu_old_l2_phi)>>hmetx_old");
  tree_out->Draw("llnunu_l2_pt*cos(llnunu_l2_phi)>>hmetx_new");
  tree_out->Draw("llnunu_old_l2_pt*sin(llnunu_old_l2_phi)>>hmety_old");
  tree_out->Draw("llnunu_l2_pt*sin(llnunu_l2_phi)>>hmety_new");
  tree_out->Draw("llnunu_old_l2_pt>>hmet_old");
  tree_out->Draw("llnunu_l2_pt>>hmet_new");
  tree_out->Draw("llnunu_mt_old>>hmt_old");
  tree_out->Draw("llnunu_mt>>hmt_new");
  tree_out->Draw("llnunu_mt_old>>hmt_old");
  tree_out->Draw("llnunu_mt>>hmt_new");
  tree_out->Draw("ut_hard_pt_old*cos(ut_hard_phi_old-llnunu_l1_phi)>>hut_hard_para_old","ut_hard_pt_old!=0");
  tree_out->Draw("ut_hard_pt*cos(ut_hard_phi-llnunu_l1_phi)>>hut_hard_para_new","ut_hard_pt!=0");
  tree_out->Draw("ut_hard_pt_old*sin(ut_hard_phi_old-llnunu_l1_phi)>>hut_hard_perp_old","ut_hard_pt_old!=0");
  tree_out->Draw("ut_hard_pt*sin(ut_hard_phi-llnunu_l1_phi)>>hut_hard_perp_new","ut_hard_pt!=0");

  hdphi_old->SetLineColor(4);
  hdphi_new->SetLineColor(2);
  hdphi_zpt100_old->SetLineColor(4);
  hdphi_zpt100_new->SetLineColor(2);
  hdphi_zpt100_met50_old->SetLineColor(4);
  hdphi_zpt100_met50_new->SetLineColor(2);
  hmetphi_old->SetLineColor(4);
  hmetphi_new->SetLineColor(2);
  hmet_para_old->SetLineColor(6);
  hmet_para_new->SetLineColor(2);
  hmet_perp_old->SetLineColor(8);
  hmet_perp_new->SetLineColor(4);
  hmet_para_zpt100_old->SetLineColor(6);
  hmet_para_zpt100_new->SetLineColor(2);
  hmet_perp_zpt100_old->SetLineColor(8);
  hmet_perp_zpt100_new->SetLineColor(4);
  hmet_para_zpt100_met50_old->SetLineColor(6);
  hmet_para_zpt100_met50_new->SetLineColor(2);
  hmet_perp_zpt100_met50_old->SetLineColor(8);
  hmet_perp_zpt100_met50_new->SetLineColor(4);
  hmet_para_pos_old->SetLineColor(4);
  hmet_para_pos_new->SetLineColor(4);
  hmet_perp_pos_old->SetLineColor(4);
  hmet_perp_pos_new->SetLineColor(4);
  hmet_para_pos_zpt100_old->SetLineColor(4);
  hmet_para_pos_zpt100_new->SetLineColor(4);
  hmet_perp_pos_zpt100_old->SetLineColor(4);
  hmet_perp_pos_zpt100_new->SetLineColor(4);
  hmet_para_pos_zpt100_met50_old->SetLineColor(4);
  hmet_para_pos_zpt100_met50_new->SetLineColor(4);
  hmet_perp_pos_zpt100_met50_old->SetLineColor(4);
  hmet_perp_pos_zpt100_met50_new->SetLineColor(4);
  hmet_para_neg_old->SetLineColor(2);
  hmet_para_neg_new->SetLineColor(2);
  hmet_perp_neg_old->SetLineColor(2);
  hmet_perp_neg_new->SetLineColor(2);
  hmet_para_neg_zpt100_old->SetLineColor(2);
  hmet_para_neg_zpt100_new->SetLineColor(2);
  hmet_perp_neg_zpt100_old->SetLineColor(2);
  hmet_perp_neg_zpt100_new->SetLineColor(2);
  hmet_para_neg_zpt100_met50_old->SetLineColor(2);
  hmet_para_neg_zpt100_met50_new->SetLineColor(2);
  hmet_perp_neg_zpt100_met50_old->SetLineColor(2);
  hmet_perp_neg_zpt100_met50_new->SetLineColor(2);

  hmetx_old->SetLineColor(6);
  hmetx_new->SetLineColor(2);
  hmety_old->SetLineColor(8);
  hmety_new->SetLineColor(4);
  hmet_old->SetLineColor(4);
  hmet_new->SetLineColor(2);
  hmt_old->SetLineColor(4);
  hmt_new->SetLineColor(2);
  hut_hard_para_old->SetLineColor(6);
  hut_hard_para_new->SetLineColor(2);
  hut_hard_perp_old->SetLineColor(8);
  hut_hard_perp_new->SetLineColor(4);


  hdphi_old->GetXaxis()->SetTitle("#Delta#Phi(Z,MET)");
  hdphi_new->GetXaxis()->SetTitle("#Delta#Phi(Z,MET)");
  hdphi_zpt100_old->GetXaxis()->SetTitle("#Delta#Phi(Z,MET)");
  hdphi_zpt100_new->GetXaxis()->SetTitle("#Delta#Phi(Z,MET)");
  hdphi_zpt100_met50_old->GetXaxis()->SetTitle("#Delta#Phi(Z,MET)");
  hdphi_zpt100_met50_new->GetXaxis()->SetTitle("#Delta#Phi(Z,MET)");
  hmetphi_old->GetXaxis()->SetTitle("#Phi(MET)");
  hmetphi_new->GetXaxis()->SetTitle("#Phi(MET)");
  hmet_para_old->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_para_new->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_perp_old->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_perp_new->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_para_zpt100_old->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_para_zpt100_new->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_perp_zpt100_old->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_perp_zpt100_new->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_para_zpt100_met50_old->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_para_zpt100_met50_new->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_perp_zpt100_met50_old->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_perp_zpt100_met50_new->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_para_pos_old->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_para_pos_new->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_perp_pos_old->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_perp_pos_new->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_para_pos_zpt100_old->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_para_pos_zpt100_new->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_perp_pos_zpt100_old->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_perp_pos_zpt100_new->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_para_pos_zpt100_met50_old->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_para_pos_zpt100_met50_new->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_perp_pos_zpt100_met50_old->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_perp_pos_zpt100_met50_new->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_para_neg_old->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_para_neg_new->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_perp_neg_old->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_perp_neg_new->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_para_neg_zpt100_old->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_para_neg_zpt100_new->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_perp_neg_zpt100_old->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_perp_neg_zpt100_new->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_para_neg_zpt100_met50_old->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_para_neg_zpt100_met50_new->GetXaxis()->SetTitle("MET_{#parallel} (GeV)");
  hmet_perp_neg_zpt100_met50_old->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmet_perp_neg_zpt100_met50_new->GetXaxis()->SetTitle("MET_{#perp} (GeV)");
  hmetx_old->GetXaxis()->SetTitle("MET_{x} (GeV)");
  hmetx_new->GetXaxis()->SetTitle("MET_{x} (GeV)");
  hmety_old->GetXaxis()->SetTitle("MET_{y} (GeV)");
  hmety_new->GetXaxis()->SetTitle("MET_{y} (GeV)");
  hmet_old->GetXaxis()->SetTitle("MET (GeV)");
  hmet_new->GetXaxis()->SetTitle("MET (GeV)");
  hmt_old->GetXaxis()->SetTitle("M_{T} (GeV)");
  hmt_new->GetXaxis()->SetTitle("M_{T} (GeV)");
  hut_hard_para_old->GetXaxis()->SetTitle("#sum P^{jets}_{T #parallel} (GeV)");
  hut_hard_para_new->GetXaxis()->SetTitle("#sum P^{jets}_{T #parallel} (GeV)");
  hut_hard_perp_old->GetXaxis()->SetTitle("#sum P^{jets}_{T #perp} (GeV)");
  hut_hard_perp_new->GetXaxis()->SetTitle("#sum P^{jets}_{T #perp} (GeV)");

  TLegend* lgdphi = new TLegend(0.65, 0.7, 0.89, 0.85);
  TLegend* lgdphi_zpt100 = new TLegend(0.65, 0.7, 0.89, 0.85);
  TLegend* lgdphi_zpt100_met50 = new TLegend(0.65, 0.7, 0.89, 0.85);
  TLegend* lgmetphi = new TLegend(0.65, 0.7, 0.89, 0.85);
  TLegend* lgmet_para = new TLegend(0.7, 0.7, 0.89, 0.85);
  TLegend* lgmet_perp = new TLegend(0.7, 0.7, 0.89, 0.85);
  TLegend* lgmet_para_zpt100 = new TLegend(0.7, 0.7, 0.89, 0.85);
  TLegend* lgmet_perp_zpt100 = new TLegend(0.7, 0.7, 0.89, 0.85);
  TLegend* lgmet_para_zpt100_met50 = new TLegend(0.7, 0.7, 0.89, 0.85);
  TLegend* lgmet_perp_zpt100_met50 = new TLegend(0.7, 0.7, 0.89, 0.85);
  TLegend* lgmetx = new TLegend(0.7, 0.7, 0.89, 0.85);
  TLegend* lgmety = new TLegend(0.7, 0.7, 0.89, 0.85);
  TLegend* lgmet = new TLegend(0.7, 0.7, 0.89, 0.85);
  TLegend* lgmt = new TLegend(0.7, 0.7, 0.89, 0.85);
  TLegend* lgut_hard_para = new TLegend(0.7, 0.7, 0.89, 0.85);
  TLegend* lgut_hard_perp = new TLegend(0.7, 0.7, 0.89, 0.85);

  TLegend* lgmet_old = new TLegend(0.7, 0.7, 0.89, 0.89);
  TLegend* lgmet_new = new TLegend(0.7, 0.7, 0.89, 0.89);
  TLegend* lgmet_zpt100_old = new TLegend(0.7, 0.7, 0.89, 0.89);
  TLegend* lgmet_zpt100_new = new TLegend(0.7, 0.7, 0.89, 0.89);
  TLegend* lgmet_zpt100_met50_old = new TLegend(0.7, 0.7, 0.89, 0.89);
  TLegend* lgmet_zpt100_met50_new = new TLegend(0.7, 0.7, 0.89, 0.89);


  lgdphi->AddEntry(hdphi_old,"old","pl");
  lgdphi->AddEntry(hdphi_new,"new","pl");
  lgdphi_zpt100->AddEntry(hdphi_zpt100_old,"old","pl");
  lgdphi_zpt100->AddEntry(hdphi_zpt100_new,"new","pl");
  lgdphi_zpt100_met50->AddEntry(hdphi_zpt100_met50_old,"old","pl");
  lgdphi_zpt100_met50->AddEntry(hdphi_zpt100_met50_new,"new","pl");
  lgmetphi->AddEntry(hmetphi_old,"old","pl");
  lgmetphi->AddEntry(hmetphi_new,"new","pl");
  lgmet_para->AddEntry(hmet_para_old,"old","pl");
  lgmet_para->AddEntry(hmet_para_new,"new","pl");
  lgmet_perp->AddEntry(hmet_perp_old,"old","pl");
  lgmet_perp->AddEntry(hmet_perp_new,"new","pl");
  lgmet_para_zpt100->AddEntry(hmet_para_zpt100_old,"old","pl");
  lgmet_para_zpt100->AddEntry(hmet_para_zpt100_new,"new","pl");
  lgmet_perp_zpt100->AddEntry(hmet_perp_zpt100_old,"old","pl");
  lgmet_perp_zpt100->AddEntry(hmet_perp_zpt100_new,"new","pl");
  lgmet_para_zpt100_met50->AddEntry(hmet_para_zpt100_met50_old,"old","pl");
  lgmet_para_zpt100_met50->AddEntry(hmet_para_zpt100_met50_new,"new","pl");
  lgmet_perp_zpt100_met50->AddEntry(hmet_perp_zpt100_met50_old,"old","pl");
  lgmet_perp_zpt100_met50->AddEntry(hmet_perp_zpt100_met50_new,"new","pl");
  lgmetx->AddEntry(hmetx_old,"old","pl");
  lgmetx->AddEntry(hmetx_new,"new","pl");
  lgmety->AddEntry(hmety_old,"old","pl");
  lgmety->AddEntry(hmety_new,"new","pl");
  lgmet->AddEntry(hmet_old,"old","pl");
  lgmet->AddEntry(hmet_new,"new","pl");
  lgmt->AddEntry(hmt_old,"old","pl");
  lgmt->AddEntry(hmt_new,"new","pl");
  lgut_hard_para->AddEntry(hut_hard_para_old,"old","pl");
  lgut_hard_para->AddEntry(hut_hard_para_new,"new","pl");
  lgut_hard_perp->AddEntry(hut_hard_perp_old,"old","pl");
  lgut_hard_perp->AddEntry(hut_hard_perp_new,"new","pl");
  lgmet_old->AddEntry(hmet_para_old,"MET_{#parallel}","pl");
  lgmet_old->AddEntry(hmet_perp_old,"MET_{#perp}","pl");
  lgmet_new->AddEntry(hmet_para_new,"MET_{#parallel}","pl");
  lgmet_new->AddEntry(hmet_perp_new,"MET_{#perp}","pl");
  lgmet_zpt100_old->AddEntry(hmet_para_zpt100_old,"MET_{#parallel}","pl");
  lgmet_zpt100_old->AddEntry(hmet_perp_zpt100_old,"MET_{#perp}","pl");
  lgmet_zpt100_new->AddEntry(hmet_para_zpt100_new,"MET_{#parallel}","pl");
  lgmet_zpt100_new->AddEntry(hmet_perp_zpt100_new,"MET_{#perp}","pl");
  lgmet_zpt100_met50_old->AddEntry(hmet_para_zpt100_met50_old,"MET_{#parallel}","pl");
  lgmet_zpt100_met50_old->AddEntry(hmet_perp_zpt100_met50_old,"MET_{#perp}","pl");
  lgmet_zpt100_met50_new->AddEntry(hmet_para_zpt100_met50_new,"MET_{#parallel}","pl");
  lgmet_zpt100_met50_new->AddEntry(hmet_perp_zpt100_met50_new,"MET_{#perp}","pl");


  plots->Clear();
  plots->SetLogy(1);
  hdphi_old->Draw();
  hdphi_new->Draw("same");
  lgdphi->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->SetLogy(0);
  plots->Clear();


  plots->Clear();
  plots->SetLogy(1);
  hdphi_zpt100_old->Draw();
  hdphi_zpt100_new->Draw("same");
  lgdphi_zpt100->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->SetLogy(0);
  plots->Clear();

  plots->Clear();
  plots->SetLogy(1);
  hdphi_zpt100_met50_old->Draw();
  hdphi_zpt100_met50_new->Draw("same");
  lgdphi_zpt100_met50->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->SetLogy(0);
  plots->Clear();

  plots->Clear();
  plots->SetLogy(1);
  hmetphi_old->Draw();
  hmetphi_new->Draw("same");
  lgmetphi->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->SetLogy(0);
  plots->Clear();


  plots->Clear();
  plots->SetLogy(1);
  hmet_para_old->Draw();
  hmet_para_new->Draw("same");
  lgmet_para->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->SetLogy(0);
  plots->Clear();

  plots->Clear();
  plots->SetLogy(1);
  hmet_perp_old->Draw();
  hmet_perp_new->Draw("same");
  lgmet_perp->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
  plots->SetLogy(1);
  hmet_para_zpt100_old->Draw();
  hmet_para_zpt100_new->Draw("same");
  lgmet_para_zpt100->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  plots->SetLogy(1);
  hmet_perp_zpt100_old->Draw();
  hmet_perp_zpt100_new->Draw("same");
  lgmet_perp_zpt100->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  plots->SetLogy(1);
  hmet_para_zpt100_met50_old->Draw();
  hmet_para_zpt100_met50_new->Draw("same");
  lgmet_para_zpt100_met50->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
  plots->SetLogy(1);
  hmet_perp_zpt100_met50_old->Draw();
  hmet_perp_zpt100_met50_new->Draw("same");
  lgmet_perp_zpt100_met50->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  plots->SetLogy(1);
  hmetx_old->Draw();
  hmetx_new->Draw("same");
  lumipt->Draw();
  lgmetx->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
  plots->SetLogy(1);
  hmety_old->Draw();
  hmety_new->Draw("same");
  lgmety->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
  plots->SetLogy(1);
  hmet_old->Draw();
  hmet_new->Draw("same");
  lgmet->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
  plots->SetLogy(1);
  hmt_old->Draw();
  hmt_new->Draw("same");
  lgmt->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
  plots->SetLogy(1);
  hut_hard_para_old->Draw();
  hut_hard_para_new->Draw("same");
  lgut_hard_para->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  plots->SetLogy(1);
  hut_hard_perp_old->Draw();
  hut_hard_perp_new->Draw("same");
  lgut_hard_perp->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  plots->SetLogy(1);
  hmet_para_old->Draw();
  hmet_perp_old->Draw("same");
  lgmet_old->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
  plots->SetLogy(1);
  hmet_para_new->Draw();
  hmet_perp_new->Draw("same");
  lgmet_new->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
  plots->SetLogy(1);
  hmet_para_zpt100_old->Draw();
  hmet_perp_zpt100_old->Draw("same");
  lgmet_zpt100_old->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
  plots->SetLogy(1);
  hmet_para_zpt100_new->Draw();
  hmet_perp_zpt100_new->Draw("same");
  lgmet_zpt100_new->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  plots->SetLogy(1);
  hmet_para_zpt100_met50_old->Draw();
  hmet_perp_zpt100_met50_old->Draw("same");
  lgmet_zpt100_met50_old->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
  plots->SetLogy(1);
  hmet_para_zpt100_met50_new->Draw();
  hmet_perp_zpt100_met50_new->Draw("same");
  lgmet_zpt100_met50_new->Draw();
  lumipt->Draw();
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->SetLogy(0);
  plots->Clear();

  plots->Clear();
//  plots->SetLogy(1);
  hdphi_old->Draw();
  hdphi_new->Draw("same");
  lgdphi->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
//  plots->SetLogy(1);
  hdphi_zpt100_old->Draw();
  hdphi_zpt100_new->Draw("same");
  lgdphi_zpt100->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
//  plots->SetLogy(1);
  hdphi_zpt100_met50_old->Draw();
  hdphi_zpt100_met50_new->Draw("same");
  lgdphi_zpt100_met50->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
//  plots->SetLogy(1);
  hmetphi_old->Draw();
  hmetphi_new->Draw("same");
  lgmetphi->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
//  plots->SetLogy(1);
  hmet_para_old->Draw();
  hmet_para_new->Draw("same");
  lgmet_para->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
//  plots->SetLogy(1);
  hmet_perp_old->Draw();
  hmet_perp_new->Draw("same");
  lgmet_perp->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
//  plots->SetLogy(1);
  hmet_para_zpt100_old->Draw();
  hmet_para_zpt100_new->Draw("same");
  lgmet_para_zpt100->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
//  plots->SetLogy(1);
  hmet_perp_zpt100_old->Draw();
  hmet_perp_zpt100_new->Draw("same");
  lgmet_perp_zpt100->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
//  plots->SetLogy(1);
  hmet_para_zpt100_met50_old->Draw();
  hmet_para_zpt100_met50_new->Draw("same");
  lgmet_para_zpt100_met50->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
//  plots->SetLogy(1);
  hmet_perp_zpt100_met50_old->Draw();
  hmet_perp_zpt100_met50_new->Draw("same");
  lgmet_perp_zpt100_met50->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
//  plots->SetLogy(1);
  hmetx_old->Draw();
  hmetx_new->Draw("same");
  lgmetx->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
//  plots->SetLogy(1);
  hmety_old->Draw();
  hmety_new->Draw("same");
  lgmety->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
//  plots->SetLogy(1);
  hmet_old->Draw();
  hmet_new->Draw("same");
  lgmet->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
//  plots->SetLogy(1);
  hmt_old->Draw();
  hmt_new->Draw("same");
  lgmt->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
//  plots->SetLogy(1);
  hut_hard_para_old->Draw();
  hut_hard_para_new->Draw("same");
  lgut_hard_para->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
//  plots->SetLogy(1);
  hut_hard_perp_old->Draw();
  hut_hard_perp_new->Draw("same");
  lgut_hard_perp->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
//  plots->SetLogy(1);
  hmet_para_old->Draw();
  hmet_perp_old->Draw("same");
  lgmet_old->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
//  plots->SetLogy(1);
  hmet_para_new->Draw();
  hmet_perp_new->Draw("same");
  lgmet_new->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
//  plots->SetLogy(1);
  hmet_para_zpt100_old->Draw();
  hmet_perp_zpt100_old->Draw("same");
  lgmet_zpt100_old->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  plots->Clear();
//  plots->SetLogy(1);
  hmet_para_zpt100_new->Draw();
  hmet_perp_zpt100_new->Draw("same");
  lgmet_zpt100_new->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
//  plots->SetLogy(1);
  hmet_para_zpt100_met50_old->Draw();
  hmet_perp_zpt100_met50_old->Draw("same");
  lgmet_zpt100_met50_old->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();

  plots->Clear();
//  plots->SetLogy(1);
  hmet_para_zpt100_met50_new->Draw();
  hmet_perp_zpt100_met50_new->Draw("same");
  lgmet_zpt100_met50_new->Draw();
  lumipt->Draw();
  plots->SetLogy(0);
  sprintf(name, "%s.ps", outputfile.c_str());
  plots->Print(name);
  plots->Clear();


  hdphi_old->Write();
  hdphi_new->Write();
  hdphi_zpt100_old->Write();
  hdphi_zpt100_new->Write();
  hdphi_zpt100_met50_old->Write();
  hdphi_zpt100_met50_new->Write();
  hmetphi_old->Write();
  hmetphi_new->Write();
  hmet_para_old->Write();
  hmet_para_new->Write();
  hmet_perp_old->Write();
  hmet_perp_new->Write();
  hmet_para_zpt100_old->Write();
  hmet_para_zpt100_new->Write();
  hmet_perp_zpt100_old->Write();
  hmet_perp_zpt100_new->Write();
  hmet_para_zpt100_met50_old->Write();
  hmet_para_zpt100_met50_new->Write();
  hmet_perp_zpt100_met50_old->Write();
  hmet_perp_zpt100_met50_new->Write();

  hmet_para_pos_old->Write();
  hmet_para_pos_new->Write();
  hmet_perp_pos_old->Write();
  hmet_perp_pos_new->Write();
  hmet_para_pos_zpt100_old->Write();
  hmet_para_pos_zpt100_new->Write();
  hmet_perp_pos_zpt100_old->Write();
  hmet_perp_pos_zpt100_new->Write();
  hmet_para_pos_zpt100_met50_old->Write();
  hmet_para_pos_zpt100_met50_new->Write();
  hmet_perp_pos_zpt100_met50_old->Write();
  hmet_perp_pos_zpt100_met50_new->Write();
  hmet_para_neg_old->Write();
  hmet_para_neg_new->Write();
  hmet_perp_neg_old->Write();
  hmet_perp_neg_new->Write();
  hmet_para_neg_zpt100_old->Write();
  hmet_para_neg_zpt100_new->Write();
  hmet_perp_neg_zpt100_old->Write();
  hmet_perp_neg_zpt100_new->Write();
  hmet_para_neg_zpt100_met50_old->Write();
  hmet_para_neg_zpt100_met50_new->Write();
  hmet_perp_neg_zpt100_met50_old->Write();
  hmet_perp_neg_zpt100_met50_new->Write();

  hmetx_old->Write();
  hmetx_new->Write();
  hmety_old->Write();
  hmety_new->Write();
  hmet_old->Write();
  hmet_new->Write();
  hmt_old->Write();
  hmt_new->Write();
  hut_hard_para_old->Write();
  hut_hard_para_new->Write();
  hut_hard_perp_old->Write();
  hut_hard_perp_new->Write();

  // other control plots
  Double_t ZPtBins[] = {0,2,4,6,8,10,12,14,16,18,20,22,24,26,28, 30, 35, 40, 50, 60, 80, 100, 150, 250, 5000 };
  Int_t NZPtBins = sizeof(ZPtBins)/sizeof(ZPtBins[0]) - 1;
  const Int_t NMetParaBins=500;
  Double_t MetParaBins[NMetParaBins+1];
  for (int i=0; i<=NMetParaBins; i++) { MetParaBins[i] = -100.0+200.0/NMetParaBins*i; };
  const Int_t NMetPerpBins=500;
  Double_t MetPerpBins[NMetPerpBins+1];
  for (int i=0; i<=NMetPerpBins; i++) { MetPerpBins[i] = -100.0+200.0/NMetPerpBins*i; };

  TH2D* h_met_para_vs_zpt_old = new TH2D("h_met_para_vs_zpt_old", "h_met_para_vs_zpt_old", NZPtBins, ZPtBins, NMetParaBins, MetParaBins);
  TH2D* h_met_para_vs_zpt_new = new TH2D("h_met_para_vs_zpt_new", "h_met_para_vs_zpt_new", NZPtBins, ZPtBins, NMetParaBins, MetParaBins);
  TH2D* h_met_perp_vs_zpt_old = new TH2D("h_met_perp_vs_zpt_old", "h_met_perp_vs_zpt_old", NZPtBins, ZPtBins, NMetPerpBins, MetPerpBins);
  TH2D* h_met_perp_vs_zpt_new = new TH2D("h_met_perp_vs_zpt_new", "h_met_perp_vs_zpt_new", NZPtBins, ZPtBins, NMetPerpBins, MetPerpBins);
  h_met_para_vs_zpt_old->Sumw2();
  h_met_para_vs_zpt_new->Sumw2();
  h_met_perp_vs_zpt_old->Sumw2();
  h_met_perp_vs_zpt_new->Sumw2();
  tree_out->Draw("met_para_old:llnunu_l1_pt>>h_met_para_vs_zpt_old", "", "colz");
  tree_out->Draw("met_para:llnunu_l1_pt>>h_met_para_vs_zpt_new", "", "colz");
  tree_out->Draw("met_perp_old:llnunu_l1_pt>>h_met_perp_vs_zpt_old", "", "colz");
  tree_out->Draw("met_perp:llnunu_l1_pt>>h_met_perp_vs_zpt_new", "", "colz");

  TF1* func = new TF1("func", "gaus", -50, 50);
  func->SetRange(-15, 15);
  TObjArray* aSlices = new TObjArray(100);

  h_met_para_vs_zpt_old->Write();  
  h_met_para_vs_zpt_new->Write();  
  h_met_perp_vs_zpt_old->Write();  
  h_met_perp_vs_zpt_new->Write();  

  h_met_para_vs_zpt_old->FitSlicesY(func, 0, -1, 0, "QNR", aSlices);
  aSlices->Write();
  h_met_para_vs_zpt_new->FitSlicesY(func, 0, -1, 0, "QNR", aSlices);
  aSlices->Write();
  h_met_perp_vs_zpt_old->FitSlicesY(func, 0, -1, 0, "QNR", aSlices);
  aSlices->Write();
  h_met_perp_vs_zpt_new->FitSlicesY(func, 0, -1, 0, "QNR", aSlices);
  aSlices->Write();

  TH1D* h_met_para_vs_zpt_old_slices[100];
  TH1D* h_met_para_vs_zpt_new_slices[100];
  TH1D* h_met_perp_vs_zpt_old_slices[100];
  TH1D* h_met_perp_vs_zpt_new_slices[100];

  

  TH2D* h_jet_perp_vs_para_tightId_lepVeto_zptL10 = new TH2D("h_jet_perp_vs_para_tightId_lepVeto_zptL10", "h_jet_perp_vs_para_tightId_lepVeto_zptL10", 1000,-2000,2000, 1000,-2000, 2000);
  TH2D* h_jet_perp_vs_para_tightId_lepVeto_zptG10L30 = new TH2D("h_jet_perp_vs_para_tightId_lepVeto_zptG10L30", "h_jet_perp_vs_para_tightId_lepVeto_zptG10L30", 1000,-2000,2000, 1000,-2000, 2000);
  TH2D* h_jet_perp_vs_para_tightId_lepVeto_zptG30L60 = new TH2D("h_jet_perp_vs_para_tightId_lepVeto_zptG30L60", "h_jet_perp_vs_para_tightId_lepVeto_zptG30L60", 1000,-2000,2000, 1000,-2000, 2000);
  TH2D* h_jet_perp_vs_para_tightId_lepVeto_zptG60L100 = new TH2D("h_jet_perp_vs_para_tightId_lepVeto_zptG60L100", "h_jet_perp_vs_para_tightId_lepVeto_zptG60L100", 1000,-2000,2000, 1000,-2000, 2000);
  TH2D* h_jet_perp_vs_para_tightId_lepVeto_zptG100L150 = new TH2D("h_jet_perp_vs_para_tightId_lepVeto_zptG100L150", "h_jet_perp_vs_para_tightId_lepVeto_zptG100L150", 1000,-2000,2000, 1000,-2000, 2000);
  TH2D* h_jet_perp_vs_para_tightId_lepVeto_zptG150L250 = new TH2D("h_jet_perp_vs_para_tightId_lepVeto_zptG150L250", "h_jet_perp_vs_para_tightId_lepVeto_zptG150L250", 1000,-2000,2000, 1000,-2000, 2000);
  TH2D* h_jet_perp_vs_para_tightId_lepVeto_zptG250 = new TH2D("h_jet_perp_vs_para_tightId_lepVeto_zptG250", "h_jet_perp_vs_para_tightId_lepVeto_zptG250", 1000,-2000,2000, 1000,-2000, 2000);

  tree_out->Draw("jet_perp:jet_para>>h_jet_perp_vs_para_tightId_lepVeto_zptL10", "njet_corr>0&&llnunu_l1_pt[0]<10", "colz");
  tree_out->Draw("jet_perp:jet_para>>h_jet_perp_vs_para_tightId_lepVeto_zptG10L30", "njet_corr>0&&llnunu_l1_pt[0]>10&&llnunu_l1_pt[0]<=30", "colz");
  tree_out->Draw("jet_perp:jet_para>>h_jet_perp_vs_para_tightId_lepVeto_zptG30L60", "njet_corr>0&&llnunu_l1_pt[0]>30&&llnunu_l1_pt[0]<=60", "colz");
  tree_out->Draw("jet_perp:jet_para>>h_jet_perp_vs_para_tightId_lepVeto_zptG60L100", "njet_corr>0&&llnunu_l1_pt[0]>60&&llnunu_l1_pt[0]<=100", "colz");
  tree_out->Draw("jet_perp:jet_para>>h_jet_perp_vs_para_tightId_lepVeto_zptG100L150", "njet_corr>0&&llnunu_l1_pt[0]>100&&llnunu_l1_pt[0]<=150", "colz");
  tree_out->Draw("jet_perp:jet_para>>h_jet_perp_vs_para_tightId_lepVeto_zptG150L250", "njet_corr>0&&llnunu_l1_pt[0]>150&&llnunu_l1_pt[0]<=250", "colz");
  tree_out->Draw("jet_perp:jet_para>>h_jet_perp_vs_para_tightId_lepVeto_zptG250", "njet_corr>0&&llnunu_l1_pt[0]>250", "colz");

  h_jet_perp_vs_para_tightId_lepVeto_zptL10->Write();
  h_jet_perp_vs_para_tightId_lepVeto_zptG10L30->Write();
  h_jet_perp_vs_para_tightId_lepVeto_zptG30L60->Write();
  h_jet_perp_vs_para_tightId_lepVeto_zptG60L100->Write();
  h_jet_perp_vs_para_tightId_lepVeto_zptG100L150->Write();
  h_jet_perp_vs_para_tightId_lepVeto_zptG150L250->Write();
  h_jet_perp_vs_para_tightId_lepVeto_zptG250->Write();


  sprintf(name, "%s.ps]", outputfile.c_str());
  plots->Print(name);

  tree_out->Write();
  foutput->Close();
  finput->Close();

  sprintf(name, ".! ps2pdf %s.ps  %s.pdf ", outputfile.c_str(), outputfile.c_str());
  gROOT->ProcessLine(name);
 
  return 0;

}



