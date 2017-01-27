
{

  TFile* fout = TFile::Open("muon_trg_summer16.root", "recreate");
 
  TFile* f_mu50_own_1= TFile::Open("hlt50muon_2016fulleff_absetapt.root");
  TFile* f_mu50_own_2= TFile::Open("hlt50muon_2016fulleff_absetapt.root");
  TFile* f_mu50_own_3= TFile::Open("hlt50muon_2016fulleff_absetapt.root");
  TFile* f_mu50_own_4= TFile::Open("hlt50muon_2016fulleff_absetapt.root");

  TFile* f_mu50tkmu50_pog_1 = TFile::Open("Trigger_Mu50TkMu50_EfficienciesAndSF_Period3.root");
  TFile* f_mu50tkmu50_pog_2 = TFile::Open("Trigger_Mu50TkMu50_EfficienciesAndSF_Period3.root");
  TFile* f_mu50tkmu50_pog_3 = TFile::Open("Trigger_Mu50TkMu50_EfficienciesAndSF_Period3.root");
  TFile* f_mu50tkmu50_pog_4 = TFile::Open("Trigger_Mu50TkMu50_EfficienciesAndSF_Period4.root");

 
  fout->cd();
  TH2F* h_trg_mu50tkmu50_sf_1 = (TH2F*)f_mu50tkmu50_pog_1->Get("Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio");
  TH2F* h_trg_mu50tkmu50_sf_2 = (TH2F*)f_mu50tkmu50_pog_2->Get("Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio");
  TH2F* h_trg_mu50tkmu50_sf_3 = (TH2F*)f_mu50tkmu50_pog_3->Get("Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio");
  TH2F* h_trg_mu50tkmu50_sf_4 = (TH2F*)f_mu50tkmu50_pog_4->Get("Mu50_OR_TkMu50_PtEtaBins/abseta_pt_ratio");

  TH2F* h_trg_mu50_sf_1 = (TH2F*)f_mu50_own_1->Get("scalefactor");
  TH2F* h_trg_mu50_sf_2 = (TH2F*)f_mu50_own_2->Get("scalefactor");
  TH2F* h_trg_mu50_sf_3 = (TH2F*)f_mu50_own_3->Get("scalefactor");
  TH2F* h_trg_mu50_sf_4 = (TH2F*)f_mu50_own_4->Get("scalefactor");

  h_trg_mu50tkmu50_sf_1->Write("h_trg_mu50tkmu50_sf_1");
  h_trg_mu50tkmu50_sf_2->Write("h_trg_mu50tkmu50_sf_2");
  h_trg_mu50tkmu50_sf_3->Write("h_trg_mu50tkmu50_sf_3");
  h_trg_mu50tkmu50_sf_4->Write("h_trg_mu50tkmu50_sf_4");

  h_trg_mu50_sf_1->Write("h_trg_mu50_sf_1");
  h_trg_mu50_sf_2->Write("h_trg_mu50_sf_2");
  h_trg_mu50_sf_3->Write("h_trg_mu50_sf_3");
  h_trg_mu50_sf_4->Write("h_trg_mu50_sf_4");

  fout->Close();


}
