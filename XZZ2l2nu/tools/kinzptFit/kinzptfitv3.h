#include "Minuit2/FCNGradientBase.h"
#include "Minuit2/MnUserParameters.h"
#include "TComplex.h"
#include "TH1D.h"
#include "math.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <time.h>

// 
class MetChi2Fcn : public ROOT::Minuit2::FCNBase {

public:
  MetChi2Fcn(void) {};
  ~MetChi2Fcn() {}

  void InitFunction(const double& z_pt, const double& z_phi, 
              const double& met_pt, const double& met_phi,
              const std::vector<double>& jets_pt, const std::vector<double>& jets_phi,
              const std::vector<double>& jets_sigma)
  {
     _z_pt = z_pt;
     _z_phi = z_phi;
     _met_pt = met_pt;
     _met_phi = met_phi;
     _jets_pt = jets_pt;
     _jets_phi = jets_phi;
     _jets_sigma = jets_sigma;
     _njets = (int)jets_pt.size();
  }

  virtual double operator()(const std::vector<double>& par) const
  {
     return getChi2(par);
  }


  double getChi2(const std::vector<double>& par) const
  {

    //double sigma_met_para = 15.3016 + 0.0262152*_z_pt;
    //double sigma_met_perp = 15.2906 + 0.025963 *_z_pt;
    // 1.11754 = 16.3326/14.6148 
    //double sigma_met_para = 1.11754*1.11754*(13.1218 + 0.0457877*_z_pt);
    //double sigma_met_perp = 1.11754*1.11754*(13.2456 + 0.0189742*_z_pt);
    //double sigma_met_para = (13.2456 + 0.0189742*_z_pt);
    //double sigma_met_perp = (13.2456 + 0.0189742*_z_pt);
    //double sigma_met_para = 1.11754*(13.2456 + 0.0189742*_z_pt);
    //double sigma_met_perp = 1.11754*(13.2456 + 0.0189742*_z_pt);
    double sigma_met_para = 3.*(13.2456 + 0.0189742*_z_pt);
    double sigma_met_perp = 3.*(13.2456 + 0.0189742*_z_pt);
    double new_met_para = _met_pt*cos(_met_phi-_z_phi);
    double new_met_perp = _met_pt*sin(_met_phi-_z_phi);
    double chi2 = 0;
    for (int i=0; i<(int)_njets; i++){
      chi2 += pow((par.at(i)-1.0)/_jets_sigma.at(i), 2);
      new_met_para += (1.0-par.at(i))*_jets_pt.at(i)*cos(_jets_phi.at(i)-_z_phi);
      new_met_perp += (1.0-par.at(i))*_jets_pt.at(i)*sin(_jets_phi.at(i)-_z_phi);
    }

    chi2 += pow(new_met_para/sigma_met_para, 2); 
    chi2 += pow(new_met_perp/sigma_met_perp, 2); 


    return chi2;
 
  }

  virtual double Up() const {return 1.0; }

 // double GetMetPara() const { return _metPara; }
 // std::vector<double> GetJetsPara() const { return _jetsPara; }
 // std::vector<double> GetJetsSigma() const { return _jetsSigma; }

  
private:
  double _z_pt;
  double _z_phi;
  double _met_pt;
  double _met_phi;
  std::vector<double> _jets_pt;
  std::vector<double> _jets_phi;
  std::vector<double> _jets_sigma;
  
  int _njets;   
};


