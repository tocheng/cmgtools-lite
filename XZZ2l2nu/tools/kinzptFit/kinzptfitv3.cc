#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "TObjArray.h"
#include "TMath.h"
#include "TVector2.h"
#include "TProfile.h"
#include "TGraphErrors.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "kinzptfitv3.h"
#include "JetResolution.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnPrint.h"
#include "Minuit2/Minuit2Minimizer.h"
#include "Minuit2/MinimumBuilder.h"
#include "Math/MinimizerOptions.h"

bool debug = false;
bool doZpTCorr = false;
bool doJetsCorr = false;
bool doMetShift = true;
bool doMetSigma = true;

int Opt = 0;
// 0: "Default" : both +/- allow to vary
// 1: "BackJets" : only allow jets back to the z boost to vary
// 2: "BackBig" : for jets back to the z, only increase jec allowed, for jets same side to the z, only decrease jec allowed
// 21: "BackBigLessConstr" : same as BackBig, but less jer constraint, 1-sigma allowed.
// 3: "DefaultSmallVariation" : both +/- allow to vary, 

//double jer_scale = 0.894823*0.894823;
Float_t jer_scale = 0.894823;

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

  // initialize
  // root files
  TFile* finput = new TFile(inputfile.c_str());
  TFile* foutput = new TFile(outputfile.c_str(), "recreate");

  
  // tree
  TTree* tree = (TTree*)finput->Get("tree");

  // out_tree
  TTree* tree_out = tree->CloneTree(0);


  Float_t llnunu_l1_pt, llnunu_l2_pt, llnunu_l1_phi, llnunu_l2_phi, llnunu_l1_mass;
  Float_t llnunu_l1_l1_pt, llnunu_l1_l1_phi, llnunu_l1_l1_eta;
  Float_t llnunu_l1_l2_pt, llnunu_l1_l2_phi, llnunu_l1_l2_eta;
  Float_t llnunu_mta, llnunu_mtb,llnunu_mtc;
  Float_t llnunu_deltaPhi, llnunu_CosdphiZMet, llnunu_dPTPara, llnunu_dPTParaRel, llnunu_dPTPerp, llnunu_dPTPerpRel;
  Float_t llnunu_l1_deltaPhi;
  Int_t njet;
  Float_t jet_pt[1000], jet_phi[1000], jet_eta[1000], jet_rawFactor[1000];
  Int_t njet_corr;
  Float_t jet_corr_pt[1000], jet_corr_phi[1000], jet_corr_eta[1000], jet_corr_rawFactor[1000];
  Int_t nTrueInt, nVert;
  Float_t rho;
  tree->SetBranchAddress("llnunu_l1_pt",&llnunu_l1_pt);
  tree->SetBranchAddress("llnunu_l2_pt",&llnunu_l2_pt);
  tree->SetBranchAddress("llnunu_l1_phi",&llnunu_l1_phi);
  tree->SetBranchAddress("llnunu_l2_phi",&llnunu_l2_phi);
  tree->SetBranchAddress("llnunu_l1_mass",&llnunu_l1_mass);
  tree->SetBranchAddress("llnunu_l1_deltaPhi",&llnunu_l1_deltaPhi);
  tree->SetBranchAddress("llnunu_l1_l1_pt",&llnunu_l1_l1_pt);
  tree->SetBranchAddress("llnunu_l1_l2_pt",&llnunu_l1_l2_pt);
  tree->SetBranchAddress("llnunu_l1_l1_phi",&llnunu_l1_l1_phi);
  tree->SetBranchAddress("llnunu_l1_l2_phi",&llnunu_l1_l2_phi);
  tree->SetBranchAddress("llnunu_l1_l1_eta",&llnunu_l1_l1_eta);
  tree->SetBranchAddress("llnunu_l1_l2_eta",&llnunu_l1_l2_eta);
  tree->SetBranchAddress("llnunu_mta",&llnunu_mta);
  tree->SetBranchAddress("llnunu_mtc",&llnunu_mtc);
  tree->SetBranchAddress("llnunu_deltaPhi", &llnunu_deltaPhi);
  tree->SetBranchAddress("nTrueInt", &nTrueInt);
  tree->SetBranchAddress("nVert", &nVert);
  tree->SetBranchAddress("rho", &rho);
  tree->SetBranchAddress("njet", &njet);
  tree->SetBranchAddress("njet_corr", &njet_corr);
  tree->SetBranchAddress("jet_pt", jet_pt);
  tree->SetBranchAddress("jet_phi", jet_phi);
  tree->SetBranchAddress("jet_eta", jet_eta);
  tree->SetBranchAddress("jet_rawFactor", jet_rawFactor);
  tree->SetBranchAddress("jet_corr_pt", jet_corr_pt);
  tree->SetBranchAddress("jet_corr_phi", jet_corr_phi);
  tree->SetBranchAddress("jet_corr_eta", jet_corr_eta);
  //tree->SetBranchAddress("jet_corr_rawFactor", jet_corr_rawFactor);
  //tree->SetBranchAddress("jet_corr_jec_corr", jet_corr_jec_corr);
  //tree->SetBranchAddress("jet_corr_jec_corrUp", jet_corr_jec_corrUp);
  //tree->SetBranchAddress("jet_corr_jec_corrDown", jet_corr_jec_corrDown);

  // new branches
  Float_t llnunu_mta_old, llnunu_mtc_old;
  tree_out->Branch("llnunu_mta_old",&llnunu_mta_old,"llnunu_mta_old/F");
  tree_out->Branch("llnunu_mtc_old",&llnunu_mtc_old,"llnunu_mtc_old/F");

  Float_t llnunu_old_l2_pt, llnunu_old_l2_phi, llnunu_old_deltaPhi;
  tree_out->Branch("llnunu_old_l2_pt",&llnunu_old_l2_pt,"llnunu_old_l2_pt/F");
  tree_out->Branch("llnunu_old_l2_phi",&llnunu_old_l2_phi,"llnunu_old_l2_phi/F");
  tree_out->Branch("llnunu_old_deltaPhi",&llnunu_old_deltaPhi,"llnunu_old_deltaPhi/F");

  Float_t jet_corr_jec_corr[1000], jet_corr_jec_corrUp[1000], jet_corr_jec_corrDown[1000];
  tree_out->Branch("jet_corr_jec_corr",jet_corr_jec_corr,"jet_corr_jec_corr[njet_corr]/F");
  tree_out->Branch("jet_corr_jec_corrUp",jet_corr_jec_corrUp,"jet_corr_jec_corrUp[njet_corr]/F");
  tree_out->Branch("jet_corr_jec_corrDown",jet_corr_jec_corrDown,"jet_corr_jec_corrDown[njet_corr]/F");


  // ut hard
  Float_t ut_hard_para, ut_hard_perp, ut_hard_pt, ut_hard_phi, ut_hard_para_old, ut_hard_perp_old, ut_hard_pt_old, ut_hard_phi_old;
  tree_out->Branch("ut_hard_pt",&ut_hard_pt,"ut_hard_pt/F");
  tree_out->Branch("ut_hard_phi",&ut_hard_phi,"ut_hard_phi/F");
  tree_out->Branch("ut_hard_pt_old",&ut_hard_pt_old,"ut_hard_pt_old/F");
  tree_out->Branch("ut_hard_phi_old",&ut_hard_phi_old,"ut_hard_phi_old/F");

  // bisector phi
  Float_t xi_phi;
  tree_out->Branch("xi_phi",&xi_phi,"xi_phi/F");

  // alias
  tree_out->SetAlias("ut_hard_para", "(ut_hard_pt*cos(ut_hard_phi-llnunu_l1_phi))");
  tree_out->SetAlias("ut_hard_perp", "(ut_hard_pt*sin(ut_hard_phi-llnunu_l1_phi))");
  tree_out->SetAlias("ut_hard_para_old", "(ut_hard_pt_old*cos(ut_hard_phi_old-llnunu_l1_phi))");
  tree_out->SetAlias("ut_hard_perp_old", "(ut_hard_pt_old*sin(ut_hard_phi_old-llnunu_l1_phi))");

  tree_out->SetAlias("met_para", "(llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi))");
  tree_out->SetAlias("met_perp", "(llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi))");
  tree_out->SetAlias("met_para_old", "(llnunu_old_l2_pt*cos(llnunu_old_l2_phi-llnunu_l1_phi))");
  tree_out->SetAlias("met_perp_old", "(llnunu_old_l2_pt*sin(llnunu_old_l2_phi-llnunu_l1_phi))");

  // met shifts
  TFile* file_metshifts;
  //if (doMetShift&&!doZpTCorr&&doJetsCorr) {
  //  file_metshifts = new TFile("file_met_para_shift_vs_zpt_dyjets_jetpt15.root");
  //}
  //else if (doMetShift&&!doZpTCorr&&!doJetsCorr) {
    //file_metshifts = new TFile("file_met_para_shift_vs_zpt_dyjets_jetpt15_forShiftOnly.root");
    file_metshifts = new TFile("file_met_para_shift_vs_zpt_dyjets_jetpt15_forShiftOnly_Corr.root");
  //}
  //else {
  //  file_metshifts = new TFile("file_met_para_shift_vs_zpt_dyjets_jetpt15.root");
  //}

  TH1D* h_metshifts = (TH1D*)file_metshifts->Get("h_met_para_vs_zpt_shift");
  TGraph* gr_metshifts = new TGraph(h_metshifts);

  // Met resolution correction
  TFile* file_metsigma;
  //file_metsigma = new TFile("h_met_sigma_para_vs_perp_met_para_shifted.root");
  file_metsigma = new TFile("h_met_sigma_para_vs_perp_met_para_shifted_corr.root");
  TH1D* h_met_sigma = (TH1D*)file_metsigma->Get("h_met_sigma_para_vs_perp_new");

  // ZPT corr model
  TFile* file_zptcorr = new TFile("file_zpt_corr.root");
  TProfile* pr_zptcorr = (TProfile*)file_zptcorr->Get("pr_zptdiff_vs_zpt_new");

  // JER
  JME::JetResolution JERReso("Summer15_25nsV6_MC_PtResolution_AK4PFchs.txt");
  JME::JetParameters JERPars;


  int nentries = (int)tree->GetEntries();
  if (debug) nentries = 10;
  for (int i=0; i<nentries;  i++){
    tree->GetEntry(i);
 
    if (debug) {
       std::cout << "Event " << i << ", njets = " << njet << ", zpt = " << llnunu_l1_pt << std::endl; 
    } 
    else if ( i%1000 == 0 ) {
       std::cout << "Event " << i << ", njets = " << njet << ", zpt = " << llnunu_l1_pt << std::endl; 
    }
    double z_pt = llnunu_l1_pt;
    double z_phi = llnunu_l1_phi;
    double met_pt = llnunu_l2_pt;
    double met_phi = llnunu_l2_phi;
    std::vector<double> jets_pt;    
    std::vector<double> jets_phi;
    std::vector<double> jets_reso;    

    // calculate the bisector direction phi for later use
    TVector2 Xi_Direction = (TVector2(cos(llnunu_l1_l1_phi),sin(llnunu_l1_l2_phi))+TVector2(cos(llnunu_l1_l2_phi),sin(llnunu_l1_l2_phi))).Unit();
    xi_phi = TVector2::Phi_mpi_pi(Xi_Direction.Phi());

    // check the results, to be add up as the corrected met. 
    double met_para = llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi);
    double met_perp = llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi);

    if (doZpTCorr) {
      met_para -= pr_zptcorr->GetBinContent(pr_zptcorr->FindBin(z_pt));
    }

    // met shifts
    if (doMetShift) {
      met_para -= h_metshifts->GetBinContent(h_metshifts->FindBin(llnunu_l1_pt));
    }

    // met resolution correction
    if (doMetSigma) {
      met_para /= h_met_sigma->GetBinContent(h_met_sigma->FindBin(llnunu_l1_pt));
    }

    // lepton info
    if (debug) {
     // std::cout << " l1 : pt = " << llnunu_l1_l1_pt << ", eta = " << llnunu_l1_l1_eta << ", phi = " << llnunu_l1_l1_phi << std::endl; 
     // std::cout << " l2 : pt = " << llnunu_l1_l2_pt << ", eta = " << llnunu_l1_l2_eta << ", phi = " << llnunu_l1_l2_phi << std::endl; 
    }
    for (int j=0; j<njet; j++){

       // lepton veto
       double deltaR1 = sqrt( pow(jet_eta[j]-llnunu_l1_l1_eta,2) + pow( TVector2::Phi_mpi_pi(jet_phi[j]-llnunu_l1_l1_phi),2) );
       double deltaR2 = sqrt( pow(jet_eta[j]-llnunu_l1_l2_eta,2) + pow( TVector2::Phi_mpi_pi(jet_phi[j]-llnunu_l1_l2_phi),2) );

       if (debug) {
     //    std::cout << " jet " << j << " : pt = " << jet_pt[j] << ", eta = " << jet_eta[j] << ", phi = " << jet_phi[j] << ", dR l1 = " << deltaR1 << ", dR l2 = " << deltaR2 << std::endl; 
       }
       
       if (jet_pt[j]<15.0) continue;
       if (deltaR1<0.4||deltaR2<0.4) continue;

       JERPars.setJetPt(jet_pt[j]);
       JERPars.setJetEta(jet_eta[j]);
       JERPars.setRho(rho);
       jets_reso.push_back(JERReso.getResolution(JERPars));
       jets_pt.push_back(jet_pt[j]);
       jets_phi.push_back(jet_phi[j]);
    }

    njet_corr = (Int_t)jets_pt.size();


    // calculate ut hard old and new
    ut_hard_para = 0;
    ut_hard_perp = 0;
    ut_hard_para_old = 0;
    ut_hard_perp_old = 0;
    ut_hard_pt = 0;
    ut_hard_phi = 0;
    ut_hard_pt_old = 0;
    ut_hard_phi_old = 0;

    // do minut fit of jet energy scale if njet_corr > 0 
    if (doJetsCorr && njet_corr>0) {
      ROOT::Math::MinimizerOptions::SetDefaultPrintLevel(0);
      MetChi2Fcn metChi2; 
      metChi2.InitFunction(z_pt, z_phi,met_pt, met_phi, jets_pt, jets_phi, jets_reso);
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
      } 
      ROOT::Minuit2::MnMigrad migrad(metChi2, upars);
      ROOT::Minuit2::FunctionMinimum min = migrad(0, 1e-7);
      //std::cout << "Minimum: " << min << std::endl;

      // fitted jet scale starting from upars[k]
      for (int j=0; j<njet_corr; j++){
        jet_corr_jec_corr[j]= min.UserParameters().Value(j);
        jet_corr_jec_corrUp[j]= min.UserParameters().Value(j)+min.UserParameters().Error(j);
        jet_corr_jec_corrDown[j]= min.UserParameters().Value(j)-min.UserParameters().Error(j);
        jet_corr_pt[j] = jets_pt.at(j)*jet_corr_jec_corr[j];
        jet_corr_phi[j] = jets_phi.at(j);
        met_para += (1.0-jet_corr_jec_corr[j])*jets_pt.at(j)*cos(jets_phi.at(j)-llnunu_l1_phi);
        met_perp += (1.0-jet_corr_jec_corr[j])*jets_pt.at(j)*sin(jets_phi.at(j)-llnunu_l1_phi);

        ut_hard_para_old += jets_pt.at(j)*cos(jets_phi.at(j)-llnunu_l1_phi);
        ut_hard_perp_old += jets_pt.at(j)*sin(jets_phi.at(j)-llnunu_l1_phi);
        ut_hard_para += jet_corr_jec_corr[j]*jets_pt.at(j)*cos(jets_phi.at(j)-llnunu_l1_phi);
        ut_hard_perp += jet_corr_jec_corr[j]*jets_pt.at(j)*sin(jets_phi.at(j)-llnunu_l1_phi);

        if (debug) {
          std::cout << "  -jet " << j << " : scale = " << min.UserParameters().Value(j) << " +- " << min.UserParameters().Error(j) 
                    << ", jetpt = " << jets_pt.at(j) << " : " << jet_corr_pt[j] << "; jetphi = " << jets_phi.at(j) 
                    << "; jet reso = " << jets_reso.at(j)  
                   << std::endl;
        }
      }  

      //
      TVector2 vec_ut_hard_zframe(ut_hard_para, ut_hard_perp);
      ut_hard_pt = vec_ut_hard_zframe.Mod();
      ut_hard_phi = TVector2::Phi_mpi_pi(vec_ut_hard_zframe.Rotate(llnunu_l1_phi).Phi()); 
      TVector2 vec_ut_hard_old_zframe(ut_hard_para_old, ut_hard_perp_old);
      ut_hard_pt_old = vec_ut_hard_old_zframe.Mod();
      ut_hard_phi_old = TVector2::Phi_mpi_pi(vec_ut_hard_old_zframe.Rotate(llnunu_l1_phi).Phi()); 

    }


    // met_x met_y
    double met_x = met_para*cos(llnunu_l1_phi)-met_perp*sin(llnunu_l1_phi);
    double met_y = met_para*sin(llnunu_l1_phi)+met_perp*cos(llnunu_l1_phi);
    TVector2 vec_met(met_x, met_y);
    double met = vec_met.Mod();
    double metphi = TVector2::Phi_mpi_pi(vec_met.Phi());
    double deltaPhi = TVector2::Phi_mpi_pi(metphi-llnunu_l1_phi);
    double et1 = TMath::Sqrt(llnunu_l1_mass*llnunu_l1_mass + llnunu_l1_pt*llnunu_l1_pt);
    double et2 = TMath::Sqrt(91.188*91.188+met*met);
    double mta = TMath::Sqrt(llnunu_l1_mass*llnunu_l1_mass + 91.188*91.188 + 2.0* (et1*et2 - llnunu_l1_pt*cos(llnunu_l1_phi)*met_x - llnunu_l1_pt*sin(llnunu_l1_phi)*met_y));
    double etc1 = TMath::Sqrt(llnunu_l1_mass*llnunu_l1_mass + llnunu_l1_pt*llnunu_l1_pt);
    double etc2 = TMath::Sqrt(llnunu_l1_mass*llnunu_l1_mass+met*met);
    double mtc = TMath::Sqrt(2.0*llnunu_l1_mass*llnunu_l1_mass + 2.0* (etc1*etc2 - llnunu_l1_pt*cos(llnunu_l1_phi)*met_x - llnunu_l1_pt*sin(llnunu_l1_phi)*met_y));


    if (debug) {
      std::cout << " old:  met = " << llnunu_l2_pt << ", metphi = " << llnunu_l2_phi << ", deltaPhi = " << -llnunu_deltaPhi 
        << ", met_para = " << llnunu_l2_pt*cos(llnunu_l2_phi-llnunu_l1_phi) 
        << ", met_perp = " << llnunu_l2_pt*sin(llnunu_l2_phi-llnunu_l1_phi) 
        << ", mt = " << llnunu_mta << std::endl;
      std::cout << " new:  met = " << met << ", metphi = " << metphi << ", deltaPhi = " << deltaPhi 
        << ", met_para = " << met*cos(deltaPhi)
        << ", met_perp = " << met*sin(deltaPhi)
        << ", mt = " << mta << std::endl;
    }

    llnunu_old_l2_pt = (Float_t)llnunu_l2_pt;
    llnunu_old_l2_phi = (Float_t)llnunu_l2_phi;
    llnunu_old_deltaPhi = (Float_t)TVector2::Phi_mpi_pi(llnunu_l2_phi-llnunu_l1_phi);
    
    llnunu_l2_pt = (Float_t)met;
    llnunu_l2_phi = (Float_t)metphi;
    llnunu_deltaPhi = (Float_t)deltaPhi;
    llnunu_mta_old = (Float_t)llnunu_mta; 
    llnunu_mta = (Float_t)mta;
    llnunu_mtc_old = (Float_t)llnunu_mtc;
    llnunu_mtc = (Float_t)mtc;
 
    tree_out->Fill();

  }

  foutput->cd();

  // draw some control plots
  TH1D* hdphi_old = new TH1D("hdphi_old", "dphi", 36, 0, TMath::Pi());
  TH1D* hdphi_new = new TH1D("hdphi_new", "dphi", 36, 0, TMath::Pi());
  TH1D* hdphi_zpt100_old = new TH1D("hdphi_zpt100_old", "dphi_zpt100", 36, 0, TMath::Pi());
  TH1D* hdphi_zpt100_new = new TH1D("hdphi_zpt100_new", "dphi_zpt100", 36, 0, TMath::Pi());
  TH1D* hdphi_zpt100_met50_old = new TH1D("hdphi_zpt100_met50_old", "dphi_zpt100_met50", 36, 0, TMath::Pi());
  TH1D* hdphi_zpt100_met50_new = new TH1D("hdphi_zpt100_met50_new", "dphi_zpt100_met50", 36, 0, TMath::Pi());
  TH1D* hmetphi_old = new TH1D("hmetphi_old", "met phi", 100, -TMath::Pi(), TMath::Pi());
  TH1D* hmetphi_new = new TH1D("hmetphi_new", "met phi", 100, -TMath::Pi(), TMath::Pi());
  TH1D* hmet_para_old = new TH1D("hmet_para_old", "met para", 1000, -200, 200);
  TH1D* hmet_para_new = new TH1D("hmet_para_new", "met para", 1000, -200, 200);
  TH1D* hmet_perp_old = new TH1D("hmet_perp_old", "met perp", 1000, -200, 200);
  TH1D* hmet_perp_new = new TH1D("hmet_perp_new", "met perp", 1000, -200, 200);
  TH1D* hmet_para_zpt100_old = new TH1D("hmet_para_zpt100_old", "met para_zpt100", 200, -200, 200);
  TH1D* hmet_para_zpt100_new = new TH1D("hmet_para_zpt100_new", "met para_zpt100", 200, -200, 200);
  TH1D* hmet_perp_zpt100_old = new TH1D("hmet_perp_zpt100_old", "met perp_zpt100", 200, -200, 200);
  TH1D* hmet_perp_zpt100_new = new TH1D("hmet_perp_zpt100_new", "met perp_zpt100", 200, -200, 200);
  TH1D* hmet_para_zpt100_met50_old = new TH1D("hmet_para_zpt100_met50_old", "met para_zpt100_met50", 200, -200, 200);
  TH1D* hmet_para_zpt100_met50_new = new TH1D("hmet_para_zpt100_met50_new", "met para_zpt100_met50", 200, -200, 200);
  TH1D* hmet_perp_zpt100_met50_old = new TH1D("hmet_perp_zpt100_met50_old", "met perp_zpt100_met50", 200, -200, 200);
  TH1D* hmet_perp_zpt100_met50_new = new TH1D("hmet_perp_zpt100_met50_new", "met perp_zpt100_met50", 200, -200, 200);
  TH1D* hmet_para_pos_old = new TH1D("hmet_para_pos_old", "met para", 500, 0, 200);
  TH1D* hmet_para_pos_new = new TH1D("hmet_para_pos_new", "met para", 500, 0, 200);
  TH1D* hmet_perp_pos_old = new TH1D("hmet_perp_pos_old", "met perp", 500, 0, 200);
  TH1D* hmet_perp_pos_new = new TH1D("hmet_perp_pos_new", "met perp", 500, 0, 200);
  TH1D* hmet_para_pos_zpt100_old = new TH1D("hmet_para_pos_zpt100_old", "met para_zpt100", 100, 0, 200);
  TH1D* hmet_para_pos_zpt100_new = new TH1D("hmet_para_pos_zpt100_new", "met para_zpt100", 100, 0, 200);
  TH1D* hmet_perp_pos_zpt100_old = new TH1D("hmet_perp_pos_zpt100_old", "met perp_zpt100", 100, 0, 200);
  TH1D* hmet_perp_pos_zpt100_new = new TH1D("hmet_perp_pos_zpt100_new", "met perp_zpt100", 100, 0, 200);
  TH1D* hmet_para_pos_zpt100_met50_old = new TH1D("hmet_para_pos_zpt100_met50_old", "met para_zpt100_met50", 100, 0, 200);
  TH1D* hmet_para_pos_zpt100_met50_new = new TH1D("hmet_para_pos_zpt100_met50_new", "met para_zpt100_met50", 100, 0, 200);
  TH1D* hmet_perp_pos_zpt100_met50_old = new TH1D("hmet_perp_pos_zpt100_met50_old", "met perp_zpt100_met50", 100, 0, 200);
  TH1D* hmet_perp_pos_zpt100_met50_new = new TH1D("hmet_perp_pos_zpt100_met50_new", "met perp_zpt100_met50", 100, 0, 200);
  TH1D* hmet_para_neg_old = new TH1D("hmet_para_neg_old", "met para", 500, 0, 200);
  TH1D* hmet_para_neg_new = new TH1D("hmet_para_neg_new", "met para", 500, 0, 200);
  TH1D* hmet_perp_neg_old = new TH1D("hmet_perp_neg_old", "met perp", 500, 0, 200);
  TH1D* hmet_perp_neg_new = new TH1D("hmet_perp_neg_new", "met perp", 500, 0, 200);
  TH1D* hmet_para_neg_zpt100_old = new TH1D("hmet_para_neg_zpt100_old", "met para_zpt100", 100, 0, 200);
  TH1D* hmet_para_neg_zpt100_new = new TH1D("hmet_para_neg_zpt100_new", "met para_zpt100", 100, 0, 200);
  TH1D* hmet_perp_neg_zpt100_old = new TH1D("hmet_perp_neg_zpt100_old", "met perp_zpt100", 100, 0, 200);
  TH1D* hmet_perp_neg_zpt100_new = new TH1D("hmet_perp_neg_zpt100_new", "met perp_zpt100", 100, 0, 200);
  TH1D* hmet_para_neg_zpt100_met50_old = new TH1D("hmet_para_neg_zpt100_met50_old", "met para_zpt100_met50", 100, 0, 200);
  TH1D* hmet_para_neg_zpt100_met50_new = new TH1D("hmet_para_neg_zpt100_met50_new", "met para_zpt100_met50", 100, 0, 200);
  TH1D* hmet_perp_neg_zpt100_met50_old = new TH1D("hmet_perp_neg_zpt100_met50_old", "met perp_zpt100_met50", 100, 0, 200);
  TH1D* hmet_perp_neg_zpt100_met50_new = new TH1D("hmet_perp_neg_zpt100_met50_new", "met perp_zpt100_met50", 100, 0, 200);


  TH1D* hmetx_old = new TH1D("hmetx_old", "metx", 1000, -200, 200);
  TH1D* hmetx_new = new TH1D("hmetx_new", "metx", 1000, -200, 200);
  TH1D* hmety_old = new TH1D("hmety_old", "mety", 1000, -200, 200);
  TH1D* hmety_new = new TH1D("hmety_new", "mety", 1000, -200, 200);
  TH1D* hmet_old = new TH1D("hmet_old", "met", 1000, 0, 2000);
  TH1D* hmet_new = new TH1D("hmet_new", "met", 1000, 0, 2000);
  TH1D* hmta_old = new TH1D("hmta_old", "mta", 1000, 0, 3000);
  TH1D* hmta_new = new TH1D("hmta_new", "mta", 1000, 0, 3000);
  TH1D* hmtc_old = new TH1D("hmtc_old", "mtc", 1000, 0, 3000);
  TH1D* hmtc_new = new TH1D("hmtc_new", "mtc", 1000, 0, 3000);

  TH1D* hut_hard_para_old = new TH1D("hut_hard_para_old", "ut_hard para", 2000, -500, 500);
  TH1D* hut_hard_para_new = new TH1D("hut_hard_para_new", "ut_hard para", 2000, -500, 500);
  TH1D* hut_hard_perp_old = new TH1D("hut_hard_perp_old", "ut_hard perp", 2000, -500, 500);
  TH1D* hut_hard_perp_new = new TH1D("hut_hard_perp_new", "ut_hard perp", 2000, -500, 500);

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
  hmta_old->Sumw2();
  hmta_new->Sumw2();
  hmtc_old->Sumw2();
  hmtc_new->Sumw2();
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
  tree_out->Draw("llnunu_mta_old>>hmta_old");
  tree_out->Draw("llnunu_mta>>hmta_new");
  tree_out->Draw("llnunu_mtc_old>>hmtc_old");
  tree_out->Draw("llnunu_mtc>>hmtc_new");
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
  hmta_old->SetLineColor(4);
  hmta_new->SetLineColor(2);
  hmtc_old->SetLineColor(4);
  hmtc_new->SetLineColor(2);
  hut_hard_para_old->SetLineColor(6);
  hut_hard_para_new->SetLineColor(2);
  hut_hard_perp_old->SetLineColor(8);
  hut_hard_perp_new->SetLineColor(4);

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
  hmta_old->Write();
  hmta_new->Write();
  hmtc_old->Write();
  hmtc_new->Write();
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


  tree_out->Write();
  foutput->Close();
  finput->Close();

  return 0;

}



