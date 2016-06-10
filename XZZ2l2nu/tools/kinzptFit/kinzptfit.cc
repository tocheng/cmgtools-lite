#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1D.h"
#include "TMath.h"
#include "TVector2.h"
#include "TGraphErrors.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "kinzptfit.h"
#include "JetResolution.h"
#include "Minuit2/MnMigrad.h"
#include "Minuit2/FunctionMinimum.h"
#include "Minuit2/MnPrint.h"

//bool debug = false;
bool debug = true;

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
  Float_t llnunu_mta, llnunu_mtb,llnunu_deltaPhi, llnunu_CosdphiZMet, llnunu_dPTPara, llnunu_dPTParaRel, llnunu_dPTPerp, llnunu_dPTPerpRel;
  Float_t llnunu_l1_deltaPhi;
  Int_t njet;
  Float_t jet_pt[1000], jet_phi[1000], jet_eta[1000], jet_rawFactor[1000];
  Int_t njet_corr;
  Float_t jet_corr_pt[1000], jet_corr_phi[1000], jet_corr_eta[1000], jet_corr_rawFactor[1000];
  Float_t jet_corr_jec_corr[1000], jet_corr_jec_corrUp[1000], jet_corr_jec_corrDown[1000];
  Int_t nTrueInt, nVert;

  tree->SetBranchAddress("llnunu_l1_pt",&llnunu_l1_pt);
  tree->SetBranchAddress("llnunu_l2_pt",&llnunu_l2_pt);
  tree->SetBranchAddress("llnunu_l1_phi",&llnunu_l1_phi);
  tree->SetBranchAddress("llnunu_l2_phi",&llnunu_l2_phi);
  tree->SetBranchAddress("llnunu_l1_mass",&llnunu_l1_mass);
  tree->SetBranchAddress("llnunu_l1_deltaPhi",&llnunu_l1_deltaPhi);
  tree->SetBranchAddress("llnunu_mta",&llnunu_mta);
  tree->SetBranchAddress("llnunu_mtb",&llnunu_mtb);
  tree->SetBranchAddress("llnunu_deltaPhi", &llnunu_deltaPhi);
  tree->SetBranchAddress("nTrueInt", &nTrueInt);
  tree->SetBranchAddress("nVert", &nVert);
  tree->SetBranchAddress("njet", &njet);
  tree->SetBranchAddress("njet_corr", &njet_corr);
  tree->SetBranchAddress("jet_pt", jet_pt);
  tree->SetBranchAddress("jet_phi", jet_phi);
  tree->SetBranchAddress("jet_eta", jet_eta);
  tree->SetBranchAddress("jet_rawFactor", jet_rawFactor);
  tree->SetBranchAddress("jet_corr_pt", jet_corr_pt);
  tree->SetBranchAddress("jet_corr_phi", jet_corr_phi);
  tree->SetBranchAddress("jet_corr_eta", jet_corr_eta);
  tree->SetBranchAddress("jet_corr_rawFactor", jet_corr_rawFactor);
  tree->SetBranchAddress("jet_corr_jec_corr", jet_corr_jec_corr);
  tree->SetBranchAddress("jet_corr_jec_corrUp", jet_corr_jec_corrUp);
  tree->SetBranchAddress("jet_corr_jec_corrDown", jet_corr_jec_corrDown);

  // JER
  JME::JetResolution JetReso("Summer15_25nsV6_MC_PtResolution_AK4PFchs.txt");
  JME::JetParameters JetPars;

  // Minuit function
  MetChi2Fcn metChi2; 


  //for (int i=0; i<(int)tree->GetEntries(); i++){
  for (int i=0; i<10; i++){
    tree->GetEntry(i);
 
    if (debug) {
       std::cout << "Event " << i << ", njets = " << njet << std::endl; 
    } 
    else if ( i%1000 == 0 ) {
       std::cout << "Event " << i << ", njets = " << njet << std::endl; 
    }
    double z_pt = llnunu_l1_pt;
    double met_para = llnunu_l2_pt*cos(llnunu_l1_phi-llnunu_l2_phi);
    double met_perp = llnunu_l2_pt*sin(llnunu_l1_phi-llnunu_l2_phi);
    double met_pt = llnunu_l2_pt;
    double met_dphi = TVector2::Phi_mpi_pi(llnunu_l1_phi-llnunu_l2_phi);
    std::vector<double> jets_pt;    
    std::vector<double> jets_dphi;    
    std::vector<double> jets_para;    
    std::vector<double> jets_perp;    
    std::vector<double> jets_reso;    
    std::vector<double> jets_scale;

    for (int j=0; j<njet; j++){
       JetPars.setJetPt(jet_pt[j]);
       JetPars.setJetEta(jet_eta[j]);
       JetPars.setRho(nVert);
       jets_reso.push_back(JetReso.getResolution(JetPars));
       jets_para.push_back(jet_pt[j]*cos(jet_phi[j]-llnunu_l1_phi));
       jets_perp.push_back(jet_pt[j]*sin(jet_phi[j]-llnunu_l1_phi));
       jets_pt.push_back(jet_pt[j]);
       jets_dphi.push_back(TVector2::Phi_mpi_pi(jet_phi[j]-llnunu_l1_phi));
       jets_scale.push_back(1.0);
    }

    //std::cout << "  n_jets_reso = " << jets_reso.size() << ", n_jets_para = " << jets_para.size() << ", n_jets_scale = " << jets_scale.size() << std::endl;

    metChi2.InitFunction(z_pt, met_pt, met_dphi, jets_pt, jets_dphi, jets_reso);
    ROOT::Minuit2::MnUserParameters upars;
   
    for (int j=0; j<njet; j++){
      sprintf(name, "jet_%d", j);
      //upars.Add(name, jets_scale.at(j),fabs(jets_reso.at(j)-1.0));
      //upars.Add(name, jets_scale.at(j),fabs(jets_reso.at(j)), 1-2*fabs(jets_reso.at(j)), 1+2*fabs(jets_reso.at(j)));
      upars.Add(name, jets_scale.at(j),fabs(jets_reso.at(j))*0.01, 1-fabs(jets_reso.at(j)), 1+fabs(jets_reso.at(j)));
    } 

    ROOT::Minuit2::MnMigrad migrad(metChi2, upars);
    
    ROOT::Minuit2::FunctionMinimum min = migrad();
    
//    std::cout << "Minimum: " << min << std::endl;
   
    // check the results
    double met_x = llnunu_l2_pt*cos(llnunu_l2_phi);
    double met_y = llnunu_l2_pt*sin(llnunu_l2_phi);

    for (int j=0; j<njet; j++){
      jet_corr_jec_corr[j]= min.UserParameters().Value(j);
      jet_corr_jec_corrUp[j]= min.UserParameters().Value(j)+min.UserParameters().Error(j);
      jet_corr_jec_corrDown[j]= min.UserParameters().Value(j)-min.UserParameters().Error(j);
      jet_corr_rawFactor[j] = 1./jet_corr_jec_corr[j];
      jet_corr_pt[j] = jet_pt[j]*jet_corr_jec_corr[j];
      met_x += (jet_corr_jec_corr[j]-1.0)*jet_corr_pt[j]*cos(jet_phi[j]);
      met_y += (jet_corr_jec_corr[j]-1.0)*jet_corr_pt[j]*sin(jet_phi[j]);
      if (debug) {
        std::cout << "  -jet " << j << " : scale = " << min.UserParameters().Value(j) << " +- " << min.UserParameters().Error(j) << ", jetpt = " << jet_pt[j] << " : " << jet_corr_pt[j] << "; jet reso = " << jets_reso.at(j) 
               << std::endl;
        //std::cout << "  -jet " << j << " : " << min.UserParameters().Value(j) << ", old = " << jets_scale.at(j) << std::endl;
      }
    }   

    TVector2 vec_met(met_x, met_y);
    double met = vec_met.Mod();
    double metphi = TVector2::Phi_mpi_pi(vec_met.Phi());
    double deltaPhi = TVector2::Phi_mpi_pi(metphi-llnunu_l1_phi);
    double et1 = TMath::Sqrt(llnunu_l1_mass*llnunu_l1_mass + llnunu_l1_pt*llnunu_l1_pt);
    double et2 = TMath::Sqrt(91.188*91.188+met*met);
    double mtb = TMath::Sqrt(llnunu_l1_mass*llnunu_l1_mass + 91.188*91.188 + 2.0* (et1*et2 - llnunu_l1_pt*cos(llnunu_l1_phi)*met_x - llnunu_l1_pt*sin(llnunu_l1_phi)*met_y));

    if (debug) {
      std::cout << "  -met : " << llnunu_l2_pt << " : " << met << ", metphi : " << llnunu_l2_phi << " : " << metphi 
          << ", deltaPhi = " << llnunu_deltaPhi << " : " << fabs(deltaPhi) << ", mt = " << llnunu_mta << " : " << mtb << std::endl;
    }
    llnunu_l2_pt = met;
    llnunu_l2_phi = metphi;
    llnunu_deltaPhi = deltaPhi;
    llnunu_mtb = mtb; 
 
    tree_out->Fill();

  }

  foutput->cd();
  tree_out->Write();
  foutput->Close();
  finput->Close();

  return 0;

}



