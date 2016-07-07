

std::string filename =
  "skim2/DYJetsToLL_M50_V4_doJetsCorrUseLepResPtErrSel8JetLepSigProtectV2"
  //"skim2/SingleEMU_Run2015_16Dec_V4_doJetsCorrUseLepResPtErrSel8JetLepSigProtectV2"
  //"skim2/SingleEMU_Run2015_16Dec_V4_doJetsCorrUseLepResPtErrSel8JetLepSigProtect"
  //"skim2/DYJetsToLL_M50_V4_doJetsCorrUseLepResPtErrSel8JetLepSigProtect"
   //"skim2/DYJetsToLL_M50_V4_doJetsCorrUseLepResPtErrSel8JetLepHardOnlyZpT250"
  ;
std::string tag = 
  "_met_para_study";

std::string lumiTag = 
  "CMS 13 TeV 2015 L=2.32 fb^{-1}"
  //"CMS 13 TeV Simulation for 2015 Data"
  ;

char name[1000];
std::string histname;
TFile* fin;
TFile* fout;
TPaveText* lumipt;
TCanvas* plots;
TH2D* h2d1;
TH2D* h2d2;
TH2D* h2d3;
TH2D* h2d4;
TH1D* h1d1[1000];
TH1D* h1d2[1000];
TH1D* h1d3[1000];
TH1D* h1d4[1000];
TF1* func1[1000];
TF1* func2[1000];
TF1* func3[1000];
TF1* func4[1000];

Int_t Nbins;
std::vector<Double_t> fit_min; 
std::vector<Double_t> fit_max; 
std::vector<Int_t> fit_rebin; 

int fit_slice_gaus(TH2D* h2d, TH1D** h1d);

void fit_met_para(){

  gROOT->ProcessLine(".x tdrstyle.C");

  lumipt = new TPaveText(0.45,0.9,0.9,0.98,"brNDC");
  lumipt->SetBorderSize(0);
  lumipt->SetTextAlign(12);
  lumipt->SetFillStyle(0);
  lumipt->SetTextFont(42);
  lumipt->SetTextSize(0.03);
  lumipt->AddText(0.15,0.3, lumiTag.c_str());

  sprintf(name, "%s.root", filename.c_str());
  fin = new TFile(name);

  sprintf(name, "%s%s.root", filename.c_str(), tag.c_str());
  fout = new TFile(name, "recreate");

  plots = new TCanvas("plots", "plots");

  sprintf(name, "%s%s.ps[", filename.c_str(), tag.c_str());
  plots->Print(name);
  
  histname="h_met_para_vs_zpt_new";
  h2d1 = (TH2D*)fin->Get(histname.c_str());
  h2d1->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
  h2d1->GetYaxis()->SetTitle("MET para (GeV)");

  Nbins = h2d1->GetNbinsX();
  fit_min = { -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -25  };
  fit_max = { +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20  };
  fit_rebin = {  4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4  };


  histname="h_met_para_vs_zpt_old";
  h2d2 = (TH2D*)fin->Get(histname.c_str());
  h2d2->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
  h2d2->GetYaxis()->SetTitle("MET para (GeV)");

  fit_min = { -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -20, -30, -30, -30, -30  };
  fit_max = { +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +20, +30, +30, +30, +30  };

  histname="h_met_perp_vs_zpt_new";
  h2d3 = (TH2D*)fin->Get(histname.c_str());
  h2d3->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
  h2d3->GetYaxis()->SetTitle("MET perp (GeV)");

  histname="h_met_perp_vs_zpt_old";
  h2d4 = (TH2D*)fin->Get(histname.c_str());
  h2d4->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
  h2d4->GetYaxis()->SetTitle("MET perp (GeV)");

  fit_slice_gaus(h2d1, h1d1);
  fit_slice_gaus(h2d2, h1d2);
  fit_slice_gaus(h2d3, h1d3);
  fit_slice_gaus(h2d4, h1d4);



  sprintf(name, "%s%s.ps]", filename.c_str(), tag.c_str());
  plots->Print(name);


  sprintf(name, ".! ps2pdf %s%s.ps %s%s.pdf", filename.c_str(), tag.c_str(), filename.c_str(), tag.c_str());
  gROOT->ProcessLine(name);

  fout->Close();
}

