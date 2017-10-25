{

gROOT->ProcessLine(".x tdrstyle.C");

//gStyle->SetErrorX(0.);
 
TFile* file=TFile::Open("ReMiniAODCRScaleMoreSig_xzz2l2nu_limit_13TeV_SRdPhiGT0p5_BulkGrav_narrow_unblind.root");

/*
Canvas Name=c Title=c Option=
 TCanvas fXlowNDC=0 fYlowNDC=0 fWNDC=1 fHNDC=1 Name= c Title= c Option=
  OBJ: TList	TList	Doubly linked list : 0
   TFrame  X1= 600.000000 Y1=-3.000000 X2=2500.000000 Y2=1.000000
   OBJ: TH1D	dummy	dummy : 1 at: 0x7f999d627010
   OBJ: TGraphAsymmErrors	gr_exp2	 : 1 at: 0x7f999d63cbd0
   OBJ: TGraphAsymmErrors	gr_exp1	 : 1 at: 0x7f999d6473f0
   OBJ: TGraphAsymmErrors	gr_exp	 : 1 at: 0x7f999d647eb0
   OBJ: TGraphAsymmErrors	gr_obs	 : 1 at: 0x7f999d648970
   Text  X=0.870000 Y=0.950000 Text=35.9 fb^{-1} (13 TeV)
   Text  X=0.250000 Y=0.850000 Text=CMS
   OBJ: TGraphErrors	sigXsec_k1p0	 : 1 at: 0x7f999d64d370
   OBJ: TGraphErrors	sigXsec_k0p5	 : 1 at: 0x7f999d64f250
   OBJ: TGraphErrors	sigXsec_k0p1	 : 1 at: 0x7f999d64fc60
   OBJ: TLegend	legend  	X1= 1306.410266 Y1=-0.150000 X2=2402.564097 Y2=0.850000
   OBJ: TLegend	legend1  	X1= 1306.410266 Y1=-0.850000 X2=2402.564097 Y2=-0.250000
   OBJ: TH1D	dummy	dummy : 1 at: 0x7f999d627010
   OBJ: TH1D	dummy_copy	dummy : 1 at: 0x7f999d658bb0
*/



TCanvas* c = (TCanvas*)file->Get("c");
TH1D* dummy = (TH1D*)c->FindObject("dummy");
TGraphErrors* sigXsec_k1p0 = (TGraphErrors*)c->FindObject("sigXsec_k1p0");
TGraphErrors* sigXsec_k0p5 = (TGraphErrors*)c->FindObject("sigXsec_k0p5");
TGraphErrors* sigXsec_k0p1 = (TGraphErrors*)c->FindObject("sigXsec_k0p1");
TLegend* legend1 = (TLegend*)c->FindObject("legend1");




c->Draw();

c->SetGridx(0);
c->SetGridy(0);

dummy->GetYaxis()->SetLabelSize(0.05);
dummy->GetXaxis()->SetNdivisions(505,kTRUE);
dummy->GetXaxis()->SetLabelSize(0.05);
dummy->GetYaxis()->SetTitle("#sigma(pp#rightarrowX#rightarrowZZ) (pb)");



sigXsec_k1p0->SetLineStyle(1);
sigXsec_k1p0->SetFillStyle(1000);
sigXsec_k1p0->SetFillColor(kRed);
sigXsec_k1p0->SetFillColorAlpha(kRed, 0.3);

sigXsec_k0p5->SetLineStyle(7);
sigXsec_k0p5->SetFillStyle(1000);
sigXsec_k0p5->SetFillColor(kRed);
sigXsec_k0p5->SetFillColorAlpha(kRed, 0.3);

sigXsec_k0p1->SetLineStyle(5);
sigXsec_k0p1->SetFillStyle(1000);
sigXsec_k0p1->SetFillColor(kRed);
sigXsec_k0p1->SetFillColorAlpha(kRed, 0.3);

TGraphErrors* sigXsec_dummy = (TGraphErrors*)sigXsec_k0p1->Clone("sigXsec_dummy");
sigXsec_dummy->SetLineStyle(1);
sigXsec_dummy->SetLineWidth(0);
sigXsec_dummy->SetLineColorAlpha(kRed,0.3);

legend1->GetListOfPrimitives()->RemoveAt(4);
legend1->AddEntry(sigXsec_dummy, "PDF+scale uncertainties", "f");
((TLegendEntry*)legend1->GetListOfPrimitives()->At(0))->SetLabel("bulk G #rightarrow ZZ cross sections");
legend1->SetY1NDC(0.48);
legend1->SetX2NDC(0.95);
legend1->SetEntrySeparation(0.25);


c->GetListOfPrimitives()->RemoveAt(6);
TPaveText* pave = new TPaveText(0.7,0.91,0.95,0.99, "NDC");
pave->SetTextFont(42);
pave->AddText("35.9 fb^{-1} (13 TeV)");
pave->SetFillColor(0);
pave->SetFillStyle(0);
pave->SetBorderSize(0);
pave->Draw("same"); 

c->SaveAs("updated_Limits_eemm.pdf");


}

