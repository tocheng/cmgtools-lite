{

gROOT->ProcessLine(".x tdrstyle.C");

//gStyle->SetErrorX(0.);
 
TFile* file=TFile::Open("ReMiniAODCRScaleMoreSig_xzz2l2nu_limit_13TeV_SRdPhiGT0p5_ggG_unblind.root");

/*
root [1] c->ls()
Canvas Name=c Title=c Option=
 TCanvas fXlowNDC=0 fYlowNDC=0 fWNDC=1 fHNDC=1 Name= c Title= c Option=
  OBJ: TList	TList	Doubly linked list : 0
   TFrame  X1= 600.000000 Y1=-3.000000 X2=2500.000000 Y2=1.000000
   OBJ: TH1D	dummy	dummy : 1 at: 0x7fcf72c63ee0
   OBJ: TGraphAsymmErrors	gr_exp2	 : 1 at: 0x7fcf72c79aa0
   OBJ: TGraphAsymmErrors	gr_exp1	 : 1 at: 0x7fcf72c842c0
   OBJ: TGraphAsymmErrors	gr_exp	 : 1 at: 0x7fcf72c84d80
   OBJ: TGraphAsymmErrors	gr_obs	 : 1 at: 0x7fcf72c85840
   OBJ: TGraphAsymmErrors	gr_exp_0p1	 : 1 at: 0x7fcf72c86300
   OBJ: TGraphAsymmErrors	gr_exp_0p2	 : 1 at: 0x7fcf72c86dc0
   OBJ: TGraphAsymmErrors	gr_exp_0p3	 : 1 at: 0x7fcf72c87880
   Text  X=0.870000 Y=0.950000 Text=35.9 fb^{-1} (13 TeV)
   Text  X=0.250000 Y=0.850000 Text=CMS
   OBJ: TLegend	legend0  	X1= 1370.270267 Y1=0.400000 X2=2397.297292 Y2=0.850000
   OBJ: TLegend	legend  	X1= 1370.270267 Y1=-0.800000 X2=2397.297292 Y2=0.350000
   OBJ: TH1D	dummy_copy	dummy : 1 at: 0x7fcf72c946f0
*/



TCanvas* c = (TCanvas*)file->Get("c");
TH1D* dummy = (TH1D*)c->FindObject("dummy");
TGraphAsymmErrors* gr_exp = (TGraphAsymmErrors*)c->FindObject("gr_exp");
TGraphAsymmErrors* gr_exp_0p1 = (TGraphAsymmErrors*)c->FindObject("gr_exp_0p1");
TGraphAsymmErrors* gr_exp_0p2 = (TGraphAsymmErrors*)c->FindObject("gr_exp_0p2");
TGraphAsymmErrors* gr_exp_0p3 = (TGraphAsymmErrors*)c->FindObject("gr_exp_0p3");
TLegend* legend0 = (TLegend*)c->FindObject("legend0");
TLegend* legend = (TLegend*)c->FindObject("legend");




c->Draw();

c->SetGridx(0);
c->SetGridy(0);

dummy->GetYaxis()->SetLabelSize(0.05);
dummy->GetXaxis()->SetNdivisions(505,kTRUE);
dummy->GetXaxis()->SetLabelSize(0.05);
dummy->GetYaxis()->SetTitle("#sigma(pp#rightarrowX#rightarrowZZ) (pb)");



gr_exp->SetLineStyle(9);
gr_exp->SetFillColor(kBlack);
gr_exp_0p1->SetLineStyle(10);
gr_exp_0p1->SetFillColor(kRed);
gr_exp_0p2->SetLineStyle(7);
gr_exp_0p2->SetFillColor(kBlue);
gr_exp_0p3->SetLineStyle(3);
gr_exp_0p3->SetFillColor(kViolet);


((TLegendEntry*)legend0->GetListOfPrimitives()->At(0))->SetLabel("95% CL upper limits: ggX");

((TLegendEntry*)legend->GetListOfPrimitives()->At(2))->SetLabel("width = 0.1 m_{X}");
((TLegendEntry*)legend->GetListOfPrimitives()->At(3))->SetLabel("width = 0.2 m_{X}");
((TLegendEntry*)legend->GetListOfPrimitives()->At(4))->SetLabel("width = 0.3 m_{X}");

//legend0->SetY1NDC(0.48);
legend0->SetX1NDC(0.43);
legend0->SetX2NDC(0.92);
legend->SetY1NDC(0.52);
legend->SetX1NDC(0.43);
legend->SetX2NDC(0.92);


c->GetListOfPrimitives()->RemoveAt(9);
TPaveText* pave = new TPaveText(0.7,0.91,0.95,0.99, "NDC");
pave->SetTextFont(42);
pave->AddText("35.9 fb^{-1} (13 TeV)");
pave->SetFillColor(0);
pave->SetFillStyle(0);
pave->SetBorderSize(0);
pave->Draw("same"); 

c->SaveAs("updated_Limits_ggG.pdf");


}