int fit_slice_gaus(TH2D* h2d, TH1D** h1d){ 

  std::string hname = h2d->GetName();
  Double_t* xbins = (Double_t*)h2d->GetXaxis()->GetXbins()->GetArray();

  plots->cd();
  plots->Clear();
  plots->SetLogx(1);
  h2d->Draw("colz");
  lumipt->Draw();
  sprintf(name, "%s%s.ps", filename.c_str(), tag.c_str());
  plots->Print(name);
  plots->SetLogx(0);
  plots->Clear();
  
  fout->cd();
  h2d->Write();

  sprintf(name, "%s_mean", hname.c_str());
  TH1D* h_mean = new TH1D(name, name, Nbins, xbins);
  h_mean->Sumw2();
  h_mean->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
  h_mean->GetYaxis()->SetTitle("mean (GeV)");
  h_mean->SetLineColor(2);
  h_mean->SetMarkerColor(2);
  h_mean->SetMarkerStyle(20);
  sprintf(name, "%s_sigma", hname.c_str());
  TH1D* h_sigma = new TH1D(name, name, Nbins, xbins);
  h_sigma->Sumw2();
  h_sigma->GetXaxis()->SetTitle("P_{T}(Z) (GeV)");
  h_sigma->GetYaxis()->SetTitle("sigma (GeV)");
  h_sigma->SetLineColor(4);
  h_sigma->SetMarkerColor(4);
  h_sigma->SetMarkerStyle(20);

  for (int i=0; i<Nbins; i++){
    sprintf(name, "%s_bin%d_func", hname.c_str(), i+1);
    TF1* afunc = new TF1(name, "gaus", -100,+100);
    sprintf(name, "%s_bin%d", hname.c_str(), i+1);
    TH1D* ahist = (TH1D*)h2d->ProjectionY(name, i+1, i+1, "e");
    ahist->SetTitle(name);
    ahist->Rebin(fit_rebin[i]);
    ahist->Fit(afunc, "R", "", fit_min[i], fit_max[i]);
    double mean = afunc->GetParameter(1);
    double sigma = afunc->GetParameter(2);
    ahist->Fit(afunc, "R", "", mean-1.5*sigma, mean+1.5*sigma); 
    func1[i] = afunc;
    h1d1[i] = ahist;

    h_mean->SetBinContent(i+1, afunc->GetParameter(1));
    h_mean->SetBinError(i+1, afunc->GetParError(1));
    h_sigma->SetBinContent(i+1, afunc->GetParameter(2));
    h_sigma->SetBinError(i+1, afunc->GetParError(2));
    

    plots->cd();
    plots->Clear();
    ahist->Draw();
    lumipt->Draw();
    sprintf(name, "%s%s.ps", filename.c_str(), tag.c_str());
    plots->Print(name);    
    plots->Clear();    
   
    fout->cd();
    ahist->Write();
    afunc->Write();
  }


  plots->cd();
  plots->Clear();
  plots->SetLogx(1);
  h_mean->Draw();
  lumipt->Draw();
  sprintf(name, "%s%s.ps", filename.c_str(), tag.c_str());
  plots->Print(name);
  plots->SetLogx(0);
  plots->Clear();

  plots->cd();
  plots->Clear();
  plots->SetLogx(1);
  h_sigma->Draw();
  lumipt->Draw();
  sprintf(name, "%s%s.ps", filename.c_str(), tag.c_str());
  plots->Print(name);
  plots->SetLogx(0);
  plots->Clear();

  fout->cd();
  h_mean->Write();
  h_sigma->Write();


  return 0;

}
