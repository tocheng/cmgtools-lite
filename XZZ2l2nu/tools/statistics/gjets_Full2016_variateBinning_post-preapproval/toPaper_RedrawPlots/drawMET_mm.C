{

gROOT->ProcessLine(".x tdrstyle.C");

//gStyle->SetErrorX(0.);
 
TFile* file=TFile::Open("plot_mlfit_met_shapes_prefit_mm_SR_False.root");


/*
Canvas Name=plots Title=plots Option=
 TCanvas fXlowNDC=0 fYlowNDC=0 fWNDC=1 fHNDC=1 Name= plots Title= plots Option=
  OBJ: TList	TList	Doubly linked list : 0
   TFrame  X1= 0.000000 Y1=-3.000000 X2=3000.000000 Y2=6.000000
   OBJ: TH1D	dummy	dummy : 1 at: 0x7fc1fd687e70
   THStack Name= stack Title= stack Option=
    OBJ: TList	TList	Doubly linked list : 0
     OBJ: TH1D	h_nonreso	nonreso : 0 at: 0x7fc1fd69efd0
     OBJ: TH1D	h_vvreso	vvreso : 0 at: 0x7fc1fd69f500
     OBJ: TH1D	h_zjets	zjets : 0 at: 0x7fc1fd69fb40
   OBJ: TGraphAsymmErrors	gr_data	Graph : 1 at: 0x7fc1fd6a6820
   OBJ: TH1D	h_signal	signal : 1 at: 0x7fc1fd6aaf20
   Text  X=0.950000 Y=0.940000 Text=35.9 fb^{-1} (13 TeV)
   Text  X=0.250000 Y=0.850000 Text=CMS
   Text  X=0.660000 Y=0.560000 Text=Post-fit B-only
   Text  X=0.660000 Y=0.500000 Text=#mu#mu channel
   OBJ: TH1D	dummy_copy	dummy : 1 at: 0x7fc1fd6af620
   TPad fXlowNDC=0 fYlowNDC=0 fWNDC=1 fHNDC=1 Name= pad Title= pad Option=
    OBJ: TList	TList	Doubly linked list : 0
     TFrame  X1= 0.000000 Y1=0.000000 X2=3000.000000 Y2=2.000000
     OBJ: TH1D	dummyR	dummyR : 1 at: 0x7fc1fd6b0300
     OBJ: TH1D	h_line	dummyR : 1 at: 0x7fc1fd6b0770
     OBJ: TH1D	h_band_sys	 : 1 at: 0x7fc1fd6b0bc0
     OBJ: TH1D	h_ratio	data : 1 at: 0x7fc1fd6b1260
   OBJ: TLegend	TPave  	X1= 1481.012670 Y1=2.142857 X2=2810.126583 Y2=5.571429
   OBJ: TH1D	dummy_copy	dummy : 1 at: 0x7fc1fd6b99b0
*/

TCanvas* plots = (TCanvas*)file->Get("plots");
TPad* pad = (TPad*)plots->FindObject("pad");


TH1D* dummy = (TH1D*)plots->FindObject("dummy");
TH1D* dummyR = (TH1D*)plots->FindObject("dummyR");

TGraphAsymmErrors* gr_data = (TGraphAsymmErrors*)plots->FindObject("gr_data");

TH1D* h_band_sys = (TH1D*)plots->FindObject("h_band_sys");

TLegend* legend = (TLegend*)plots->FindObject("TPave")->Clone("legend");

TH1D* h_line = (TH1D*)plots->FindObject("h_line")->Clone();
TH1D* h_ratio = (TH1D*)plots->FindObject("h_ratio")->Clone();
plots->Draw();


//plots->GetListOfPrimitives()->RemoveAt(7);
//((TLatex*)plots->GetListOfPrimitives()->At(8))->

h_band_sys->SetFillColor(kGreen-8);
h_band_sys->SetFillStyle(1000);


dummyR->GetYaxis()->SetNdivisions(508,kTRUE);
dummyR->GetYaxis()->SetRangeUser(0.01,2.99);
dummyR->GetYaxis()->SetLabelSize(0.03);
dummyR->GetXaxis()->SetLabelSize(0.05);
//dummyR->GetXaxis()->SetTitleSize(0.06);
dummyR->GetXaxis()->SetTitle("p_{T}^{miss} (GeV)");



gr_data->SetPointEYlow(13,0);
gr_data->SetPointEYhigh(13,1.84105);

// to fix problem that error bar do not reach zero, double draw with zero marker size.
TGraphAsymmErrors* data_dummy = (TGraphAsymmErrors*)gr_data->Clone("data_dummy");
data_dummy->SetMarkerSize(0);
data_dummy->Draw("p same");

legend->GetListOfPrimitives()->RemoveAt(5);
legend->AddEntry(h_band_sys, "Syst. uncertainty", "f");
((TLegendEntry*)legend->GetListOfPrimitives()->At(2))->SetLabel("Nonreson. backgrounds");
((TLegendEntry*)legend->GetListOfPrimitives()->At(4))->SetLabel("1 pb bulk G, M = 1 TeV");
legend->Draw("same");

TPaveText* pavetext_channel = new TPaveText(0.7,0.57,0.9,0.62, "NDC");
pavetext_channel->SetTextFont(42);
pavetext_channel->AddText("#mu#mu channel");
pavetext_channel->SetFillColor(0);
pavetext_channel->SetFillStyle(0);
pavetext_channel->SetBorderSize(0);
pavetext_channel->Draw("same");

pad->cd();
//dummyR->Draw();
//h_band_sys->Draw("same");
h_line->Draw("same");
h_ratio->Draw("same");


plots->Update();
plots->Draw();

plots->SaveAs("updated_met_mm.pdf");
}

