{
  gROOT->ProcessLine(".! cp study_gjets_data_b2h36p22fbinv_v5resbos_norm.root study_gjets_data_b2h36p22fbinv_v5resbos_norm_modify.root");

  TFile* file = TFile::Open("study_gjets_data_b2h36p22fbinv_v5resbos_norm_modify.root", "update");

  TH1D* hzpt = (TH1D*)file->Get("h_zpt_ratio");
  TH1D* hzpt_el = (TH1D*)file->Get("h_zpt_ratio_el");
  TH1D* hzpt_mu = (TH1D*)file->Get("h_zpt_ratio_mu");
  TH1D* hzpt_up = (TH1D*)file->Get("h_zpt_ratio_up");
  TH1D* hzpt_el_up = (TH1D*)file->Get("h_zpt_ratio_el_up");
  TH1D* hzpt_mu_up = (TH1D*)file->Get("h_zpt_ratio_mu_up");
  TH1D* hzpt_dn = (TH1D*)file->Get("h_zpt_ratio_dn");
  TH1D* hzpt_el_dn = (TH1D*)file->Get("h_zpt_ratio_el_dn");
  TH1D* hzpt_mu_dn = (TH1D*)file->Get("h_zpt_ratio_mu_dn");


  TH1D* hzpt_lowlpt = (TH1D*)file->Get("h_zpt_lowlpt_ratio");
  TH1D* hzpt_lowlpt_el = (TH1D*)file->Get("h_zpt_lowlpt_ratio_el");
  TH1D* hzpt_lowlpt_mu = (TH1D*)file->Get("h_zpt_lowlpt_ratio_mu");
 
  // bin, scale
  int b;
  double s;

  // hzpt_el
  b=60; s=0.75;
  hzpt_el->SetBinContent(b,hzpt_el->GetBinContent(b)*s);
  hzpt_el_up->SetBinContent(b,hzpt_el_up->GetBinContent(b)*s);
  hzpt_el_dn->SetBinContent(b,hzpt_el_dn->GetBinContent(b)*s);
  b=61; s=0.85;
  hzpt_el->SetBinContent(b,hzpt_el->GetBinContent(b)*s);
  hzpt_el_up->SetBinContent(b,hzpt_el_up->GetBinContent(b)*s);
  hzpt_el_dn->SetBinContent(b,hzpt_el_dn->GetBinContent(b)*s);
  b=59; s=1.05;
  hzpt_el->SetBinContent(b,hzpt_el->GetBinContent(b)*s);
  hzpt_el_up->SetBinContent(b,hzpt_el_up->GetBinContent(b)*s);
  hzpt_el_dn->SetBinContent(b,hzpt_el_dn->GetBinContent(b)*s);
  b=56; s=1.03;
  hzpt_el->SetBinContent(b,hzpt_el->GetBinContent(b)*s);
  hzpt_el_up->SetBinContent(b,hzpt_el_up->GetBinContent(b)*s);
  hzpt_el_dn->SetBinContent(b,hzpt_el_dn->GetBinContent(b)*s);
  b=55; s=1.03;
  hzpt_el->SetBinContent(b,hzpt_el->GetBinContent(b)*s);
  hzpt_el_up->SetBinContent(b,hzpt_el_up->GetBinContent(b)*s);
  hzpt_el_dn->SetBinContent(b,hzpt_el_dn->GetBinContent(b)*s);
  b=54; s=1.03;
  hzpt_el->SetBinContent(b,hzpt_el->GetBinContent(b)*s);
  hzpt_el_up->SetBinContent(b,hzpt_el_up->GetBinContent(b)*s);
  hzpt_el_dn->SetBinContent(b,hzpt_el_dn->GetBinContent(b)*s);
  b=52; s=0.9;
  hzpt_el->SetBinContent(b,hzpt_el->GetBinContent(b)*s);
  hzpt_el_up->SetBinContent(b,hzpt_el_up->GetBinContent(b)*s);
  hzpt_el_dn->SetBinContent(b,hzpt_el_dn->GetBinContent(b)*s);
  b=51; s=0.9;
  hzpt_el->SetBinContent(b,hzpt_el->GetBinContent(b)*s);
  hzpt_el_up->SetBinContent(b,hzpt_el_up->GetBinContent(b)*s);
  hzpt_el_dn->SetBinContent(b,hzpt_el_dn->GetBinContent(b)*s);
  b=50; s=0.35;
  hzpt_el->SetBinContent(b,hzpt_el->GetBinContent(b)*s);
  hzpt_el_up->SetBinContent(b,hzpt_el_up->GetBinContent(b)*s);
  hzpt_el_dn->SetBinContent(b,hzpt_el_dn->GetBinContent(b)*s);
  b=49; s=0.35;
  hzpt_el->SetBinContent(b,hzpt_el->GetBinContent(b)*s);
  hzpt_el_up->SetBinContent(b,hzpt_el_up->GetBinContent(b)*s);
  hzpt_el_dn->SetBinContent(b,hzpt_el_dn->GetBinContent(b)*s);


  // hzpt_mu
  b=61; s=0.71; 
  hzpt_mu->SetBinContent(b,hzpt_mu->GetBinContent(b)*s);
  hzpt_mu_up->SetBinContent(b,hzpt_mu_up->GetBinContent(b)*s);
  hzpt_mu_dn->SetBinContent(b,hzpt_mu_dn->GetBinContent(b)*s);
  b=60; s=0.96; 
  hzpt_mu->SetBinContent(b,hzpt_mu->GetBinContent(b)*s);
  hzpt_mu_up->SetBinContent(b,hzpt_mu_up->GetBinContent(b)*s);
  hzpt_mu_dn->SetBinContent(b,hzpt_mu_dn->GetBinContent(b)*s);
  for (b=48; b<=58; b++) { 
    s=0.947; 
    hzpt_mu->SetBinContent(b,hzpt_mu->GetBinContent(b)*s);
    hzpt_mu_up->SetBinContent(b,hzpt_mu_up->GetBinContent(b)*s);
    hzpt_mu_dn->SetBinContent(b,hzpt_mu_dn->GetBinContent(b)*s);
  }


  file->cd();
  hzpt_el->Write("h_zpt_ratio_el");
  hzpt_el_up->Write("h_zpt_ratio_el_up");
  hzpt_el_dn->Write("h_zpt_ratio_el_dn");
  hzpt_mu->Write("h_zpt_ratio_mu");
  hzpt_mu_up->Write("h_zpt_ratio_mu_up");
  hzpt_mu_dn->Write("h_zpt_ratio_mu_dn");

  
  file->Close();  

}
