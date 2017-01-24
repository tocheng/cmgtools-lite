#include "TH1.h"
#include "TMath.h"

void KeysPar(TH1* hist, int nbin, int ndiv, double* par){

  double N = hist->Integral();

  double t[nbin];
  double n[nbin];
  double w[nbin];

  for ( int i=0; i<nbin; i++ ){
    t[i] = hist->GetBinCenter(i+1);
    n[i] = hist->GetBinContent(i+1);
    w[i] = hist->GetBinWidth(i+1);
    // protection of spikes bins
    if (i+1>1 && i+1<nbin) {
      double nm = hist->GetBinContent(i);
      double np = hist->GetBinContent(i+2);

      //if (nm<=1e-30 && np<=1e-30) continue;

      //if (n[i]>5*(nm+np) || n[i]<(nm+np)/5) {
      if (n[i]>5*(nm+np)) {
        n[i] = (nm+np)/2.0;
      }
      //else 
      //if ( fabs(n[i]-(nm+np)/2)>fabs(nm-np)/2) {
      //if (n[i]>nm+np) {
    //    n[i] = (n[i]+nm+np)/3.0;
      //  n[i] = (nm+np)/2.0;
      //}
    }
  }
 
  par[0] = N;
  par[1] = N;
  par[2] = (double)nbin;
  par[3] = (double)ndiv;
  par[4] = 0.0; // initial shift
  par[5] = 0.0; // initial sigma

  for ( int i=0; i<nbin; i++ ){
    par[i+6] = t[i];
    par[i+6+nbin] = n[i];
    par[i+6+2*nbin] = w[i];
  }

  return ;
}

Double_t KeysPdf(Double_t* x, Double_t* par){

  double N = par[1];
  const int nbin = (const int)par[2];
  const int ndiv = (const int)par[3];
  double delta = par[4];
  double sigma = par[5];
  double X = x[0]-delta;

  double t[nbin];
  double n[nbin];
  double w[nbin];

  for ( int i=0; i<nbin; i++ ){
    t[i] = par[i+6];
    n[i] = par[i+6+nbin];
    w[i] = par[i+6+2*nbin];
  }

  double h[nbin];

  for ( int i=0; i<nbin; i++ ){
    if (n[i]>1e-30) {
      //h[i] = pow(4./3., 0.2) * pow(N, 0.3) * sqrt(w[i]/2./sqrt(3.)) * pow(n[i], -0.5);
      //h[i] = pow(4./3., 0.2) * pow(N, 0.5) * sqrt(w[i]/2./sqrt(3.)) * pow(n[i], -0.5);
      //h[i] = 0.5*pow(4./3., 0.2) * pow(N, 0.5) * sqrt(w[i]/2./sqrt(3.)) * pow(n[i], -0.5);
      //h[i] =  0.5*pow(4./3., 0.2)*pow(N, 0.5) * sqrt(w[i]/2./sqrt(3.)) * pow(n[i], -0.5);
      //h[i] =  1/ndiv*pow(4./3., 0.2)*pow(N, 0.5) * sqrt(w[i]/2./sqrt(3.)) * pow(n[i], -0.5);
      h[i] =  sqrt(2)/ndiv*pow(4./3., 0.2)*pow(N, 0.5) * sqrt(w[i]/2./sqrt(3.)) * pow(n[i], -0.5);




    }
    else {
      h[i]=0;
    }
  }

  double var(0);

  for ( int i=0; i<nbin; i++ ){
    if ( h[i]>0 ){
      double nm = 0;
      double np = 0;
      if (i>1) nm = n[i-1];
      if (i<nbin-1) np=n[i+1];
      double k = (nm-np)/2/w[i];
      
      //var +=  n[i]/h[i] * exp(-(X-t[i])*(X-t[i])/(2.*h[i]*h[i]));
      //var += n[i]/sqrt(h[i]*h[i]+sigma*sigma) * exp(-(X-t[i])*(X-t[i])/(2.*(h[i]*h[i]+sigma*sigma)));
      //var += n[i]/sqrt((h[i]+sigma)*(h[i]+sigma)) * exp(-(X-t[i])*(X-t[i])/(2.*(h[i]+sigma)*(h[i]+sigma)));
      //var += n[i]/2/sqrt((h[i]+sigma)*(h[i]+sigma)) * exp(-(X-(t[i]-w[i]/4))*(X-(t[i]-w[i]/4))/(2.*(h[i]+sigma)*(h[i]+sigma)));
      //var += n[i]/2/sqrt((h[i]+sigma)*(h[i]+sigma)) * exp(-(X-(t[i]+w[i]/4))*(X-(t[i]+w[i]/4))/(2.*(h[i]+sigma)*(h[i]+sigma)));
      //var += n[i]/4/sqrt((h[i]+sigma)*(h[i]+sigma)) * exp(-(X-(t[i]-w[i]*3/8))*(X-(t[i]-w[i]*3/8))/(2.*(h[i]+sigma)*(h[i]+sigma)));
      //var += n[i]/4/sqrt((h[i]+sigma)*(h[i]+sigma)) * exp(-(X-(t[i]-w[i]*1/8))*(X-(t[i]-w[i]*1/8))/(2.*(h[i]+sigma)*(h[i]+sigma)));
      //var += n[i]/4/sqrt((h[i]+sigma)*(h[i]+sigma)) * exp(-(X-(t[i]+w[i]*1/8))*(X-(t[i]+w[i]*1/8))/(2.*(h[i]+sigma)*(h[i]+sigma)));
      //var += n[i]/4/sqrt((h[i]+sigma)*(h[i]+sigma)) * exp(-(X-(t[i]+w[i]*3/8))*(X-(t[i]+w[i]*3/8))/(2.*(h[i]+sigma)*(h[i]+sigma)));
      for (int j=0; j<ndiv; j++) {
        //var += n[i]/ndiv/sqrt((h[i]+sigma)*(h[i]+sigma)) * exp(-(X-(t[i]-w[i]/2+w[i]*(2*j+1)/2/ndiv))*(X-(t[i]-w[i]/2+w[i]*(2*j+1)/2/ndiv))/(2.*(h[i]+sigma)*(h[i]+sigma)));
        var += (n[i]+k*w[i]/2-k*(2*j+1)*w[i]/2/ndiv)/ndiv/sqrt((h[i]+sigma)*(h[i]+sigma)) * exp(-(X-(t[i]-w[i]/2+w[i]*(2*j+1)/2/ndiv))*(X-(t[i]-w[i]/2+w[i]*(2*j+1)/2/ndiv))/(2.*(h[i]+sigma)*(h[i]+sigma)));
      }
    }
  }

  if (N>0) {
    return par[0]*var/N/sqrt(2*TMath::Pi()) ;
  }
  else {
    return 0;
  }
  

}
