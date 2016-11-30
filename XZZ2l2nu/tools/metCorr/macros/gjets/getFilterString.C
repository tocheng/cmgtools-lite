{

  std::string option="eb";
  std::string tag="_more1";
  std::string foutname="getFileterString_"+option+tag;



  TFile *_file0 = TFile::Open("/home/heli/XZZ/80X_20161029_light/SinglePhoton_Run2016B2H_ReReco_36p1fbinv/vvTreeProducer/tree.root");
  //TFile *_file0 = TFile::Open("/home/heli/XZZ/80X_20160927/SinglePhoton_Run2016D_PromptReco_v2/vvTreeProducer/tree.root");
  //TFile *_file0 = TFile::Open("/home/heli/XZZ/80X_20160927/SinglePhoton_Run2016BCD_PromptReco/vvTreeProducer/tree.root");
  //gROOT->ProcessLine(".L printFilter.C");
  TTree* tree = (TTree*)_file0->Get("tree");
  tree->SetAlias("absDeltaPhi", "fabs(TVector2::Phi_mpi_pi(gjet_l2_phi-gjet_l1_phi))");
  tree->SetAlias("metPara", "gjet_l2_pt*cos(gjet_l2_phi-gjet_l1_phi)");
  tree->SetAlias("metPerp", "gjet_l2_pt*sin(gjet_l2_phi-gjet_l1_phi)");
  tree->SetAlias("uPara", "-gjet_l2_pt*cos(gjet_l2_phi-gjet_l1_phi)-gjet_l1_pt");
  tree->SetAlias("uPerp", "-gjet_l2_pt*sin(gjet_l2_phi-gjet_l1_phi)");
  tree->SetAlias("ut", "sqrt(uPara*uPara+uPerp*uPerp)");
  tree->SetAlias("eta", "gjet_l1_eta");
  tree->SetAlias("phi", "gjet_l1_phi");
  tree->SetAlias("pt", "gjet_l1_pt");
  tree->SetAlias("metfilter", "(Flag_EcalDeadCellTriggerPrimitiveFilter&&Flag_HBHENoiseIsoFilter&&Flag_goodVertices&&Flag_HBHENoiseFilter&&Flag_globalTightHalo2016Filter&&Flag_eeBadScFilter&&Flag_CSCTightHalo2015Filter)");

  tree->SetAlias("ieta", "gjet_l1_ieta");
  tree->SetAlias("iphi", "gjet_l1_iphi");

  tree->SetAlias("flag3", "((pt>=60&&!(eta>-2.5&&eta<-1.4&&phi>2.4&&phi<3.2)&&!(eta>-2.5&&eta<-1.4&&phi>-0.9&&phi<0.9)&&!(eta>-2.5&&eta<-1.4&&phi>-3.2&&phi<-2.4)&&!(eta>1.4&&eta<2.5&&phi>2.4&&phi<3.2)&&!(eta>1.4&&eta<2.5&&phi>-0.9&&phi<0.9)&&!(eta>1.4&&eta<2.5&&phi>-3.2&&phi<-2.0)&&!(eta>-2.1&&eta<-1.8&&phi>1.2&&phi<1.6)&&!(eta>-2.0&&eta<-1.6&&phi>-2.1&&phi<-1.8)&&!(eta>-0.3&&eta<0.0&&phi>2.5&&phi<3.0)&&!(eta>0.0&&eta<0.3&&phi>2.8&&phi<3.2)&&!(eta>-0.2&&eta<0.3&&phi>0.2&&phi<0.8)&&!(eta>-0.5&&eta<-0.2&&phi>-2.4&&phi<2.0)&&!(eta>2.3&&eta<2.5&&phi>-1.5&&phi<-1.0))||(pt<60&&!(eta>-2.5&&eta<-2.2&&phi>-3.0&&phi<-2.6)&&!(eta>0&&eta<0.3&&phi>-2.4&&phi<-1.4)&&!(eta>-0.2&&eta<0.2&&phi>-3.2&&phi<-2.6)&&!(eta>2.3&&eta<2.5&&phi>-2.6&&phi<-2.0)))");

  //tree->Draw("gjet_l1_iphi:gjet_l1_ieta>>h077(181,-90.5,90.5,360,0.5,360.5)", "HLT_PHOTONIDISO&&metfilter&&fabs(eta)<1.47&&ngjet==1&&ut<150&&metPara<-60&&pt>60", "colz");
  // ee+ iphi:ieta>>h006(100,0.5,100.5,100,0.5,100.5)
  //tree->Draw("gjet_l1_iphi:gjet_l1_ieta>>h077(100,0.5,100.5,100,0.5,100.5)", "HLT_PHOTONIDISO&&metfilter&&eta>1.562&&ngjet==1&&flag3&&ut<150&&metPara<-60&&pt>60", "colz");
  //tree->Draw("gjet_l1_iphi:gjet_l1_ieta>>h077(100,0.5,100.5,100,0.5,100.5)", "HLT_PHOTONIDISO&&metfilter&&eta<-1.562&&ngjet==1&&flag3&&ut<150&&metPara<-60&&pt>60", "colz");


  sprintf(name, "%s.root", foutname.c_str());
  TFile* fout = TFile::Open(name, "recreate");

  TCanvas* plots = new TCanvas("plots", "plots");

  sprintf(name, "%s.pdf[", foutname.c_str());
  plots->Print(name);

  // ee+
  if (option=="eep") tree->Draw("gjet_l1_iphi:gjet_l1_ieta>>hist(100,0.5,100.5,100,0.5,100.5)", "eta>1.566&&fabs(uPara)<100", "colz");
  // ee-
  else if (option=="eem") tree->Draw("gjet_l1_iphi:gjet_l1_ieta>>hist(100,0.5,100.5,100,0.5,100.5)", "eta<-1.566&&fabs(uPara)<100", "colz");
  // eb
  else if (option=="eb") tree->Draw("gjet_l1_iphi:gjet_l1_ieta>>hist(181,-90.5,90.5,360,0.5,360.5)", "fabs(eta)<1.47&&fabs(uPara)<100", "colz");
  else std::cout << "wrong option do nothing" << std::endl;

  TH2D* hist = (TH2D*)gDirectory->Get("hist");

  plots->Clear();
  hist->Draw("colz");
  sprintf(name, "%s.pdf[", foutname.c_str());
  plots->Print(name);
  plots->Clear();

  hist->Write("hist_map_before");

  char name[1000];
  std::string selec ;
  //selec = "HLT_PHOTONIDISO&&metfilter&&fabs(eta)<1.47&&ngjet==1";
  //selec = "HLT_PHOTONIDISO&&metfilter&&eta>1.562&&ngjet==1&&flag3";
  //selec = "HLT_PHOTONIDISO&&metfilter&&eta<-1.562&&ngjet==1&&flag3";
  if (option=="eep") selec = "eta>1.566";
  else if (option=="eem") selec = "eta<-1.566";
  else if (option=="eb") selec = "fabs(eta)<1.47";
  else std::cout << "wrong option do nothing" << std::endl;

  float utcut=0.5;
  Int_t n_loop=50;

  Int_t cx;   Int_t cy;   Int_t bc;
  Int_t bx;   Int_t by; Int_t bz;
  Double_t n1; Double_t n2;

  std::string flag1 = "(";
  std::cout << "(";
  for (int i=0;i<n_loop; i++) {
    hist->GetMaximumBin(bx,by,bz);
    cx = (Int_t)hist->GetXaxis()->GetBinCenter(bx);
    cy = (Int_t)hist->GetYaxis()->GetBinCenter(by);
    bc = (Int_t)hist->GetBinContent(bx,by);
    std::cout << "(ieta==" << cx << "&&iphi==" << cy << ")||";
    std::cout  << "    # bc="<<bc ;
    std::cout << std::endl;

    sprintf(name, "(ieta==%d&&iphi==%d)", cx, cy);

    if (i==n_loop-1) flag1 += std::string(name);
    else flag1 += std::string(name)+"||";

    // draw the xtal
    sprintf(name, "(ieta==%d&&iphi==%d)", cx, cy);
    std::string xtal_selec = selec+"&&"+std::string(name);

    plots->Clear();
    tree->Draw("metPara/pt>>hist1(60,-3,3)", xtal_selec.c_str(), "e");
    TH1D* hist1 = (TH1D*)gDirectory->Get("hist1");
    sprintf(name, "hmetparavpt_%i", i);
    hist1->SetName(name);
    sprintf(name, "(ieta==%d&&iphi==%d)", cx, cy);
    hist1->SetTitle(name);
    sprintf(name, "%s.pdf", foutname.c_str());
    plots->Print(name);
    hist1->Write();
    plots->Clear();




    hist->SetBinContent(bx,by,0);
  }; 
  
  hist->Write("hist_map_after");

  flag1 += ")";

  std::cout << flag1 << std::endl;

  std::string selec_flag1 = selec+"&&"+flag1;
  std::string selec_notflag1 = selec+"&&!"+flag1;

  tree->Draw("metPara/pt>>h_metParaVpT_flag1(1000,-10,10)", selec_flag1.c_str(), "e");
  tree->Draw("metPara/pt>>h_metParaVpT_Nflag1(1000,-10,10)", selec_notflag1.c_str(), "e");

  TH1D* h_flg1 = (TH1D*)gDirectory->Get("h_metParaVpT_flag1");
  TH1D* h_nflg1 = (TH1D*)gDirectory->Get("h_metParaVpT_Nflag1");

  h_flg1->Scale(1./h_flg1->Integral());
  h_nflg1->Scale(1./h_nflg1->Integral());

  h_flg1->SetLineColor(2);
  h_nflg1->SetLineColor(4);

  plots->Clear();
  h_flg1->Draw();
  h_nflg1->Draw("same");
  sprintf(name, "%s.pdf", foutname.c_str());
  plots->Print(name);
  plots->Clear();

  h_flg1->Write();
  h_nflg1->Write();


  sprintf(name, "%s.pdf]", foutname.c_str());
  plots->Print(name);

}
