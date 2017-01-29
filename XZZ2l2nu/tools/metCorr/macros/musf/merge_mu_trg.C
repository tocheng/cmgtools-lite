
{

  TFile* fout = TFile::Open("muon_trg_summer16.root", "recreate");
 
  TFile* f_mu50_own_1= TFile::Open("hlt50muon_2016fulleff_absetapt.root");
  TFile* f_mu50_own_2= TFile::Open("hlt50muon_2016fulleff_absetapt.root");
  TFile* f_mu50_own_3= TFile::Open("hlt50muon_2016fulleff_absetapt.root");
  TFile* f_mu50_own_4= TFile::Open("hlt50muon_2016fulleff_absetapt.root");

  TFile* f_mu50tkmu50_own_1 = TFile::Open("hlt50muonortk_2016period1and2_absetapt.root");
  TFile* f_mu50tkmu50_own_2 = TFile::Open("hlt50muonortk_2016period1and2_absetapt.root");

  TFile* f_mu50tkmu50_pog_1 = TFile::Open("Trigger_Mu50TkMu50_EfficienciesAndSF_Period3.root");
  TFile* f_mu50tkmu50_pog_2 = TFile::Open("Trigger_Mu50TkMu50_EfficienciesAndSF_Period3.root");
  TFile* f_mu50tkmu50_pog_3 = TFile::Open("Trigger_Mu50TkMu50_EfficienciesAndSF_Period3.root");
  TFile* f_mu50tkmu50_pog_4 = TFile::Open("Trigger_Mu50TkMu50_EfficienciesAndSF_Period4.root");

 
  fout->cd();

  TH2F* h_eff_trg_mu50tkmu50_dt_1 = (TH2F*)f_mu50tkmu50_own_1->Get("efficiency_dt");
  TH2F* h_eff_trg_mu50tkmu50_dt_2 = (TH2F*)f_mu50tkmu50_own_2->Get("efficiency_dt");
  TH2F* h_eff_trg_mu50tkmu50_mc_1 = (TH2F*)f_mu50tkmu50_own_1->Get("efficiency_mc");
  TH2F* h_eff_trg_mu50tkmu50_mc_2 = (TH2F*)f_mu50tkmu50_own_2->Get("efficiency_mc");

  //TH2F* h_eff_trg_mu50tkmu50_dt_1 = (TH2F*)f_mu50tkmu50_pog_1->Get("Mu50_OR_TkMu50_PtEtaBins/efficienciesDATA/abseta_pt_DATA");
  //TH2F* h_eff_trg_mu50tkmu50_dt_2 = (TH2F*)f_mu50tkmu50_pog_2->Get("Mu50_OR_TkMu50_PtEtaBins/efficienciesDATA/abseta_pt_DATA");
  TH2F* h_eff_trg_mu50tkmu50_dt_3 = (TH2F*)f_mu50tkmu50_pog_3->Get("Mu50_OR_TkMu50_PtEtaBins/efficienciesDATA/abseta_pt_DATA");
  TH2F* h_eff_trg_mu50tkmu50_dt_4 = (TH2F*)f_mu50tkmu50_pog_4->Get("Mu50_OR_TkMu50_PtEtaBins/efficienciesDATA/abseta_pt_DATA");
  //TH2F* h_eff_trg_mu50tkmu50_mc_1 = (TH2F*)f_mu50tkmu50_pog_1->Get("Mu50_OR_TkMu50_PtEtaBins/efficienciesMC/abseta_pt_MC");
  //TH2F* h_eff_trg_mu50tkmu50_mc_2 = (TH2F*)f_mu50tkmu50_pog_2->Get("Mu50_OR_TkMu50_PtEtaBins/efficienciesMC/abseta_pt_MC");
  TH2F* h_eff_trg_mu50tkmu50_mc_3 = (TH2F*)f_mu50tkmu50_pog_3->Get("Mu50_OR_TkMu50_PtEtaBins/efficienciesMC/abseta_pt_MC");
  TH2F* h_eff_trg_mu50tkmu50_mc_4 = (TH2F*)f_mu50tkmu50_pog_4->Get("Mu50_OR_TkMu50_PtEtaBins/efficienciesMC/abseta_pt_MC");

  TH2F* h_eff_trg_mu50_dt_1 = (TH2F*)f_mu50_own_1->Get("efficiency_dt");
  TH2F* h_eff_trg_mu50_dt_2 = (TH2F*)f_mu50_own_2->Get("efficiency_dt");
  TH2F* h_eff_trg_mu50_dt_3 = (TH2F*)f_mu50_own_3->Get("efficiency_dt");
  TH2F* h_eff_trg_mu50_dt_4 = (TH2F*)f_mu50_own_4->Get("efficiency_dt");
  TH2F* h_eff_trg_mu50_mc_1 = (TH2F*)f_mu50_own_1->Get("efficiency_mc");
  TH2F* h_eff_trg_mu50_mc_2 = (TH2F*)f_mu50_own_2->Get("efficiency_mc");
  TH2F* h_eff_trg_mu50_mc_3 = (TH2F*)f_mu50_own_3->Get("efficiency_mc");
  TH2F* h_eff_trg_mu50_mc_4 = (TH2F*)f_mu50_own_4->Get("efficiency_mc");

  h_eff_trg_mu50tkmu50_dt_1->Write("h_eff_trg_mu50tkmu50_dt_1");
  h_eff_trg_mu50tkmu50_dt_2->Write("h_eff_trg_mu50tkmu50_dt_2");
  h_eff_trg_mu50tkmu50_dt_3->Write("h_eff_trg_mu50tkmu50_dt_3");
  h_eff_trg_mu50tkmu50_dt_4->Write("h_eff_trg_mu50tkmu50_dt_4");
  h_eff_trg_mu50tkmu50_mc_1->Write("h_eff_trg_mu50tkmu50_mc_1");
  h_eff_trg_mu50tkmu50_mc_2->Write("h_eff_trg_mu50tkmu50_mc_2");
  h_eff_trg_mu50tkmu50_mc_3->Write("h_eff_trg_mu50tkmu50_mc_3");
  h_eff_trg_mu50tkmu50_mc_4->Write("h_eff_trg_mu50tkmu50_mc_4");

  h_eff_trg_mu50_dt_1->Write("h_eff_trg_mu50_dt_1");
  h_eff_trg_mu50_dt_2->Write("h_eff_trg_mu50_dt_2");
  h_eff_trg_mu50_dt_3->Write("h_eff_trg_mu50_dt_3");
  h_eff_trg_mu50_dt_4->Write("h_eff_trg_mu50_dt_4");
  h_eff_trg_mu50_mc_1->Write("h_eff_trg_mu50_mc_1");
  h_eff_trg_mu50_mc_2->Write("h_eff_trg_mu50_mc_2");
  h_eff_trg_mu50_mc_3->Write("h_eff_trg_mu50_mc_3");
  h_eff_trg_mu50_mc_4->Write("h_eff_trg_mu50_mc_4");

  fout->Close();


}
