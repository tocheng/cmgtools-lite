{
TFile* fdt = new TFile("/data/XZZ/80X_Ntuple/80X_20160606_NoHLT_Skim/SingleMuon_Run2016B_PromptReco_v2.root");
TFile* fmc = new TFile("/data/XZZ/80X_Ntuple/80X_20160606_NoHLT_Skim/DYJetsToLL_M50.root");

std::string output("studybs_2016b");
char name[1000];

sprintf(name, "%s.root", output.c_str());
TFile* fout = new TFile(name, "recreate");

gROOT->ProcessLine(".x tdrstyle.C");


TTree* tdt = (TTree*)fdt->Get("tree");
TTree* tmc = (TTree*)fmc->Get("tree");


TH1D* hdt_x = new TH1D("hdt_x", "Data Vertex X",100,0.056,0.075);
TH1D* hdt_y = new TH1D("hdt_y", "Data Vertex Y",100,0.08,0.105);
TH1D* hdt_z = new TH1D("hdt_z", "Data Vertex Z",100,-15,15);

TH1D* hmc_x = new TH1D("hmc_x", "MC Vertex X",100,0.08,0.12);
TH1D* hmc_y = new TH1D("hmc_y", "MC Vertex Y",100,0.155,0.18);
TH1D* hmc_z = new TH1D("hmc_z", "MC Vertex Z",100,-15,15);

TH1D* hmc_corr_x = new TH1D("hmc_corr_x", "Corrected MC Vertex X",100,0.056,0.075);
TH1D* hmc_corr_y = new TH1D("hmc_corr_y", "Corrected MC Vertex Y",100,0.08,0.105);
TH1D* hmc_corr_z = new TH1D("hmc_corr_z", "Corrected MC Vertex Z",100,-15,15);

hdt_x->Sumw2();
hdt_y->Sumw2();
hdt_z->Sumw2();
hmc_x->Sumw2();
hmc_y->Sumw2();
hmc_z->Sumw2();
hmc_corr_x->Sumw2();
hmc_corr_y->Sumw2();
hmc_corr_z->Sumw2();

hdt_x->SetLineColor(1);
hdt_y->SetLineColor(1);
hdt_z->SetLineColor(1);
hmc_x->SetLineColor(4);
hmc_y->SetLineColor(4);
hmc_z->SetLineColor(4);
hmc_corr_x->SetLineColor(4);
hmc_corr_y->SetLineColor(4);
hmc_corr_z->SetLineColor(4);

hdt_x->SetMarkerColor(1);
hdt_y->SetMarkerColor(1);
hdt_z->SetMarkerColor(1);
hmc_x->SetMarkerColor(4);
hmc_y->SetMarkerColor(4);
hmc_z->SetMarkerColor(4);
hmc_corr_x->SetMarkerColor(4);
hmc_corr_y->SetMarkerColor(4);
hmc_corr_z->SetMarkerColor(4);

hdt_x->SetMarkerStyle(20);
hdt_y->SetMarkerStyle(20);
hdt_z->SetMarkerStyle(20);

hdt_x->GetXaxis()->SetTitle("vertex x (cm)");
hdt_y->GetXaxis()->SetTitle("vertex y (cm)");
hdt_z->GetXaxis()->SetTitle("vertex z (cm)");
hmc_x->GetXaxis()->SetTitle("vertex x (cm)");
hmc_y->GetXaxis()->SetTitle("vertex y (cm)");
hmc_z->GetXaxis()->SetTitle("vertex z (cm)");
hmc_corr_x->GetXaxis()->SetTitle("vertex x (cm)");
hmc_corr_y->GetXaxis()->SetTitle("vertex y (cm)");
hmc_corr_z->GetXaxis()->SetTitle("vertex z (cm)");

TF1* fc_dt_x = new TF1("fc_dt_x", "gaus", -15,15);
TF1* fc_dt_y = new TF1("fc_dt_y", "gaus", -15,15);
TF1* fc_dt_z = new TF1("fc_dt_z", "gaus", -15,15);
TF1* fc_mc_x = new TF1("fc_mc_x", "gaus", -15,15);
TF1* fc_mc_y = new TF1("fc_mc_y", "gaus", -15,15);
TF1* fc_mc_z = new TF1("fc_mc_z", "gaus", -15,15);

TCanvas* plots = new TCanvas("plots", "plots", 600,600);

sprintf(name, "%s_plots.ps", output.c_str());
plots->Print(name);

// data
tdt->Draw("vtx_x>>hdt_x");
tdt->Draw("vtx_x>>hdt_y");
tdt->Draw("vtx_x>>hdt_z");
// mc
tmc->Draw("vtx_x>>hmc_x");
tmc->Draw("vtx_x>>hmc_y");
tmc->Draw("vtx_x>>hmc_z");

// fit data 
hdt_x->Fit(fc_dt_x, "R", "", 0.062, 0.067);
hdt_y->Fit(fc_dt_y, "R", "", 0.092, 0.098);
hdt_z->Fit(fc_dt_z, "R", "", -10, 10);
hmc_x->Fit(fc_mc_x, "R", "", 0.1, 0.11);
hmc_y->Fit(fc_mc_y, "R", "", 0.165, 0.172);
hmc_z->Fit(fc_mc_z, "R", "", -10, 10);


double dt_vtx_x_mean = fc_dt_x->GetParameter(1);
double dt_vtx_y_mean = fc_dt_y->GetParameter(1);
double dt_vtx_z_mean = fc_dt_z->GetParameter(1);
double mc_vtx_x_mean = fc_mc_x->GetParameter(1);
double mc_vtx_y_mean = fc_mc_y->GetParameter(1);
double mc_vtx_z_mean = fc_mc_z->GetParameter(1);
 

double dt_vtx_x_sigma = fc_dt_x->GetParameter(2);
double dt_vtx_y_sigma = fc_dt_y->GetParameter(2);
double dt_vtx_z_sigma = fc_dt_z->GetParameter(2);
double mc_vtx_x_sigma = fc_mc_x->GetParameter(2);
double mc_vtx_y_sigma = fc_mc_y->GetParameter(2);
double mc_vtx_z_sigma = fc_mc_z->GetParameter(2);

// correction:
// starting from equation:
//    (corr_mc_vtx_x - dt_vtx_x_mean) / (mc_vtx_x - mc_vtx_x_mean) = dt_vtx_x_sigma / mc_vtx_x_sigma
// we can have:
//   corr_mc_vtx_x = dt_vtx_x_mean + (mc_vtx_x - mc_vtx_x_mean) * dt_vtx_x_sigma / mc_vtx_x_sigma
// same applies to y and z.

// prediction:
sprintf(name, "(%f+(vtx_x-%f)*%f/%f)>>hmc_corr_x", dt_vtx_x_mean, mc_vtx_x_mean, dt_vtx_x_sigma, mc_vtx_x_sigma);
tmc->Draw(name);
sprintf(name, "(%f+(vtx_y-%f)*%f/%f)>>hmc_corr_y", dt_vtx_y_mean, mc_vtx_y_mean, dt_vtx_y_sigma, mc_vtx_y_sigma);
tmc->Draw(name);
sprintf(name, "(%f+(vtx_z-%f)*%f/%f)>>hmc_corr_z", dt_vtx_z_mean, mc_vtx_z_mean, dt_vtx_z_sigma, mc_vtx_z_sigma);
tmc->Draw(name);

// norm to data
hmc_corr_x->Scale(hdt_x->Integral()/hmc_corr_x->Integral());
hmc_corr_y->Scale(hdt_y->Integral()/hmc_corr_y->Integral());
hmc_corr_z->Scale(hdt_z->Integral()/hmc_corr_z->Integral());

// plotting
TLegend* lg_x = new TLegend(0.6,0.7,0.8,0.9);
TLegend* lg_y = new TLegend(0.6,0.7,0.8,0.9);
TLegend* lg_z = new TLegend(0.6,0.7,0.8,0.9);
lg_x->SetName("lg_x");
lg_y->SetName("lg_y");
lg_z->SetName("lg_z");

lg_x->AddEntry(hdt_x, "Data", "pl");
lg_x->AddEntry(hmc_corr_x, "Corrected MC", "l");
lg_y->AddEntry(hdt_y, "Data", "pl");
lg_y->AddEntry(hmc_corr_y, "Corrected MC", "l");
lg_z->AddEntry(hdt_z, "Data", "pl");
lg_z->AddEntry(hmc_corr_z, "Corrected MC", "l");

// dt x
plots->Clear();
hdt_x->Draw();
sprintf(name, "%s_plots.ps", output.c_str());
plots->Print(name);
plots->Clear();
// mc x
plots->Clear();
hmc_x->Draw("HIST");
sprintf(name, "%s_plots.ps", output.c_str());
plots->Print(name);
plots->Clear();
// corr x
plots->Clear();
hdt_x->Draw();
hmc_corr_x->Draw("hist");
lg_x->Draw();
sprintf(name, "%s_plots.ps", output.c_str());
plots->Print(name);
plots->Clear();


// close
sprintf(name, "%s_plots.ps]", output.c_str());
plots->Print(name);

sprintf(name, ".! ps2pdf %s_plots.ps", output.c_str());
gROOT->ProcessLine(name);


fout->cd();
hdt_x->Write();
hdt_y->Write();
hdt_z->Write();
hmc_x->Write();
hmc_y->Write();
hmc_z->Write();
hmc_corr_x->Write();
hmc_corr_y->Write();
hmc_corr_z->Write();
fc_dt_x->Write();
fc_dt_y->Write();
fc_dt_z->Write();
fc_mc_x->Write();
fc_mc_y->Write();
fc_mc_z->Write();
lg_x->Write();
lg_y->Write();
lg_z->Write();


fout->Close();



}