{

gROOT->ProcessLine(".x tdrstyle.C");

//gStyle->SetErrorX(0.);
 
TFile* file=TFile::Open("PAS1_ReMiniSummer16_DT_PhReMiniMCRcFixXsec_GMCPhPtWt_SRdPhiGT0p5_puWeightsummer16_muoneg_gjet_metfilter_unblind_el_log_1pb.root");


TCanvas* zpt_c1 = (TCanvas*)file->Get("zpt_c1");
TPad* zpt_pad1 = (TPad*)zpt_c1->FindObject("zpt_pad1");
TPad* zpt_pad2 = (TPad*)zpt_c1->FindObject("zpt_pad2");

TH1F* zpt_frame = (TH1F*)zpt_c1->FindObject("zpt_frame");
THStack* zpt_stack = (THStack*)zpt_c1->FindObject("zpt_stack");
TH1D* zpt_NonReso = (TH1D*)zpt_stack->GetHists()->FindObject("zpt_NonReso");
TH1D* zpt_VVZReso = (TH1D*)zpt_stack->GetHists()->FindObject("zpt_VVZReso");
TH1D* zpt_ZJets = (TH1D*)zpt_stack->GetHists()->FindObject("zpt_ZJets");
TGraphAsymmErrors* zpt_dataG = (TGraphAsymmErrors*)zpt_c1->FindObject("zpt_dataG");
TH1D* zpt_BulkGravToZZToZlepZinv_narrow_1000 = (TH1D*)zpt_c1->FindObject("zpt_BulkGravToZZToZlepZinv_narrow_1000");
TPaveText* zpt_pavetext = (TPaveText*)zpt_c1->FindObject("zpt_pavetext");
TLegend* zpt_legend = (TLegend*)zpt_c1->FindObject("zpt_legend");
TH1F* zpt_frame_copy = (TH1F*)zpt_c1->FindObject("zpt_frame_copy");

TH1D* zpt_hratio_band = (TH1D*)zpt_c1->FindObject("zpt_hratio_band");
TH1D* zpt_hline = (TH1D*)zpt_c1->FindObject("zpt_hline");
TH1D* zpt_hratio = (TH1D*)zpt_c1->FindObject("zpt_hratio");


zpt_c1->Draw();

zpt_c1->SetWindowSize(800,800);
zpt_pad1->SetPad(0,0.19,1,0.97);
zpt_pad2->SetPad(0,0,   1,0.3);
zpt_pad2->SetBottomMargin(0.43);

zpt_frame->GetXaxis()->SetLabelSize(0);
zpt_frame->GetXaxis()->SetTitleSize(0);

zpt_hratio_band->GetXaxis()->SetTitleSize(0.17);
zpt_hratio_band->GetXaxis()->SetLabelSize(0.16);
zpt_hratio_band->GetXaxis()->SetTitle("p_{T}(Z) (GeV)");

zpt_hratio_band->GetYaxis()->SetNdivisions(505,kTRUE);
zpt_hratio_band->GetYaxis()->SetRangeUser(0.01,1.99);
zpt_hratio_band->GetYaxis()->SetLabelSize(0.1);
zpt_hratio_band->GetYaxis()->SetTitleSize(0.15);
zpt_hratio_band->GetYaxis()->SetTitleOffset(0.42);
zpt_hratio_band->SetFillColor(0);


zpt_frame->GetYaxis()->SetTitleOffset(1.1);
zpt_frame->GetYaxis()->SetTitleSize(0.06);

zpt_pad1->GetListOfPrimitives()->RemoveAt(5);

zpt_pad1->cd();
TPaveText* pave = new TPaveText(0.68,0.95,1,1, "NDC");
pave->SetTextFont(42);
pave->AddText("35.9 fb^{-1} (13 TeV)");
pave->SetFillColor(0);
pave->SetFillStyle(0);
pave->SetBorderSize(0);
pave->Draw("same");

zpt_ZJets->SetFillColor(kGreen+2);
zpt_BulkGravToZZToZlepZinv_narrow_1000->SetLineColor(kRed+2);
zpt_ZJets->SetLineWidth(3);
zpt_NonReso->SetLineWidth(3);
zpt_VVZReso->SetLineWidth(3);
zpt_BulkGravToZZToZlepZinv_narrow_1000->SetLineWidth(3);

zpt_pad1->cd();
zpt_legend->GetListOfPrimitives()->RemoveAt(5);
((TLegendEntry*)zpt_legend->GetListOfPrimitives()->At(1))->SetLabel("Reson. backgrounds");
((TLegendEntry*)zpt_legend->GetListOfPrimitives()->At(2))->SetLabel("Nonreson. backgrounds");
((TLegendEntry*)zpt_legend->GetListOfPrimitives()->At(4))->SetLabel("1 pb bulk G, M = 1 TeV");

zpt_legend->SetX1NDC(0.53);
zpt_legend->SetY1NDC(0.56);

zpt_pad1->cd();
TPaveText* zpt_pavetext_channel = new TPaveText(0.63,0.45,0.86,0.5, "NDC");
zpt_pavetext_channel->SetTextFont(42);
zpt_pavetext_channel->AddText("ee channel");
zpt_pavetext_channel->SetFillColor(0);
zpt_pavetext_channel->SetBorderSize(0);

zpt_pavetext_channel->Draw();


int np = zpt_dataG->GetN();

zpt_dataG->SetPoint(np,1225,0);
zpt_dataG->SetPointEYlow(np,0);
zpt_dataG->SetPointEYhigh(np,1.84105);
zpt_dataG->SetPoint(np+1,1325,0);
zpt_dataG->SetPointEYlow(np+1,0);
zpt_dataG->SetPointEYhigh(np+1,1.84105);
zpt_dataG->SetPoint(np+2,1375,0);
zpt_dataG->SetPointEYlow(np+2,0);
zpt_dataG->SetPointEYhigh(np+2,1.84105);
zpt_dataG->SetPoint(np+3,1425,0);
zpt_dataG->SetPointEYlow(np+3,0);
zpt_dataG->SetPointEYhigh(np+3,1.84105);
zpt_dataG->SetPoint(np+4,1475,0);
zpt_dataG->SetPointEYlow(np+4,0);
zpt_dataG->SetPointEYhigh(np+4,1.84105);

zpt_dataG->Sort();

np = zpt_dataG->GetN();
for (int i=0; i<np; i++){
  zpt_dataG->SetPointEXlow(i,25);
  zpt_dataG->SetPointEXhigh(i,25);
}

// to fix problem that error bar do not reach zero, double draw with zero marker size.
zpt_pad1->cd();
TGraphAsymmErrors* data_dummy = (TGraphAsymmErrors*)zpt_dataG->Clone("data_dummy");
data_dummy->SetMarkerSize(0);
data_dummy->Draw("p same");

zpt_c1->Update();


zpt_c1->Draw();

zpt_c1->SaveAs("updated_zpt_ee.pdf");

}

