

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

int main(){
  
  TFile* fin = TFile::Open("/home/heli/XZZ/80X_20170122_light/DYJetsToLL_M50_MGMLM_Ext1/vvTreeProducer/tree.root");
  TFile* fout = TFile::Open("/home/heli/XZZ/80X_20170122_light/DY0JetsToLL_M50_MGMLM_Ext1/vvTreeProducer/tree.root", "recreate");

  TTree* tree = (TTree*)fin->Get("tree");
  fout->cd();
 
  TTree* otree = (TTree*)tree->CopyTree("lheNj==0");

  otree->Write();
  fout->Close(); 
  fin->Close();

  return 0;
}
