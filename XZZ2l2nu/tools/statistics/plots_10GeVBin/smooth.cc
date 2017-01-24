#include "TROOT.h"
#include "TFile.h"
#include "TH1D.h"
#include "TObject.h"
#include "TF1.h"
#include "TList.h"
#include "TCanvas.h"
#include "Keys.h"
#include "TAxis.h"
#include "string"
#include "tdrstyle.h"

int main(int argc, char** argv) {

  if( argc<4 ) {
     std::cout << argv[0] << ":  " << std::endl ;
     std::cout << " Functionality: Smoothing TH1D  ... "  << std::endl;
     std::cout << "                 "  << std::endl;
     std::cout << " usage: " << argv[0] << " inputfile.root outputfile.root Region " << std::endl ;
     std::cout << "                where Region can be SR, CR1, CR2, CR3. "  << std::endl;
     exit(1) ;
  }

  // style
  tdrstyle();

  // 
  char name[1000];

  // input file name
  std::string _file_in_name((const char*)argv[1]);

  // output file name
  std::string _file_out_name((const char*)argv[2]);

  // region
  std::string _region((const char*)argv[3]);

  // output plots 
  TCanvas* plots = new TCanvas("plots", "plots");
  sprintf(name, "%s.pdf[", _file_out_name.c_str());
  plots->Print(name);

  TFile* file_in = TFile::Open(_file_in_name.c_str());
  TFile* file_out = TFile::Open(_file_out_name.c_str(), "recreate");

  // get list of objects
  TList* list_keys = (TList*)file_in->GetListOfKeys();

  for (int ik=0; ik<list_keys->GetSize(); ik++)
  {

    TObject* obj = (TObject*)file_in->Get(list_keys->At(ik)->GetName());
    std::string obj_name(obj->GetName());
    std::string obj_class(obj->ClassName());

    // smooth all TH1D    
    if ((obj_class == "TH1D") && (obj_name.find("hratio")==std::string::npos) && (obj_name.find("_data_")==std::string::npos) ) 
    {

      TH1D* hfunc = (TH1D*)obj->Clone("hfunc");
      TH1D* h1 = (TH1D*)obj->Clone("h1");

      sprintf(name, "old_%s", obj_name.c_str());
      TH1D* h1_old = (TH1D*)obj->Clone(name);

      double xmin = 140;//h1->GetXaxis()->GetXmin(); //150; //83;
      double xmax = h1->GetXaxis()->GetXmax(); //3000;//98.0;

      if (_region=="CR3") xmax = 270;
      else if (_region=="CR2") xmax = 500;
      else if (_region=="CR1") xmax = 520;

      // upper cut off
      double xcutoff = 1e30;
      if (_region=="CR3") xcutoff = 270;
      else if (_region=="CR2") xcutoff = 550;
      else if (_region=="CR1") xcutoff = 650;    


      int nbin = h1->FindBin(xmax); //h1->GetNbinsX();
      int ndiv = 8;
      double par[6+3*nbin];

      if (_region=="SR") ndiv = 2;
      else ndiv = 8;

      KeysPar(h1,nbin,ndiv,par);

      sprintf(name, "func_%s", obj_name.c_str());
      TF1* func = new TF1(name, KeysPdf, xmin, xmax, 6+3*nbin);
      func->SetNpx(3000);

      for (int ik=0; ik<6+3*nbin; ik++){
        func->FixParameter(ik, par[ik]);
      }

      func->ReleaseParameter(0);
      h1->Fit(func, "WL R", "", xmin, xmax);

      std::cout << "original hist integral = " << h1_old->Integral("width") << std::endl;
      std::cout << "smoothed hist integral = " << hfunc->Integral("width") << std::endl;

      h1->SetMarkerStyle(20);
      func->SetLineColor(2);

      for (int i=0; i<=hfunc->GetNbinsX(); i++) {
        hfunc->SetBinContent(i, func->Eval(hfunc->GetBinCenter(i)));
        hfunc->SetBinError(i, 0);
        // set to 0 for bins below xmin
        if (hfunc->GetBinCenter(i)<xmin) hfunc->SetBinContent(i,1e-30);
        // upper bins
        if (hfunc->GetBinCenter(i)>xcutoff) hfunc->SetBinContent(i,1e-30);
      }

      hfunc->SetLineColor(4);
      hfunc->SetMarkerColor(4);
      hfunc->SetFillColor(0);
      hfunc->SetLineStyle(1);
      hfunc->SetLineWidth(3);

      // plot
      plots->Clear();
      plots->SetLogy();
      h1->Draw("hist e");
      func->Draw("same");
      hfunc->Draw("same hist");
      sprintf(name, "%s.pdf", _file_out_name.c_str());
      plots->Print(name);
      plots->SetLogy(0);
      plots->Clear();

      // save 
      file_out->cd();
      h1_old->Write();
      func->Write();
      sprintf(name, "%s", obj_name.c_str());
      hfunc->Write(name);


      // delete
      func->Delete();
      hfunc->Delete();
      h1_old->Delete();
      h1->Delete();
    }
    else // only save the obj
    {
      // 
      file_out->cd();
      obj->Write();
    }
  }

  // close plot file
  sprintf(name, "%s.pdf]", _file_out_name.c_str());
  plots->Print(name);

  file_out->Close();

}
