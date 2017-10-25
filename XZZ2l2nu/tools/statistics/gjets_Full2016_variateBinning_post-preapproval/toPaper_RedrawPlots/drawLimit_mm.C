{

gROOT->ProcessLine(".x tdrstyle.C");

//gStyle->SetErrorX(0.);
 
TFile* file=TFile::Open("ReMiniAODCRScaleMoreSig_xzz2l2nu_limit_13TeV_mm_SRdPhiGT0p5_BulkGrav_narrow_unbiind.root");

/*
root [2] c4->ls()
Canvas Name=c4 Title=c4 Option=
 TCanvas fXlowNDC=0 fYlowNDC=0 fWNDC=1 fHNDC=1 Name= c4 Title= c4 Option=
  OBJ: TList	TList	Doubly linked list : 0
   TFrame  X1= 600.000000 Y1=-3.000000 X2=2500.000000 Y2=1.000000
   OBJ: TH1D	dummy	dummy : 1 at: 0x7fd41ad49c30
   OBJ: TGraphAsymmErrors	gr_exp2_mm	 : 1 at: 0x7fd41ad5f7f0
   OBJ: TGraphAsymmErrors	gr_exp1_mm	 : 1 at: 0x7fd41ad6a010
   OBJ: TGraphAsymmErrors	gr_exp	 : 1 at: 0x7fd41ad6aad0
   OBJ: TGraphAsymmErrors	gr_exp_mm	 : 1 at: 0x7fd41ad6b5a0
   OBJ: TGraphAsymmErrors	gr_obs_mm	 : 1 at: 0x7fd41ad6c060
   OBJ: TLegend	legend4  	X1= 1306.410266 Y1=-0.650000 X2=2402.564097 Y2=0.850000
   Text  X=0.870000 Y=0.950000 Text=35.9 fb^{-1} (13 TeV)
   Text  X=0.250000 Y=0.850000 Text=CMS
   OBJ: TH1D	dummy_copy	dummy : 1 at: 0x7fd41ad78ae0

*/



TCanvas* c = (TCanvas*)file->Get("c4");
TH1D* dummy = (TH1D*)c->FindObject("dummy");
TLegend* legend = (TLegend*)c->FindObject("legend4");
TGraphAsymmErrors* gr_obs = (TGraphAsymmErrors*)c->FindObject("gr_obs_mm");
TGraphAsymmErrors* gr_exp_mm = (TGraphAsymmErrors*)c->FindObject("gr_exp_mm");



c->Draw();

c->SetGridx(0);
c->SetGridy(0);

dummy->GetYaxis()->SetLabelSize(0.05);
dummy->GetXaxis()->SetNdivisions(505,kTRUE);
dummy->GetXaxis()->SetLabelSize(0.05);
dummy->GetYaxis()->SetTitle("#sigma(pp#rightarrowX#rightarrowZZ) (pb)");




((TLegendEntry*)legend->GetListOfPrimitives()->At(0))->SetLabel("95% CL upper limits");
((TLegendEntry*)legend->GetListOfPrimitives()->At(1))->SetLabel("Observed #mu#mu channel");
legend->SetY1NDC(0.55);
legend->SetX1NDC(0.42);
legend->SetX2NDC(0.93);
//legend->SetEntrySeparation(0.25);


c->GetListOfPrimitives()->RemoveAt(8);
TPaveText* pave = new TPaveText(0.7,0.91,0.95,0.99, "NDC");
pave->SetTextFont(42);
pave->AddText("35.9 fb^{-1} (13 TeV)");
pave->SetFillColor(0);
pave->SetFillStyle(0);
pave->SetBorderSize(0);
pave->Draw("same"); 


gr_obs->SetLineColor(kBlack);
gr_obs->SetMarkerColor(kBlack);
gr_exp_mm->SetLineColor(kBlack);
gr_exp_mm->SetMarkerColor(kBlack);

c->Update();
c->SaveAs("updated_Limits_mm.pdf");


}

