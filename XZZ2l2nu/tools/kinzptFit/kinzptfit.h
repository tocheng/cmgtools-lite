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

  void InitFunction(const double& z_pt, const double& met_pt, const double& met_dphi,
              const std::vector<double>& jets_pt, const std::vector<double>& jets_dphi,
              const std::vector<double>& jets_sigma)
  {
     _z_pt = z_pt;
     _met_pt = met_pt;
     _met_dphi = met_dphi;
     _jets_pt = jets_pt;
     _jets_dphi = jets_dphi;
     _jets_sigma = jets_sigma;
  }
 
  virtual double operator()(const std::vector<double>& par) const
  {
    //double _newMetPara = _metPara;
    //for (int i=0; i<(int)_jetsPara.size(); i++){
    //  _newMetPara += (par.at(i)-1.0)*_jetsPara.at(i);
    //}
    //return _newMetPara;

    // define chi2 to minimize met + sum{ (k_i-1)*pt_i } => 0
    // Chi2 = (met + sum{ (k_i-1)*p_i })^2 / sigma_met^2
    // sigma_met^2 = sum{ (k_i-1)^2 * sigma_p_i^2 }
/*
    double sigma_met2 = 0;
    double sum_jets_para = 0;
    for (int i=0; i<(int)_jetsSigma.size(); i++){
      sigma_met2 += (par.at(i)-1.0)*(par.at(i)-1.0)*_jetsSigma.at(i)*_jetsSigma.at(i)*_jetsPara.at(i)*_jetsPara.at(i);
      sum_jets_para += (par.at(i)-1.0)*_jetsPara.at(i);
    } 
    double chi2 = (_metPara+sum_jets_para)*(_metPara+sum_jets_para)/sigma_met2;
*/


    // below this caluclation is wrong.
    // met' = met - sum(pi) + sum(ki*pi) // substruct jets pt and add it back with scales
    // because the met and jets are opsition sign
    // should be
    // met' = met + sum(pi) - sum(ki*pi)
    //  which means add back jets pt and substract the scaled jets pt.

    double sigma_met_para2 = pow(15.3016 + 0.0262152*_z_pt, 2);
    double sigma_met_perp2 = pow(15.2906 + 0.025963 *_z_pt, 2);
    double sum_jets_para = 0;
    double sum_jets_perp = 0;
    for (int i=0; i<(int)_jets_sigma.size(); i++){
     // sigma_met_para2 += (par.at(i)-1.0)*(par.at(i)-1.0)*_jets_sigma.at(i)*_jets_sigma.at(i)*_jets_pt.at(i)*_jets_pt.at(i)*cos(_jets_dphi.at(i))*cos(_jets_dphi.at(i));
     // sigma_met_perp2 += (par.at(i)-1.0)*(par.at(i)-1.0)*_jets_sigma.at(i)*_jets_sigma.at(i)*_jets_pt.at(i)*_jets_pt.at(i)*sin(_jets_dphi.at(i))*sin(_jets_dphi.at(i));
      sum_jets_para += (1.0-par.at(i))*_jets_pt.at(i)*cos(_jets_dphi.at(i));
      sum_jets_perp += (1.0-par.at(i))*_jets_pt.at(i)*sin(_jets_dphi.at(i));
    }

    //std::cout << " sigma_met_para = " << sqrt(sigma_met_para2) << ", sigma_met_perp = " << sqrt(sigma_met_perp2) << std::endl;
 
    double chi2 = 0;
    chi2 += pow(_met_pt*cos(_met_dphi)+sum_jets_para,2)/sigma_met_para2;
    chi2 += pow(_met_pt*sin(_met_dphi)+sum_jets_perp,2)/sigma_met_perp2;

    return chi2;

/*
    double sigma_met_para = 15.3016 + 0.0262152*_z_pt;
    double sigma_met_perp = 15.2906 + 0.025963 *_z_pt;
    double chi2_para = pow(_met_pt*cos(_met_dphi)/sigma_met_para, 2);
    double chi2_perp = pow(_met_pt*sin(_met_dphi)/sigma_met_perp, 2);

    for (int i=0; i<(int)_jets_sigma.size(); i++){
      // the equation is actually 
      // [(1-ki)*pi*cos(dphii)/dpi*cos(dphii)]^2,
      // but cos(dphii) cancels out... so simplified to be:
      double chi2_jet = pow((1.0-par.at(i))*_jets_pt.at(i)/_jets_sigma.at(i), 2);
      chi2_para += chi2_jet;
      chi2_perp += chi2_jet;
    }

    //if (debug) std::cout << " chi2_para = " << chi2_para << ", chi2_perp = " << chi2_perp2 << std::endl;
 
    double chi2 = chi2_para+chi2_perp;

    return chi2;
 */
  }

  virtual double Up() const {return 1.0; }

 // double GetMetPara() const { return _metPara; }
 // std::vector<double> GetJetsPara() const { return _jetsPara; }
 // std::vector<double> GetJetsSigma() const { return _jetsSigma; }

  
private:
  double _z_pt;
  double _met_pt;
  double _met_dphi;
  std::vector<double> _jets_pt;
  std::vector<double> _jets_dphi;
  std::vector<double> _jets_sigma;
  
     
};


