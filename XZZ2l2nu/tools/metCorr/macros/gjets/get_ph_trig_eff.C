{

  gROOT->ProcessLine(".x tdrstyle.C");

  std::string outtag="get_ph_trig_eff_fullv2";
  TFile* file1 = TFile::Open("/data2/XZZ2/80X_20170202_GJets/SinglePhoton_Run2016Full_03Feb2017_v0/vvTreeProducer/tree.root");

  std::string lumiTag;
  TPaveText* lumipt;

  lumiTag = "CMS 13 TeV 2016 L=35.87 fb^{-1}";

  lumipt = new TPaveText(0.2,0.8,0.8,0.88,"brNDC");
  lumipt->SetBorderSize(0);
  lumipt->SetTextAlign(12);
  lumipt->SetFillStyle(0);
  lumipt->SetTextFont(42);
  lumipt->SetTextSize(0.03);
  lumipt->AddText(0.15,0.3, lumiTag.c_str());
 
  char name[1000];
  sprintf(name, "%s.root", outtag.c_str());
  TFile* fout = TFile::Open(name, "recreate");
  TTree* tree1 = (TTree*)file1->Get("tree");



  TCanvas* plots = new TCanvas("plots", "plots");

  sprintf(name, "%s.pdf[", outtag.c_str());
  plots->Print(name);

  
  Double_t ZPtBins[] = {0,30,36,50,75,90,120,165,3000};
  Int_t NZPtBins = sizeof(ZPtBins)/sizeof(ZPtBins[0]) - 1;
  Double_t EtaBins[] = {0,0.5,0.7,0.9,1.1,1.3,1.5,1.7,1.9,2.1,2.3,2.5};

  Int_t NEtaBins = sizeof(EtaBins)/sizeof(EtaBins[0]) - 1; 

  TH2D* h_eta_pt_1 = new TH2D("h_eta_pt_1", "h_eta_pt_1", NZPtBins, ZPtBins, NEtaBins, EtaBins);
  TH2D* h_eta_pt_2 = new TH2D("h_eta_pt_2", "h_eta_pt_2", NZPtBins, ZPtBins, NEtaBins, EtaBins);

  h_eta_pt_1->Sumw2();
  h_eta_pt_2->Sumw2();

  tree1->Draw("fabs(gjet_l1_eta):gjet_l1_pt>>h_eta_pt_1");
  tree1->Draw("fabs(gjet_l1_eta):gjet_l1_pt>>h_eta_pt_2", "(gjet_l1_trigerob_HLTbit>>0&1&&gjet_l1_trigerob_pt<=30)||(gjet_l1_trigerob_HLTbit>>1&1&&gjet_l1_trigerob_pt<=36)||(gjet_l1_trigerob_HLTbit>>2&1&&gjet_l1_trigerob_pt<=50)||(gjet_l1_trigerob_HLTbit>>3&1&&gjet_l1_trigerob_pt<=75)||(gjet_l1_trigerob_HLTbit>>4&1&&gjet_l1_trigerob_pt<=90)||(gjet_l1_trigerob_HLTbit>>5&1&&gjet_l1_trigerob_pt<=120)||(gjet_l1_trigerob_HLTbit>>6&1&&gjet_l1_trigerob_pt<=165)||(gjet_l1_trigerob_HLTbit>>7&1&&gjet_l1_trigerob_pt<=10000000)");

  TH2D* h_eta_pt_r21 = (TH2D*)h_eta_pt_2->Clone("h_eta_pt_r21");
  h_eta_pt_r21->Divide(h_eta_pt_1);

  sprintf(name, "%s.pdf", outtag.c_str());
  plots->Clear();
  
  h_eta_pt_1->Draw("colz text");
  plots->Print(name);
  plots->Clear();

  h_eta_pt_2->Draw("colz text");
  plots->Print(name);
  plots->Clear();

  h_eta_pt_r21->Draw("colz text");
  plots->Print(name);
  plots->Clear();

  //
  TH1D* h_pt_1_sb = new TH1D("h_pt_1_sb", "h_pt_1_sb", 60, 0, 3000);
  TH1D* h_pt_2_sb = new TH1D("h_pt_2_sb", "h_pt_2_sb", 60, 0, 3000);
  h_pt_1_sb->Sumw2();
  h_pt_2_sb->Sumw2();

  tree1->Draw("gjet_l1_pt>>h_pt_1_sb");
  tree1->Draw("gjet_l1_pt>>h_pt_2_sb","(gjet_l1_trigerob_HLTbit>>0&1&&gjet_l1_trigerob_pt<=30)||(gjet_l1_trigerob_HLTbit>>1&1&&gjet_l1_trigerob_pt<=36)||(gjet_l1_trigerob_HLTbit>>2&1&&gjet_l1_trigerob_pt<=50)||(gjet_l1_trigerob_HLTbit>>3&1&&gjet_l1_trigerob_pt<=75)||(gjet_l1_trigerob_HLTbit>>4&1&&gjet_l1_trigerob_pt<=90)||(gjet_l1_trigerob_HLTbit>>5&1&&gjet_l1_trigerob_pt<=120)||(gjet_l1_trigerob_HLTbit>>6&1&&gjet_l1_trigerob_pt<=165)||(gjet_l1_trigerob_HLTbit>>7&1&&gjet_l1_trigerob_pt<=10000000)");

  TH1D* h_pt_r21_sb = (TH1D*)h_pt_2_sb->Clone("h_pt_r21_sb");
  h_pt_r21_sb->Divide(h_pt_1_sb);

  h_pt_1_sb->SetLineColor(2);
  h_pt_2_sb->SetLineColor(4);
  h_pt_1_sb->SetMarkerColor(2);
  h_pt_2_sb->SetMarkerColor(4);
 
  h_pt_1_sb->SetMarkerStyle(20);
  h_pt_2_sb->SetMarkerStyle(20);
 
  h_pt_r21_sb->SetMarkerStyle(20);

  plots->Clear();
  h_pt_1_sb->Draw();
  h_pt_2_sb->Draw("same");
  plots->Print(name);

  plots->Clear();
  h_pt_r21_sb->Draw();
  plots->Print(name);



  TProfile* pr_pt_1 = (TProfile*)h_eta_pt_1->ProfileX("pr_pt_1");
  TProfile* pr_pt_2 = (TProfile*)h_eta_pt_2->ProfileX("pr_pt_2");

  TH1D* h_pt_1 = (TH1D*)h_eta_pt_1->ProjectionX("h_pt_1");
  TH1D* h_pt_2 = (TH1D*)h_eta_pt_2->ProjectionX("h_pt_2");

  h_pt_1->SetTitle("h_pt_1");
  h_pt_2->SetTitle("h_pt_2");

  TH1D* h_pt_r21 = (TH1D*)h_pt_2->Clone("h_pt_r21");
  h_pt_r21->Divide(h_pt_1);

  
  h_pt_1->SetLineColor(2);
  h_pt_2->SetLineColor(4);
  h_pt_1->SetMarkerColor(2);
  h_pt_2->SetMarkerColor(4);

  h_pt_1->SetMarkerStyle(20);
  h_pt_2->SetMarkerStyle(20);

  h_pt_r21->SetMarkerStyle(20);

  plots->Clear();
  h_pt_1->Draw();
  h_pt_2->Draw("same");
  plots->Print(name);

  plots->Clear();
  h_pt_r21->Draw();
  plots->Print(name);


  TH2D* h_eta_pt_1_ptnorm = (TH2D*)h_eta_pt_1->Clone("h_eta_pt_1_ptnorm");
  TH2D* h_eta_pt_2_ptnorm = (TH2D*)h_eta_pt_2->Clone("h_eta_pt_2_ptnorm");

  h_eta_pt_1_ptnorm->SetTitle("h_eta_pt_1_ptnorm");
  h_eta_pt_2_ptnorm->SetTitle("h_eta_pt_2_ptnorm");

  for (int ipt=0; ipt<NZPtBins; ipt++){
    double npt1 = h_pt_1->GetBinContent(ipt+1);
    double npt2 = h_pt_2->GetBinContent(ipt+1);
 
    for (int ieta=0; ieta<NEtaBins; ieta++){
      h_eta_pt_1_ptnorm->SetBinContent(ipt+1, ieta+1, h_eta_pt_1_ptnorm->GetBinContent(ipt+1, ieta+1)/npt1);
      h_eta_pt_1_ptnorm->SetBinError(ipt+1, ieta+1, h_eta_pt_1_ptnorm->GetBinError(ipt+1, ieta+1)/npt1);
      h_eta_pt_2_ptnorm->SetBinContent(ipt+1, ieta+1, h_eta_pt_2_ptnorm->GetBinContent(ipt+1, ieta+1)/npt2);
      h_eta_pt_2_ptnorm->SetBinError(ipt+1, ieta+1, h_eta_pt_2_ptnorm->GetBinError(ipt+1, ieta+1)/npt2);
    }

  }

  h_eta_pt_weight = (TH2D*)h_eta_pt_1_ptnorm->Clone("h_eta_pt_weight");
  h_eta_pt_weight->Divide(h_eta_pt_2_ptnorm);
  h_eta_pt_weight->SetTitle("h_eta_pt_weight");

  
  plots->Clear();
  h_eta_pt_1_ptnorm->Draw("colz text");
  plots->Print(name);

  plots->Clear();
  h_eta_pt_2_ptnorm->Draw("colz text");
  plots->Print(name);
 
  plots->Clear();
  h_eta_pt_weight->Draw("colz text");
  plots->Print(name);


  sprintf(name, "%s.pdf]", outtag.c_str());
  plots->Print(name);
  // save

  fout->cd();

  h_eta_pt_1->Write();
  h_eta_pt_2->Write();
  h_eta_pt_r21->Write();

  pr_pt_1->Write();
  pr_pt_2->Write();

  h_pt_1->Write();
  h_pt_2->Write();
  
  h_eta_pt_1_ptnorm->Write();
  h_eta_pt_2_ptnorm->Write();

  h_eta_pt_weight->Write();

  h_pt_1_sb->Write();
  h_pt_2_sb->Write();
  h_pt_r21_sb->Write();

  fout->Close();

}
